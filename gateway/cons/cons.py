import pika
import sys
import os
import requests


def main():
    connection = pika.BlockingConnection(pika.ConnectionParameters('rabbitmq'))
    channel = connection.channel()

    channel.exchange_declare(exchange="sound_ex", exchange_type="fanout")

    result = channel.queue_declare(queue='', exclusive=True)

    queue_name = result.method.queue

    def callback(ch, method, properties, body):
        print(" [x] Received %r" % body)
        requests.post("http://34.95.136.144:54322/sound", data=body.decode(),
                      headers={"Content-Type": "application/json"})

    channel.queue_bind(exchange='sound_ex', queue=queue_name)

    channel.basic_consume(
        queue=queue_name, on_message_callback=callback, auto_ack=True)

    channel.start_consuming()
    print("Program Started. Consuming...")


if __name__ == '__main__':
    try:
        main()
    except KeyboardInterrupt:
        print('Interrupted')
        try:
            sys.exit(0)
        except SystemExit:
            os._exit(0)
