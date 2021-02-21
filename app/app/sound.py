from flask import Blueprint, current_app, request, jsonify
from .model import Sound
from .serializer import SoundSchema

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

    for sound in lista:
        new_sound = Sound(sound)
        current_app.db.session.add(new_sound)
        current_app.db.session.commit()

    return jsonify(data), 201
