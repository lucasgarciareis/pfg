import requests
import json
import time
from bot import cry_alert, ip_addr, port_num

threshold = 1600  # minimum value for crying detection
count = 0  # amount of instances where threshold was surpassed
max = 3  # maximum amount of  instances of threshold surpassed
noise = 0  # flag for noise
cry = 0  # flag for crying when there's noise AND movement


while 1:
    #start_time = time.time()
    rs = requests.get(url="http://{0}:{1}/sound".format(ip_addr, port_num))
    list_sound = rs.json()

    for row in list_sound:
        if(row['sound'] > threshold):
            count += 1

        if(count >= max):
            noise = 1

    if(noise == 1):
        print("barulho detectado\n")
        print("pontos acima do threshold = {0}".format(count))
        noise = 0
        count = 0
        rm = requests.get(
            url="http://{0}:{1}/movement".format(ip_addr, port_num))
        list_mov = rm.json()
        for row in list_mov:
            if(row['movement'] == 1):
                print("choro detectado")
                cry_alert()
                time.sleep(30)
                cry = 1
                break
    #print("--- %s seconds ---" % (time.time() - start_time))
    time.sleep(1.98)
