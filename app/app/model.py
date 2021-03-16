from flask_sqlalchemy import SQLAlchemy
from time import time
# from datetime import datetime, timedelta

db = SQLAlchemy()


def configure(app):
    db.init_app(app)
    app.db = db


# Class Sound
class Sound(db.Model):
    id = db.Column(db.Integer, primary_key=True)
    sound = db.Column(db.Float)
    date = db.Column(db.Numeric(precision=17, scale=7,
                                asdecimal=False))

    def __init__(self, sound, date):
        self.sound = sound
        self.date = date


# Class temperature
class Temperature(db.Model):
    id = db.Column(db.Integer, primary_key=True)
    temperature = db.Column(db.Float)
    # date = db.Column(db.DateTime, default=datetime.datetime.utcnow)
    date = db.Column(db.Float)

    def __init__(self, temperature):
        self.temperature = temperature


# Class Humidity
class Humidity(db.Model):
    id = db.Column(db.Integer, primary_key=True)
    humidity = db.Column(db.Integer)
    # date = db.Column(db.DateTime, default=datetime.datetime.utcnow)
    date = db.Column(db.Float)

    def __init__(self, humidity):
        self.humidity = humidity


# Class light
class Light(db.Model):
    id = db.Column(db.Integer, primary_key=True)
    light = db.Column(db.Integer)
    # date = db.Column(db.DateTime, default=datetime.datetime.utcnow)
    date = db.Column(db.Float)

    def __init__(self, light):
        self.light = light


# Class vibration
class Vibration(db.Model):
    id = db.Column(db.Integer, primary_key=True)
    vibration = db.Column(db.Boolean)
    date = db.Column(db.Numeric(precision=17, scale=7, asdecimal=False))

    def __init__(self, vibration, date):
        self.vibration = vibration
        self.date = date