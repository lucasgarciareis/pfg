import pika
import sys
import os
import requests

ip_addr = "35.199.72.74"
port_num = "54322"


def main():
    connection = pika.BlockingConnection(pika.ConnectionParameters('rabbitmq'))
    channel = connection.channel()

    channel.exchange_declare(exchange="main_ex", exchange_type="direct")

    result = channel.queue_declare(queue='sound', exclusive=True)

    queue_name = result.method.queue

    def callback_sound(ch, method, properties, body):
        print(" [x] Received %r" % body)
        requests.post("http://{0}:{1}/sound".format(ip_addr, port_num), data=body.decode(),
                      headers={"Content-Type": "application/json"})

    channel.queue_bind(exchange='main_ex', queue=queue_name,
                       routing_key='sound')

    channel.basic_consume(
        queue=queue_name, on_message_callback=callback_sound, auto_ack=True)

    result2 = channel.queue_declare(queue='xdk', exclusive=True)

    queue_name2 = result2.method.queue

    def callback_xdk(ch, method, properties, body):
        print(" [x] Received %r" % body)
        requests.post("http://{0}:{1}/xdk".format(ip_addr, port_num), data=body.decode(),
                      headers={"Content-Type": "application/json"})

    channel.queue_bind(exchange='main_ex',
                       queue=queue_name2, routing_key='xdk')

    channel.basic_consume(
        queue=queue_name2, on_message_callback=callback_xdk, auto_ack=True)

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
