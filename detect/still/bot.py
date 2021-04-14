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


def still_alert():
    message = bot.send_message(
        chat_id, "Sem movimentos detectados nos últimos 10 minutos. Verifique o bebê. - {0}".format(time.ctime(time.time())))
    return message


def still_critical_alert():
    message = bot.send_message(
        chat_id, "Sem movimentos detectados há mais de 20 minutos! Verifique o bebê! - {0}".format(time.ctime(time.time())))
    return message


def agitation_alert():
    message = bot.send_message(
        chat_id, "Agitação detectada. Movimentos prolongados por pelo menos 5 segundos. - {0}".format(time.ctime(time.time())))
    return message


def agitation_critical_alert():
    message = bot.send_message(
        chat_id, "Movimentação excessiva por mais de 10 segundos. - {0}".format(time.ctime(time.time())))
    return message


bot.polling()
