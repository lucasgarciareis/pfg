from flask import Blueprint, current_app, request, jsonify
from .model import Vibration
from .serializer import VibrationSchema

bp_vibration = Blueprint('vibration', __name__)


@bp_vibration.route('/vibration', methods=['GET'])
def getvibr():
    ss = VibrationSchema(many=True)
    result = Vibration.query.all()
    return ss.jsonify(result), 200


@bp_vibration.route('/vibration', methods=['POST'])
def postvibr():

    data = request.get_json()

    vibration = request.json['vibration']

    new_vibration = Vibration(vibration)

    current_app.db.session.add(new_vibration)

    current_app.db.session.commit()

    return jsonify(data), 201
