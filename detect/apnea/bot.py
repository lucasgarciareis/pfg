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


def apnea_alert():
    message = bot.send_message(
        chat_id, "Respiração anormal detectada. Verifique o bebê e o posicionamento do sensor. - {0}".format(time.ctime(time.time())))
    return message


def apnea_critical_alert():
    message = bot.send_message(
        chat_id, "Respiração anormal há mais de 6 segundos! - {0}".format(time.ctime(time.time())))
    return message
