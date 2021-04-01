import requests
import json

threshold = 0.05	#minimum value for crying detection
count = 0	#amount of instances where threshold was surpassed
max = 3		#maximum amount of  instances of threshold surpassed
noise = 0		#flag for noise
movement = 0		#flag for movement

# while 1:
r = requests.get(url="http://192.168.0.17:54322/sound")
list = r.json()

for row in list:
	if(row['sound'] > threshold):
		count+=1	
	
	if(count >= max): 
		noise = 1

if(noise==1):
	print("telegram avisa que chorou\n")
	print("count = {0}".format(count))
	noise = 0 
	count = 0	
