import os
import pika
from flask import Flask, jsonify, request
from flask_api import FlaskAPI, status

app = Flask(__name__)

# we define the route /


@app.route('/esp32', methods=['POST'])
def post_sound():
    connection = pika.BlockingConnection(pika.ConnectionParameters('rabbitmq'))
    channel = connection.channel()

    channel.exchange_declare(exchange="main_ex", exchange_type="direct")

    channel.basic_publish(exchange='main_ex',
                          routing_key='esp32',
                          body=request.data)

    channel.close()
    return request.data, status.HTTP_201_CREATED


@app.route('/xdk', methods=['POST'])
def post_xdk():
    connection = pika.BlockingConnection(pika.ConnectionParameters('rabbitmq'))
    channel = connection.channel()

    channel.exchange_declare(exchange="main_ex", exchange_type="direct")

    channel.basic_publish(exchange='main_ex',
                          routing_key='xdk',
                          body=request.data)

    channel.close()
    return request.data, status.HTTP_201_CREATED


if __name__ == '__main__':
    # define the localhost ip and the port that is going to be used
    # in some future article, we are going to use an env variable instead a hardcoded port
    app.run(host='0.0.0.0', port='5000')
