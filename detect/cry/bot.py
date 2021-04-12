import os
import telebot
import urllib
import json
import requests
import time

bot = telebot.TeleBot("1727576179:AAE6t-psMqxv3uNd9_yP62JbG-qLWCykKpg")
chat_id = -555998562
#ip_addr = "192.168.0.17"
ip_addr = "35.199.72.74"
port_num = 54322


def cry_alert():
    message = bot.send_message(
        chat_id, "Choro detectado! - {0}".format(time.ctime(time.time())))
    return message


def moan_alert():
    message = bot.send_message(
        chat_id, "Possível gemido detectado! - {0}".format(time.ctime(time.time())))
    return message


bot.polling()
