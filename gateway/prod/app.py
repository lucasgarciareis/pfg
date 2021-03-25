import os
import pika
from flask import Flask, jsonify, request

app = Flask(__name__)

# we define the route /


@app.route('/sound', methods=['POST'])
def post_sound():
    connection = pika.BlockingConnection(pika.ConnectionParameters('broker'))
    channel = connection.channel()

    channel.exchange_declare(exchange="sound_ex", exchange_type="fanout")

    channel.basic_publish(exchange='sound_ex',
                          routing_key='sound',
                          body=request.data)

    connection.close()
    return request.data, status.HTTP_201_CREATED


if __name__ == '__main__':
    # define the localhost ip and the port that is going to be used
    # in some future article, we are going to use an env variable instead a hardcoded port
    app.run(host='0.0.0.0', port='5000')
