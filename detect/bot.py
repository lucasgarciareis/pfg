# coding=utf-8
import os
import telebot
import urllib
import json

bot = telebot.TeleBot("1727576179:AAE6t-psMqxv3uNd9_yP62JbG-qLWCykKpg")


@bot.message_handler(commands=['start', 'help'])
def send_start_message(message):
    bot.reply_to(message, "Olá, eu sou o Bot monitor de bebê.")

bot.polling()
