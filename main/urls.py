from django.urls import path
from .views import index, new_data, devices, add_device, delete_device, get_chart_data, test_signal

urlpatterns = [
    path("", index, name="index"),
    path("data/<str:id>", new_data, name="data"),
    path("devices", devices, name="devices"),
    path("add_device/<str:id>", add_device, name="add_devices"),
    path("delete_device/<str:id>", delete_device, name="delete_device"),
    path("test_signal/<str:id>", test_signal, name="test_signal"),
    path("get-chart-data/<str:id>", get_chart_data, name="get_chart_data"),
]
