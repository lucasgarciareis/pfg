import requests
import json
from time import sleep

threshold = 460  # minimum value for crying detection
count = 0  # amount of instances where threshold was surpassed
max = 3  # maximum amount of  instances of threshold surpassed
noise = 0  # flag for noise
cry = 0  # flag for crying when there's noise AND movement

while 1:
    r = requests.get(url="http://192.168.0.17:54322/sound")
    list = r.json()

    for row in list:
        if(row['sound'] > threshold):
            count += 1

        if(count >= max):
            noise = 1

    if(noise == 1):
        print("barulho detectado\n")
        print("pontos acima do threshold = {0}".format(count))
        noise = 0
        count = 0
        r = requests.get(url="http://192.168.0.17:54322/movement")
        list = r.json()
        for row in list:
            if(row['movement'] == 1):
                cry = 1
                break

    sleep(2)
