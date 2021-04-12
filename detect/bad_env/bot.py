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


def high_temp_alert():
    message = bot.send_message(
        chat_id, "Temperatura do quarto acima do ideal para o bebê. - {0}".format(time.ctime(time.time())))
    return message


def low_temp_alert():
    message = bot.send_message(
        chat_id, "Temperatura do quarto abaixo do ideal para o bebê. - {0}".format(time.ctime(time.time())))
    return message


def low_humidity_alert():
    message = bot.send_message(
        chat_id, "Umidade do ar abaixo do ideal para o bebê. - {0}".format(time.ctime(time.time())))
    return message


def high_luminosity_alert():
    message = bot.send_message(
        chat_id, "Quarto com claridade! - {0}".format(time.ctime(time.time())))
    return message
