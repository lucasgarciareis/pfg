from flask import Blueprint, current_app, request, jsonify
from .model import Vibration
from .serializer import VibrationSchema
from time import time
from datetime import datetime, timedelta

bp_vibration = Blueprint('vibration', __name__)


@bp_vibration.route('/vibration', methods=['GET'])
def getvibr():
    ss = VibrationSchema(many=True)
    result = Vibration.query.order_by(Vibration.id.desc()).limit(120)
    return ss.jsonify(result), 200


@bp_vibration.route('/vibration/inactivity', methods=['GET'])
def getinactivity():
    ss = VibrationSchema(many=True)
    result = Vibration.query.order_by(Vibration.id.desc()).limit(1800)
    return ss.jsonify(result), 200


@bp_vibration.route('/vibration', methods=['POST'])
def postvibr():

    data = request.get_json()

    vibration = request.json['vibration']

    received_time = time()

    new_vibration = Vibration(vibration, received_time)

    current_app.db.session.add(new_vibration)

    current_app.db.session.commit()

    return jsonify(data), 201
