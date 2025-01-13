# smartG
"SmartG" stands for smart gardening. This code was created to monitor data from my greenhouse and control temperature in my plant grower. <br>
- ESP32 microcontroller(ESP32 DEV KIT boards 30 or 38 pins) <br>
- 2 DS18B20 sensors <br>
- DHT22 sensor <br>
- Android phone(older is better, Android10, 11) <br> <br>
When everything is set-up, looks like on image: <br>
<img src="https://github.com/user-attachments/assets/f504f468-7d26-4bdf-af86-698e9a021954" width="200" height="340" /> <br>

1. Test connection <br>
Open smartG_server.ino in ArduinoIDE, enter Android phone SSID and Pass or leave Pass empty. Compile code(ESPAsyncWebServer, AsyncTCP, DallasTemperature, Adafruit_Unified_Sensor, DHT_sensor, OneWire lib's must be installed), write code to ESP32. When ESP32 connected to Android Access point, enter ESP32 IP in phone browser, the basic UI will be displayed: <br><img src="https://github.com/user-attachments/assets/611a2f91-7da8-463d-a892-2412c006eda2" width="200" height="169" /> <br> 
2. Set User interface <br>
  To draw custom user interface open cguiapp.html in web browser: <br><br>
![cguiapp](https://github.com/user-attachments/assets/09ee23f4-1a4f-42aa-a7d6-b2f60c710afa) <br>
Create svg.txt file and copy generated text. Feel free to experiment. <br>
3. Set phone <br>
  -Install Termux app (download 0.118 apk or newer) <br>
  -Open Termux, type: apt update, apt install php <br>
  -Copy index.php, cguijs.js, Chart.min.js, svg.txt to /storage/Pictures or other folder visible from termux. <br>
  -In termux type: php -S localhost:8000 <br>
4. Test app <br>
  -Make sure ESP32 is connected to Aandroid access point <br>
  -Open phone's browser, in address bar enter: localhost:8000 <br>
5. Automate with Termux:Widget and startup script

   
   

[Code](https://github.com/simvla/smartG)




