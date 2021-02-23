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

    received_time = time()

    # para a primeira iteração marcar 1 segundo atrás
    sent_time = datetime.fromtimestamp(
        received_time) - timedelta(microseconds=1016000) - timedelta(hours=3)

    for sound in lista:
        new_sound = Sound(sound, sent_time + timedelta(microseconds=16000))
        current_app.db.session.add(new_sound)
        current_app.db.session.commit()

    return jsonify(data), 201
