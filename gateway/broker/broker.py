import pika
import sys
import os
import requests

url = 'http://34.95.136.144/sound'


def main():
    connection = pika.BlockingConnection(pika.ConnectionParameters('broker'))
    channel = connection.channel()

    channel.exchange_declare(exchange="sound_ex", exchange_type="fanout")

    channel.queue_declare(queue='sound')

    def callback(ch, method, properties, body):
        print(" [x] Received %r" % body)
        #x = requests.post(url, data = myobj)

    channel.queue_bind(exchange='sound_ex', queue='sound')

    channel.basic_consume(
        queue='sound', on_message_callback=callback, auto_ack=True)

    channel.start_consuming()


if __name__ == '__main__':
    try:
        main()
    except KeyboardInterrupt:
        print('Interrupted')
        try:
            sys.exit(0)
        except SystemExit:
            os._exit(0)
