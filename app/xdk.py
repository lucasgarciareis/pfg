from flask import Blueprint, current_app, request, jsonify
from .model import Temperature, Humidity, Light
from .serializer import TemperatureSchema

bp_xdk = Blueprint('xdk', __name__)


@bp_xdk.route('/temperature', methods=['GET'])
def gettemp():
    ss = TemperatureSchema(many=True)
    result = Temperature.query.all()
    return ss.jsonify(result), 200


@bp_xdk.route('/xdk', methods=['POST'])
def posttemp():

    data = request.get_json()

    temp = request.json['temperature']
    humd = request.json['humidity']
    light = request.json['light']

    new_temp = Temperature(temp)
    new_humd = Humidity(humd)
    new_light = Light(light)

    current_app.db.session.add(new_temp)
    current_app.db.session.add(new_humd)
    current_app.db.session.add(new_light)

    current_app.db.session.commit()

    return jsonify(data), 201
