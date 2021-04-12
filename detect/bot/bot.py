import os
import telebot
import urllib
import json
import requests

bot = telebot.TeleBot("1727576179:AAE6t-psMqxv3uNd9_yP62JbG-qLWCykKpg")
chat_id = -555998562
#ip_addr = "192.168.0.17"
ip_addr = "35.199.72.74"
port_num = 54322


@bot.message_handler(commands=['start', 'help'])
def send_start_message(message):
    #    bot.send_message(-555998562,"PFG atrasado !")
    bot.reply_to(message, "Olá, eu sou o Bot monitor de bebê.")


@bot.message_handler(commands=['environment', 'env'])
def send_environment_message(message):
    rt = requests.get(
        url="http://{0}:{1}/temperature/now".format(ip_addr, port_num))
    rh = requests.get(
        url="http://{0}:{1}/humidity/now".format(ip_addr, port_num))
    rl = requests.get(url="http://{0}:{1}/light/now".format(ip_addr, port_num))

    rt_data = rt.json()
    rh_data = rh.json()
    rl_data = rl.json()

    msg = "Temperatura atual: {0} °C\nUmidade atual: {1}%\nLuminosidade atual: {2} mlx".format(
        round(rt_data[0]['temperature']/1000, 1), rh_data[0]['humidity'], rl_data[0]['light'])

    bot.reply_to(message, msg)


bot.polling()
