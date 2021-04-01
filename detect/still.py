import requests
import json
import time

still = 0
count = 0
#start_time = time.time()

while 1:
    #start_time = time.time()
    rm = requests.get(url="http://192.168.0.17:54322/movement/still")
    list_mov = rm.json()

    for row in list_mov:
        if(row['movement'] == 1):
            still = 0
            count = 0
        else:
            count += 1

    if(count >= 1800):
        still = 1
        print("sem movimentos detectados em 30 segundos")
        # codigo de alerta
    #print("still: {0}".format(count))

    #print("--- %s seconds ---" % (time.time() - start_time))
    time.sleep(10)
