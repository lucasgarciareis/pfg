import requests
import json
import time
from bot import high_temp_alert, low_temp_alert, low_humidity_alert, high_luminosity_alert

# temperature
upper_temp_threshold = 25
lower_temp_threshold = 20
count_high_temp = 0  # amount of instances where threshold was surpassed
count_low_temp = 0
max_temp = 20  # maximum amount of  instances of threshold surpassed
high_temp = 0
low_temp = 0

# humidity
humidity_threshold = 40
count_humid = 0
max_humid = 20
low_humid = 0

# lighting
light_threshold = 500
count_light = 0
max_light = 10
high_light = 0

while 1:
    #start_time = time.time()
    # temperature
    rt = requests.get(url="http://192.168.1.7:54322/temperature")
    list_temp = rt.json()

    for row in list_temp:
        if(row['temperature'] > upper_temp_threshold):
            count_high_temp += 1

        elif(row['temperature'] < lower_temp_threshold):
            count_low_temp += 1
        else:
            count_high_temp = count_low_temp = 0

    if(count_high_temp >= max_temp):
        print("temperatura do quarto acima do recomendado")
        high_temp_alert()
        high_temp = 1

    if(count_low_temp <= max_temp):
        print("temperatura do quarto abaixo do recomendado")
        low_temp_alert()
        low_temp = 1

    # humidity
    rh = requests.get(url="http://192.168.1.7:54322/humidity")
    list_humid = rh.json()

    for row in list_humid:
        if(row['humidity'] < humidity_threshold):
            count_humid += 1
        else:
            count_humid = 0

    if(count_humid >= max_humid):
        print("Umidade do ar muito baixa")
        low_humidity_alert()
        low_humid = 1
        # codigo de alerta

    # lighting
    rl = requests.get(url="http://192.168.1.7:54322/light")
    list_light = rl.json()

    for row in list_light:
        if(row['light'] > light_threshold):
            count_light += 1
        else:
            count_light = 0

    if(count_light >= max_light):
        print("alta luminosidade detectada")
        high_luminosity_alert()
        high_light = 1
        # codigo de alerta

    if(high_temp == 1 or low_temp == 1 or low_humid == 1 or high_light == 1):
        time.sleep(300)

    high_temp = low_temp = low_humid = high_light = 0
    #print("--- %s seconds ---" % (time.time() - start_time))
    time.sleep(19.97)
