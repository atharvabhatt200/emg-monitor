from django.db import models
from django.contrib.auth.models import User
from django.contrib.postgres.fields import ArrayField


def get_default_array():
    return list([0 for i in range(0, 50)])


# Create your models here.
class Device(models.Model):
    user = ArrayField(
        models.IntegerField(),
        blank=True,
        default=list,
    )
    device_id = models.CharField(max_length=100)
    analog_input = ArrayField(
        models.FloatField(),
        blank=True,
        default=get_default_array,
    )

    def __str__(self) -> str:
        return self.device_id
