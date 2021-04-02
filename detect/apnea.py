import requests
import json
import time
from bot import apnea_alert, apnea_critical_alert

threshold = 13  # minimum value for apnea detection
count = 0  # amount of instances where threshold was surpassed
max = 180  # maximum amount of instances where value surpassed the threshold
apnea = 0  # flag for apnea

while 1:
    start_time = time.time()
    rp = requests.get(url="http://192.168.1.7:54322/pressure")
    list_press = rp.json()

    for row in list_press:
        if(row['pressure'] < threshold):
            count += 1
        else:
            if(apnea == 1):
                print("respiração detectada novamente.")
                apnea = 0
            count = 0

        if(count >= max):
            apnea = 1
            if(count == max):
                print("Apneia detectada. Sem dados respiratorios por 3 segundos")
                apnea_alert()
            elif(count >= 2*max):
                print("Apneia prolongada por mais de 6 segundos! Verifique o bebê!")
                apnea_critical_alert()
                time.sleep(60)

    #print("--- %s seconds ---" % (time.time() - start_time))
    time.sleep(2.98)
