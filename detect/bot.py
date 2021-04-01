# coding=utf-8
import os
import telebot
import urllib
import json
import requests

bot = telebot.TeleBot("1727576179:AAE6t-psMqxv3uNd9_yP62JbG-qLWCykKpg")


@bot.message_handler(commands=['start', 'help'])
def send_start_message(message):
    bot.reply_to(message, "Olá, eu sou o Bot monitor de bebê.")


@bot.message_handler(commands=['environment', 'env'])
def send_environment_message(message):
    rs = requests.get(url="http://192.168.1.7:54322/temperature/now")
    bot.reply_to(message, "A última medição de temperatura retornou {0}°C".format(
        rs['temperature']))


bot.polling()
