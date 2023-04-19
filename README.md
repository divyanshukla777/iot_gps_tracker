IOT VEHICLE TRACKING PROJECT
Description: This is an IOT based GPS tracking device to track the vehicles in the real time.

Hardware Used: SIM808 GPRS module,Arduino Uno, SIM CARD(2g compatible), 12V adapter and jumper wires.

Software Used: Arduino IDE, ThingSpeak dashboard, google map api.

How to run this project:
1. Connect the SIM808 module and Arduino UNO through jumper wires.
2. Insert the sim card inside the gprs module and supply the power to gprs module through 12v adapter.
3. Make a new account on the ThingSpeak dashboard and get GET API from there.
4. Download the sketch file and make following changes in it: mobile number, APN, ThingSpeak API.
5. Run the sketch on the Arduino IDE and Upload the sketch to arduino.
6. You will get a call on the mobile number you have given the sketch, which will be autodisconnected. 
7. Next, You will receive a google maps api link as message on the same mobile number.
8. Now look for lattitude and longitude values on the ThingSpeak dashboard.
9. Get the google maps api key.
10. Download the webpage file. Change the read api of ThingSpeak and google maps api.
11. Run the webpage on the browser. You will able to see the exact location of module on the maps and this value gets updated every 10s.

For the futher details refer to PPT. 
