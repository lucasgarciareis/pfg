from flask import Blueprint, current_app, request, jsonify
from .model import Sound
from .serializer import SoundSchema
from time import time
from datetime import datetime, timedelta


bp_sound = Blueprint('sound', __name__)


@bp_sound.route('/sound', methods=['GET'])
def getsound():
    ss = SoundSchema(many=True)
    result = Sound.query.all()
    return ss.jsonify(result), 200


@bp_sound.route('/sound', methods=['POST'])
def postsound():

    data = request.get_json()

    lista = request.json['data']

    received_time = time() - 3.25

    for sound in lista:
        received_time = received_time + 0.05
        # print(str(received_time) + ' : ' +
        #      datetime.fromtimestamp(received_time).strftime('%Y-%m-%d %H:%M:%S.%f'))
        new_sound = Sound(sound, received_time)
        current_app.db.session.add(new_sound)

    current_app.db.session.commit()
    return jsonify(data), 201
