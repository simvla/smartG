# smartG
"SmartG" stands for smart gardening. This code was created to monitor data from my greenhouse and control temperature in my plant grower. <br>
- ESP32 microcontroller(ESP32 DEV KIT boards 30 or 38) <br>
- 2 DS18B20 sensors <br>
- DHT22 sensor <br>
- Android phone(Termux app installed, run PHP server, display UI)
When everything is set-up, looks like on image: <br>
<img src="https://github.com/user-attachments/assets/f504f468-7d26-4bdf-af86-698e9a021954" width="200" height="340" /> <br>

Main feature of this project is simple custom graphic user interface(cgui). Mcu is free of heavy lifting(js app code, chart drawing, svg ui file). There is only basic html template and some data handling code(smart_server.ino), that means the same firmware can be run on a less powerful mcu. When connect with client and enter ESP32 server IP, only primitive UI are shown: <br><img src="https://github.com/user-attachments/assets/611a2f91-7da8-463d-a892-2412c006eda2" width="200" height="169" /> <br> 
Other files(cguijs.js, index.php, svg.txt, chart.min.js) resides on client(mobile phone or pc), and to work properly(display custom UI) a some server must be established(light php built in server, python http server...), this project is tested with php built-in server. Server will load all required UI data to be displayed on the browser and comunicate with mcu in background. <br> <br>
To draw custom user interface open cguiapp.html on pc browser: <br>
![cguiapp](https://github.com/user-attachments/assets/09ee23f4-1a4f-42aa-a7d6-b2f60c710afa) <br>
Copy generated text to svg.txt file. For now I will not explain cguiapp.html in detail, but feel free to experiment. <br>
[Code](https://github.com/simvla/smartG)
...




