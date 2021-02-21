import requests
import json
from matplotlib import pyplot as plt

# api-endpoint
URL = "http://0.0.0.0:54322/sound"

# sending get request and saving the response as response object
r = requests.get(url=URL)

# extracting data in json format
data = r.json()

eixoX = []
eixoY = []
x = 0

for i in data:
    eixoY.append(i['sound'])

for j in range(len(eixoY)):
    eixoX.append(x)
    x += 0.016

# Eixo_x, Eixo_y
plt.plot(eixoX, eixoY)
plt.ylabel('Amostras de Som')
plt.xlabel('Segundos')
plt.show()
