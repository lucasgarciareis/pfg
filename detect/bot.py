import os
import telebot
import urllib
import json
import requests

bot = telebot.TeleBot("1727576179:AAE6t-psMqxv3uNd9_yP62JbG-qLWCykKpg")
chat_id = -555998562

@bot.message_handler(commands=['start', 'help'])
def send_start_message(message):
#    bot.send_message(-555998562,"PFG atrasado !")
    bot.reply_to(message, "Olá, eu sou o Bot monitor de bebê. Caguei aqui!")


@bot.message_handler(commands=['environment', 'env'])
def send_environment_message(message):
    rt = requests.get(url="http://192.168.1.7:54322/temperature/now")
    rh = requests.get(url="http://192.168.1.7:54322/humidity/now")
    rl = requests.get(url="http://192.168.1.7:54322/light/now")

    rt_data = rt.json()
    rh_data = rh.json()
    rl_data = rl.json()

    msg = "Temperatura atual: {0} °C\nHumidade atual: {1}%\nLuminosidade atual: {2} mlx".format(rt_data[0]['temperature'], rh_data[0]['humidity'], rl_data[0]['light'])

    bot.reply_to(message, msg)

def crying():
    bot.send_message(chat_id,"Choro detectado!")

bot.polling()
