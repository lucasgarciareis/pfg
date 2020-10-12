from flask_marshmallow import Marshmallow
from marshmallow import fields
from .model import Sound, Temperature, Light, Humidity

ma = Marshmallow()


def configure(app):
    ma.init_app(app)


class SoundSchema(ma.Schema):
    class Meta:
        fields = ('id', 'sound', 'date')


class TemperatureSchema(ma.Schema):
    class Meta:
        model = Temperature


class HumiditySchema(ma.Schema):
    class Meta:
        model = Humidity


class LightSchema(ma.Schema):
    class Meta:
        model = Light
