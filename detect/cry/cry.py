import requests
import json
import time
from bot import cry_alert, moan_alert, high_cry_alert, scream_alert, ip_addr, port_num
import datetime

max_threshold = 1600  # minimum value for crying detection
min_threshold = 1200
scream_max_threshold = 2000
scream_min_threshold = 800
scream_count = 0
screams = 0
scream = 0
max_scream = 3
time_window_start = 0
time_window = 0
count = 0  # amount of instances where threshold was surpassed
cry_count = 0
max_moan = 6  # maximum amount of  instances of threshold surpassed for moaning
max_cry = 40
moan = 0  # flag for moan
cry = 0  # flag for crying when there's noise AND movement


while 1:
    #start_time = time.time()
    rs = requests.get(url="http://{0}:{1}/sound".format(ip_addr, port_num))
    list_sound = rs.json()

    for row in list_sound:
        if(row['sound'] > max_threshold or row['sound'] < min_threshold):
            count += 1
            if(row['sound'] > scream_max_threshold or row['sound'] < scream_min_threshold):
                scream_count += 1

    if(count >= max_moan and count < max_cry and scream_count >= max_scream):  # evento de grito
        scream = 1

    elif(count >= max_moan and count < max_cry and scream_count < max_scream):
        moan = 1

    elif(count >= max_cry):
        cry = 1

    if(moan == 1):
        print("Pouco barulho detectado\n")
        print("pontos acima do threshold = {0}".format(count))
        rm = requests.get(
            url="http://{0}:{1}/movement".format(ip_addr, port_num))
        list_mov = rm.json()
        for row in list_mov:
            if(row['movement'] == 1):
                print("gemido detectado")
                msg = moan_alert()
                formatted_date = datetime.datetime.fromtimestamp(
                    msg.date-10800).strftime('%c')
                print("tempo de detecção: {0}".format(formatted_date))
                moan = 0
                count = 0
                time.sleep(5)
                break

    if(scream == 1):
        print("Pico alto de barulho detectado\n")
        #print("pontos acima do threshold = {0}".format(count))
        rm = requests.get(
            url="http://{0}:{1}/movement".format(ip_addr, port_num))
        list_mov = rm.json()
        for row in list_mov:
            if(row['movement'] == 1):
                print("grito detectado")
                if(scream_count == 0):
                    msg = scream_alert()
                    formatted_date = datetime.datetime.fromtimestamp(
                        msg.date-10800).strftime('%c')
                    print("tempo de detecção: {0}".format(formatted_date))
                    moan = 0
                    count = 0
                    scream = 0
                    scream_count = 1
                    time_window_start = time.time()  # inicia a contagem dos 10 segundos
                    break
                elif(scream_count == 1):
                    time_window = time.time() - time_window_start
                    if(time_window < 11):
                        scream_count += 1
                    else:
                        msg = scream_alert()
                        formatted_date = datetime.datetime.fromtimestamp(
                            msg.date-10800).strftime('%c')
                        print("tempo de detecção: {0}".format(formatted_date))
                        moan = 0
                        count = 0
                        scream = 0
                        scream_count = 1
                        time_window_start = time.time()  # inicia a contagem dos 10 segundos
                        break
                elif(scream_count >= 2):
                    time_window = time.time() - time_window_start
                    if(time_window < 11):
                        msg = high_cry_alert()
                        formatted_date = datetime.datetime.fromtimestamp(
                            msg.date-10800).strftime('%c')
                        print("tempo de detecção: {0}".format(formatted_date))
                        moan = 0
                        count = 0
                        scream = 0
                        scream_count = 0
                        time.sleep(5)
                        break
                    else:
                        msg = scream_alert()
                        formatted_date = datetime.datetime.fromtimestamp(
                            msg.date-10800).strftime('%c')
                        print("tempo de detecção: {0}".format(formatted_date))
                        moan = 0
                        count = 0
                        scream = 0
                        scream_count = 1
                        time_window_start = time.time()  # inicia a contagem dos 10 segundos
                        break

    if(cry == 1):
        print("Muito barulho detectado\n")
        print("pontos acima do threshold = {0}".format(count))
        rm = requests.get(
            url="http://{0}:{1}/movement".format(ip_addr, port_num))
        list_mov = rm.json()
        for row in list_mov:
            if(row['movement'] == 1):
                print("choro detectado")
                msg = cry_alert()
                formatted_date = datetime.datetime.fromtimestamp(
                    msg.date-10800).strftime('%c')
                print("tempo de detecção: {0}".format(formatted_date))
                cry = 0
                count = 0
                time.sleep(20)
                break
    #print("--- %s seconds ---" % (time.time() - start_time))
    time.sleep(2)
