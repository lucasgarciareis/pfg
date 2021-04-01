import requests
import json
import time

threshold = 13  # minimum value for apnea detection
count = 0  # amount of instances where threshold was surpassed
max = 180  # maximum amount of instances where value surpassed the threshold
apnea = 0  # flag for apnea

while 1:
    start_time = time.time()
    rp = requests.get(url="http://192.168.0.17:54322/pressure")
    list_press = rp.json()

    for row in list_press:
        if(row['pressure'] < threshold):
            count += 1
        else:
            if(apnea=1):
                print("respiração detectada novamente.")
                apnea = 0
            count = 0

        if(count >= max):
            apnea = 1

    if(apnea == 1):
        print("apneia detectada. Sem dados respiratorios por 3 segundos")
        # codigo de alerta
    print("--- %s seconds ---" % (time.time() - start_time))
    time.sleep(2.98)
