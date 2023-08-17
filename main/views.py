from django.shortcuts import render, redirect
from .models import Device
from django.shortcuts import get_object_or_404
from django.http import JsonResponse
from django.views.decorators.csrf import csrf_exempt
import json
from sklearn.preprocessing import RobustScaler
import numpy as np
import requests
import os

model_url = os.environ.get('MODEL_HOST', ''),
# model_url = "bac9-34-67-80-128.ngrok.io"

# Create your views here.

sampling_freq = 1000

@csrf_exempt
def new_data(request, id):
    device = get_object_or_404(Device, device_id=id)
    # device.analog_input.append(min(150, m))
    json_data = request.body.decode('utf-8')
    data_dict = json.loads(json_data)  

    if len(device.analog_input) >= sampling_freq:
        device.analog_input.clear()

    # iterate over the values in the dictionary
    for value in data_dict.values():
        # do something with each value
        device.analog_input.append(value)

    # while len(device.analog_input) > 2000:
    #     device.analog_input.pop(0)

    # print(m)
    device.save()
    return JsonResponse({"status": "ok"})


def index(request):
    if request.user.is_authenticated is False:
        return redirect("/login")
    name = request.user.first_name
    username = request.user.username
    email = request.user.email
    devices = Device.objects.filter(user__contains=[request.user.id]).count()
    return render(
        request,
        "user.html",
        {"name": name, "username": username, "email": email, "devices": devices},
    )


def devices(request):
    if request.user.is_authenticated is False:
        return redirect("/login")
    devices = Device.objects.filter(user__contains=[request.user.id])
    print(devices)
    return render(request, "devices.html", {"devices": devices})


def add_device(request, id):
    if request.user.is_authenticated is False:
        return redirect("/login")

    device, created = Device.objects.get_or_create(device_id=id)
    device.user.append(request.user.id)
    device.save()

    return redirect("/devices")


def delete_device(request, id):
    if request.user.is_authenticated is False:
        return redirect("/login")

    device = get_object_or_404(Device, device_id=id)
    device.user.remove(request.user.id)
    if len(device.user) == 0:
        device.delete()
    else:
        device.save()
    return redirect("/devices")

@csrf_exempt
def test_signal(request, id):
    if request.user.is_authenticated is False:
        return redirect("/login")

    device = get_object_or_404(Device, device_id=id)
    analog_input = device.analog_input
    analog_input = np.array(analog_input)

    scaler = RobustScaler()
    analog_input = scaler.fit_transform(
        analog_input.reshape(-1, analog_input.shape[-1])).reshape(analog_input.shape)
    analog_input = np.reshape(analog_input, (1, sampling_freq, 1))

    data = json.dumps({"signature_name": "serving_default",
                      "instances": analog_input.tolist()})
    # print('Data: {} ... {}'.format(data[:50], data[len(data)-52:]))

    headers = {"content-type": "application/json"}
    # json_response = requests.post(
        # 'http://'+model_url+'/v1/models/emg_model:predict', data=data, headers=headers, verify=False)
    json_response = requests.post(
        f'https://{model_url}/v1/models/emg_model:predict', data=data, headers=headers, verify=False)
    # print(json_response.text)
    predictions = json.loads(json_response.text)
    prediction = predictions['predictions'][0][0]
    
    if prediction >= 0.5:
        verdict = "Unhealthy"
    else:
        verdict = "Healthy"

    device.verdict = verdict
    device.save()

    return redirect("/devices")


def get_chart_data(request, id):
    if request.user.is_authenticated is False:
        return redirect("/login")
    devices = Device.objects.filter(
        user__contains=[request.user.id], device_id=id)
    # print(id)
    analog_input = devices.first().analog_input
    # print(analog_input)
    return JsonResponse({"analog_input": analog_input})
