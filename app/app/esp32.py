from flask import Blueprint, current_app, request, jsonify
from .model import Sound, Movement, Pressure
from .serializer import SoundSchema, MovementSchema, PressureSchema
from time import time
from datetime import datetime, timedelta

bp_esp32 = Blueprint('esp32', __name__)


@bp_esp32.route('/sound', methods=['GET'])
def getsound():
    ss = SoundSchema(many=True)
    result = Sound.query.order_by(Sound.id.desc()).limit(100)
    return ss.jsonify(result), 200


@bp_esp32.route('/movement', methods=['GET'])
def getmovement():
    ms = MovementSchema(many=True)
    result = Movement.query.order_by(Movement.id.desc()).limit(100)
    return ms.jsonify(result), 200


@bp_esp32.route('/movement/still', methods=['GET'])
def getstill():
    ms = MovementSchema(many=True)
    result = Movement.query.order_by(Movement.id.desc()).limit(500)
    return ms.jsonify(result), 200


@bp_esp32.route('/pressure', methods=['GET'])
def getpressure():
    ps = PressureSchema(many=True)
    result = Pressure.query.order_by(Pressure.id.desc()).limit(150)
    return ps.jsonify(result), 200


@bp_esp32.route('/esp32', methods=['POST'])
def postesp32():

    data = request.get_json()

    lista_s = request.json['sound']
    lista_m = request.json['movement']
    lista_p = request.json['pressure']

    received_time = time() - 1.02

    for sound, movem, press in zip(lista_s, lista_m, lista_p):
        received_time = received_time + 0.02
        # print(str(received_time) + ' : ' +
        #      datetime.fromtimestamp(received_time).strftime('%Y-%m-%d %H:%M:%S.%f'))
        new_sound = Sound(sound, received_time)
        new_movem = Movement(movem, received_time)
        new_press = Pressure(press, received_time)
        current_app.db.session.add(new_sound)
        current_app.db.session.add(new_movem)
        current_app.db.session.add(new_press)

    current_app.db.session.commit()
    return jsonify(data), 201
