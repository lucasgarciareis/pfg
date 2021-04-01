import requests
import json

# while 1:
r = requests.get(url="http://192.168.0.17:54322/sound")
data = r.json()
data_dict = json.loads(data)

print(data_dict)
