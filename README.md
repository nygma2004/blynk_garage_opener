# Blynk Garage Door Opener
Garage opener / temperature monitor in Blynk

This is my first Blynk project and it combines a few tricks I learned about Blynk:
1) Momentary relay outputs to operate the garage and gate doors
2) Reading sensors (DHT11 or DHT22) and send them across as virtual pins
3) Read virtual pin status and execute local logic on the ESP
4) Sharing project to 3rd parties

The project is built for the following purpose
1) Operate the garage and gate door remotely via relays connected to the ESP
2) Read a DHT11 or DHT22 temperature and humidity sensor and make those readings available in the Blynk app
3) Create a times accessory socket which turns off automatically after a preset time after being turned on: this is to be used for charging devices and turning off the charger after let's say 6 hours
4) Use the last relay (out of a 4 relay board) as a general purpose accessory socket.

You can watch every detail of this project here: https://youtu.be/IBrc2v9lNm0
