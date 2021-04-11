import requests
import json
import time
from bot import still_alert, still_critical_alert, ip_addr, port_num
import datetime

still = 0
count = 0
#start_time = time.time()

while 1:
    #start_time = time.time()
    rm = requests.get(
        url="http://{0}:{1}/movement/still".format(ip_addr, port_num))
    list_mov = rm.json()

    for row in list_mov:
        if(row['movement']):
            still = 0
            count = 0
        else:
            count += 1

    if(count == 15000):
        still = 1
        print("sem movimentos detectados em 5 minutos")
        msg = still_alert()
        formatted_date = datetime.datetime.fromtimestamp(
            msg.date-10800).strftime('%c')
        print("tempo de detecção: {0}".format(formatted_date))
    elif(count >= 60000):
        print("sem movimentos detectados há mais de 20 minutos!")
        msg = still_critical_alert()
        formatted_date = datetime.datetime.fromtimestamp(
            msg.date-10800).strftime('%c')
        print("tempo de detecção: {0}".format(formatted_date))
        time.sleep(50)
    #print("still: {0}".format(count))

    #print("--- %s seconds ---" % (time.time() - start_time))
    time.sleep(10)
