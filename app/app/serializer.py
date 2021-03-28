from flask_marshmallow import Marshmallow
from marshmallow import fields
from .model import Sound, Temperature, Light, Humidity

ma = Marshmallow()


def configure(app):
    ma.init_app(app)

class TemperatureSchema(ma.Schema):
    class Meta:
        fields = ('id', 'temperature', 'date')

class HumiditySchema(ma.Schema):
    class Meta:
        fields = ('id', 'humidity', 'date')

class LightSchema(ma.Schema):
    class Meta:
        fields = ('id', 'light', 'date')

class SoundSchema(ma.Schema):
    class Meta:
        fields = ('id', 'sound', 'date')

class MovementSchema(ma.Schema):
    class Meta:
        fields = ('id', 'movement', 'date')

class PressureSchema(ma.Schema):
    class Meta:
        fields = ('id', 'pressure', 'date')

class VibrationSchema(ma.Schema):
    class Meta:
        fields = ('id', 'vibration', 'date')

