from flask_sqlalchemy import SQLAlchemy
import datetime

db = SQLAlchemy()


def configure(app):
    db.init_app(app)
    app.db = db


# Class Sound
class Sound(db.Model):
    id = db.Column(db.Integer, primary_key=True)
    sound = db.Column(db.Float)
    date = db.Column(db.DateTime, default=datetime.datetime.utcnow)

    def __init__(self, sound):
        self.sound = sound


# Class temperature
class Temperature(db.Model):
    id = db.Column(db.Integer, primary_key=True)
    temperature = db.Column(db.Float)
    date = db.Column(db.DateTime, default=datetime.datetime.utcnow)

    def __init__(self, temperature):
        self.temperature = temperature


# Class Humidity
class Humidity(db.Model):
    id = db.Column(db.Integer, primary_key=True)
    humidity = db.Column(db.Integer)
    date = db.Column(db.DateTime, default=datetime.datetime.utcnow)

    def __init__(self, humidity):
        self.humidity = humidity


# Class light
class Light(db.Model):
    id = db.Column(db.Integer, primary_key=True)
    light = db.Column(db.Integer)
    date = db.Column(db.DateTime, default=datetime.datetime.utcnow)

    def __init__(self, light):
        self.light = light
