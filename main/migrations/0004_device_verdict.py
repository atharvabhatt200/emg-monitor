# Generated by Django 3.2.12 on 2023-04-21 19:21

from django.db import migrations, models


class Migration(migrations.Migration):

    dependencies = [
        ('main', '0003_rename_moisture_level_device_analog_input'),
    ]

    operations = [
        migrations.AddField(
            model_name='device',
            name='verdict',
            field=models.CharField(default='Healthy', max_length=100),
        ),
    ]
