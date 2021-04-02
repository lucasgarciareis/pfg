import requests
import json
import time
from bot import still_alert, still_critical_alert, ip_addr, port_num

still = 0
count = 0
#start_time = time.time()

while 1:
    #start_time = time.time()
    rm = requests.get(
        url="http://{0}:{1}/movement/still".format(ip_addr, port_num))
    list_mov = rm.json()

    for row in list_mov:
        if(row['movement'] == 1):
            still = 0
            count = 0
        else:
            count += 1

    if(count == 1800):
        still = 1
        print("sem movimentos detectados em 30 segundos")
        still_alert()
    elif(count >= 3600):
        print("sem movimentos detectados há mais de 1 minuto!")
        still_critical_alert()
        time.sleep(50)
    #print("still: {0}".format(count))

    #print("--- %s seconds ---" % (time.time() - start_time))
    time.sleep(10)
