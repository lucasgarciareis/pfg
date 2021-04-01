from flask import Blueprint, current_app, request, jsonify
from .model import Temperature, Humidity, Light
from time import time
from .serializer import TemperatureSchema, LightSchema, HumiditySchema

bp_xdk = Blueprint('xdk', __name__)


@bp_xdk.route('/light', methods=['GET'])
def getlight():
    ss = LightSchema(many=True)
    result = Light.query.order_by(Light.id.desc()).limit(20)
    return ss.jsonify(result), 200


@bp_xdk.route('/humidity', methods=['GET'])
def gethumd():
    ss = HumiditySchema(many=True)
    result = Humidity.query.order_by(Humidity.id.desc()).limit(20)
    return ss.jsonify(result), 200


@bp_xdk.route('/temperature', methods=['GET'])
def gettemp():
    ss = TemperatureSchema(many=True)
    result = Temperature.query.order_by(Temperature.id.desc()).limit(20)
    return ss.jsonify(result), 200


@bp_xdk.route('/xdk', methods=['POST'])
def posttemp():

    data = request.get_json()

    temp = request.json['temperature']
    humd = request.json['humidity']
    light = request.json['light']

    received_time = time()

    new_temp = Temperature(temp, received_time)
    new_humd = Humidity(humd, received_time)
    new_light = Light(light, received_time)

    current_app.db.session.add(new_temp)
    current_app.db.session.add(new_humd)
    current_app.db.session.add(new_light)

    current_app.db.session.commit()

    return jsonify(data), 201
