# smartG
"SmartG" stands for smart gardening. This code was created to monitor data from my greenhouse and control temperature in my plant grower. <br>
I use ESP32 for development mostly, because of wifi, but it's also a powerful microcontroller no doubt, also is cheap, for example ESP32 DEV KIT boards 30 or 38 pins from china. <br>
When everything is set-up, looks like on image: <br>
 ![T2UIR](https://github.com/user-attachments/assets/26be5429-f8fd-4494-8ffa-be1cd4b633c0) <br>
Apparently the screenshot was taken from a mobile phone. Yes, that's right, there is no hardware other than the esp32 dev board and the sensors. The display is actually a mobile phone or PC browser. <br> <br>
Main feature of this project is simple custom graphic user interface(cgui). Mcu is free of heavy lifting(js app code, chart drawing, svg ui file). There is only basic html template and some data handling code(smart_server.ino), that means the same firmware can be run on a less powerful mcu. When connect with client and enter ESP32 server IP, only primitive UI are shown. <br>
Other files(cguijs.js, index.php, svg.txt, chart.min.js) resides on client(mobile phone or pc), and to work properly(display custom UI) a some server must be established(light php built in server, python http server...(this project is tested with php built-in server). Server will load all required UI data to be displayed on the browser and comunicate with mcu in bacground.



