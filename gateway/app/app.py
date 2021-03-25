import os
import pika
from flask import Flask, jsonify, request

app = Flask(__name__)

# we define the route /


@app.route('/sound', methods=['POST'])
def post_sound():
    data = request.get_json()
    lista = request.json['data']
    connection = pika.BlockingConnection(pika.ConnectionParameters('broker'))
    channel = connection.channel()

    for sound in lista:
        channel.basic_publish(exchange='',
                              routing_key='sound',
                              body=sound)
        #print(" [x] Sent 'Hello World!'")

    connection.close()


if __name__ == '__main__':
    # define the localhost ip and the port that is going to be used
    # in some future article, we are going to use an env variable instead a hardcoded port
    app.run(host='0.0.0.0', port='5000')
