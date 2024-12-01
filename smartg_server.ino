// esp32 gui server by simvla
//  using espAsyncWebServer lib 

#include <Arduino.h>
#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <FS.h>
#include "SPIFFS.h"

#define ONE_WIRE_PIN 4
#define DHT_PIN 16
#define RELAY 17
#define RELAY1 15
#define HIST 9 //(0.9C)

#define NUM_OF_ELEM 192

int ndevices=0;
DeviceAddress addr;
int dstemp [2]; //ds readings stored as int
OneWire onewire(ONE_WIRE_PIN);
DallasTemperature sensors(&onewire);

int dhtemp[2]; //dht readings stored as int
DHT dht(DHT_PIN,DHT22);

int toff=0;
int t3off=0;

uint16_t data[NUM_OF_ELEM]={0}; //log data array
int d_idx=0; //data index
String dot=".";
String space=" ";

unsigned long startTime=0;
uint8_t firstRun=1; //true

void readDS(){
  sensors.requestTemperatures();
  for(int i=0;i<ndevices;i++){
    if(sensors.getAddress(addr,i)){
      float temp=sensors.getTempC(addr);
      dstemp[i]=temp*10; //store reading float 23.3 as int 233 
    }
  }
}

/*void printAddress(DeviceAddress deviceAddress){
  for(uint8_t i=0;i<8;i++){
    if(deviceAddress[i]<16)Serial.print("0");
    Serial.print(deviceAddress[i],HEX);
  }
}*/

void readDHT(){
  float temp=dht.readTemperature();
  float humi=dht.readHumidity();
  dhtemp[0]=temp*10;
  dhtemp[1]=humi*10;
  if(dhtemp[0]>999){dhtemp[0]=10;dhtemp[1]=10;} // no sensor, return 0.0
}

//write readings to data array, format and limit n inputs
void formatData(){
//write first 48x4 values {233,180,121,55}
  if(d_idx<NUM_OF_ELEM){
    for(int i=0;i<2;i++){
      data[d_idx]=dstemp[i];
      d_idx++;
    }
    for(int i=0;i<2;i++){
        data[d_idx]=dhtemp[i];
        d_idx++;
    }
    File f=SPIFFS.open("/data.txt","w");
    //write file "23.3 18.0 12.1 5.5"
    if(f){//debug
    uint8_t n=1;
    for(int i=0;i<d_idx/4;i++){
      f.print(
        String(
          (data[(4*n-4)]/10)+dot+(data[(4*n-4)]-((data[(4*n-4)]/10)*10))+space+
          (data[(4*n-3)]/10)+dot+(data[(4*n-3)]-((data[(4*n-3)]/10)*10))+space+
          (data[(4*n-2)]/10)+dot+(data[(4*n-2)]-((data[(4*n-2)]/10)*10))+space+
          (data[(4*n-1)]/10)+dot+(data[(4*n-1)]-((data[(4*n-1)]/10)*10))+"\n"
        )
      );
      n++;
    }
    f.close();
    }else Serial.println("error w");
    
  }else{
    //shift data 4 places left, write new group of 4
    d_idx=4;
    for(int i=0;i<NUM_OF_ELEM-4;i++){
      data[i]=data[d_idx];
      d_idx++; 
    }
    d_idx=NUM_OF_ELEM-4; //188
    for(int i=0;i<2;i++){
      data[d_idx]=dstemp[i];
      d_idx++;
    }
    for(int i=0;i<2;i++){
        data[d_idx]=dhtemp[i];
        d_idx++;
    }
    //"w" overwrite "a" append 
    File f=SPIFFS.open("/data.txt","w");
    //write file
    d_idx=0;
    if(f){//debug
    for(int i=0;i<48;i++){
      f.println(
        String(
          //23(233/10)      .    3(233   -   230(23(233/10)*10))
          (data[d_idx]/10)+dot+(data[d_idx]-((data[d_idx]/10)*10))+space+
          (data[d_idx+1]/10)+dot+(data[d_idx+1]-((data[d_idx+1]/10)*10))+space+
          (data[d_idx+2]/10)+dot+(data[d_idx+2]-((data[d_idx+2]/10)*10))+space+
          (data[d_idx+3]/10)+dot+(data[d_idx+3]-((data[d_idx+3]/10)*10))
        )
      );
      d_idx+=4;
    }
    f.close();
    }else Serial.println("error w"); 
  }
  //debug
  delay(1000);
  File f=SPIFFS.open("/data.txt","r");
  if(f){ 
    Serial.print(f.readString());
    f.close();
  }else Serial.println("error r");
}

void controlOUT(){
  if(toff<=HIST) digitalWrite(RELAY,LOW); //lock
  else{
    if(dstemp[0]<toff-HIST){digitalWrite(RELAY,HIGH);}
    if(dstemp[0]>toff+HIST){digitalWrite(RELAY,LOW);}
  }
  if(t3off<=HIST) digitalWrite(RELAY1,LOW);
  else{ 
    if(dhtemp[0]<t3off-HIST){digitalWrite(RELAY1,HIGH);}
    if(dhtemp[0]>t3off+HIST){digitalWrite(RELAY1,LOW);}
  }
}

AsyncWebServer server(80);

const char* ssid = "Redmi9A";
const char* password = "";

const char* PAR1="toff";
const char* PAR2="t3off";

String value; // conversion 

IPAddress local_IP(192,168,43,138);
IPAddress gateway(192,168,43,1);
IPAddress subnet(255,255,255,0);

void notFound(AsyncWebServerRequest *request) {
    request->send(404, "text/plain", "Not found");
}

const char* html ="<!DOCTYPE HTML><html> \
  <head> <meta name=\"viewport\" content=\"width=device-width, initial-scale=1\"></head><body> \
  <h2>ESP GUI Server</h2> \
  <div id=\"svg\"></div> \
  <div>%T1%</div> \
  <div>%T2%</div> \
  <div>%T3%</div> \
  <div>%H1%</div> \
  <form method=\"GET\" action=\"/par\"> \
  <label>T1OFF:</label> \
  <input name=\"toff\" style=\"width:10%%\" value=\"%T1OFF%\"> <BR> \
  <label>T3OFF:</label> \
	<input name=\"t3off\" style=\"width:10%%\" value=\"%T3OFF%\"> <BR> \
  <button type=\"submit\">SET</button></form> \
  <select style=\"margin-top:5px\" id=\"sel\"> \
  <option value=\"1\">T1</option> \
  <option value=\"2\">T2</option> \
  <option value=\"3\">T3</option> \
  <option value=\"4\">H1</option></select> \ 
  <canvas id=\"log\" width=\"400\" height=\"200\"></canvas></body> \
  <script type=\"text/javascript\" src=\"Chart.min.js\"></script> \
  <script src=\"cguijs.js\"></script> \
  </html>";

//replace %T1% etc
String processor(const String& var){
  if(var=="T1") return String((dstemp[0]/10)+dot+(dstemp[0]-((dstemp[0]/10)*10))); 
  if(var=="T2") return String((dstemp[1]/10)+dot+(dstemp[1]-((dstemp[1]/10)*10)));
  if(var=="T3") return String((dhtemp[0]/10)+dot+(dhtemp[0]-((dhtemp[0]/10)*10)));
  if(var=="H1") return String((dhtemp[1]/10)+dot+(dhtemp[1]-((dhtemp[1]/10)*10)));
  if(var=="T1OFF") return String((toff/10)+dot+(toff-((toff/10)*10)));
  if(var=="T3OFF") return String((t3off/10)+dot+(t3off-((t3off/10)*10)));
  return String();
}

void setup() {

    Serial.begin(115200);

    sensors.begin();
    ndevices=sensors.getDeviceCount();
    Serial.print(ndevices,DEC);
    Serial.println();
    delay(1000);

    //test read
    sensors.requestTemperatures();
    /*for(int i=0;i<ndevices;i++){
      if(sensors.getAddress(addr,i)){
        printAddress(addr);
        Serial.println();
      }    
    }*/
    delay(1000);

    dht.begin();

    pinMode(RELAY,OUTPUT);
    digitalWrite(RELAY,LOW);
    pinMode(RELAY1,OUTPUT);
    digitalWrite(RELAY1,LOW);
    pinMode(LED_BUILTIN,OUTPUT);

    if(SPIFFS.begin(true)) Serial.println("spiffs ok");
    else Serial.println("error s");
    //if(SPIFFS.format()) Serial.println("formated");
    //else Serial.println("error f");

    File f=SPIFFS.open("/ctrl.txt","r");
    //Serial.println(f.readString()); //debug
    value=f.readString();  //233\n233 ..
    toff=value.substring(0,value.indexOf('\n')).toInt();
    t3off=value.substring(value.indexOf('\n')+1).toInt();
    Serial.println(toff); Serial.println(t3off);
    f.close();
    //Serial.println(String((t3off/10)+dot+(t3off-((t3off/10)*10)))); //debug

    WiFi.config(local_IP,gateway,subnet);
    
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, password);
    if (WiFi.waitForConnectResult() != WL_CONNECTED) {
        Serial.printf("WiFi Failed!\n");
        //return;
    }
    delay(2000);

  Serial.print("IP Address: ");
    Serial.println(WiFi.localIP());

    server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
        request->send_P(200, "text/html", html, processor);
    });
    
    server.on("/data", HTTP_GET, [](AsyncWebServerRequest *request){
        request->send(SPIFFS, "/data.txt", "text/plain");
    });

    // Send a GET req <IP>/par?toff=value&t3off=value
    server.on("/par", HTTP_GET, [] (AsyncWebServerRequest *request) {
        String value1="";
        String digit,deci;
        if (request->hasParam(PAR1)&&request->hasParam(PAR2)) {
            value = request->getParam(PAR1)->value();
            value1 = request->getParam(PAR2)->value();
            Serial.println(value); Serial.println(value1); //debug
            digit=value.substring(0,value.indexOf('.'));
            deci=value.charAt(value.length()-1);
            toff=digit.toInt()*10 + deci.toInt(); //convert str to int "23.3"=233
            digit=value1.substring(0,value1.indexOf('.'));
            deci=value1.charAt(value1.length()-1);
            t3off=digit.toInt()*10 + deci.toInt();
            File f=SPIFFS.open("/ctrl.txt","w");
            f.println(String(toff));
            f.println(String(t3off));
            f.close();                
        } else {
            value = "0.0"; 
            value1="0.0";
        }
        value+="\n"+value1;
        request->send(200, "text/plain", value);
    });

    server.onNotFound(notFound);

    server.begin();
    
    /*// test
    digit=value.substring(0,value.indexOf('.'));
    deci=value.charAt(value.length()-1);
    toff=digit.toInt()*10 + deci.toInt();
    Serial.println(toff);
    for(int i=0;i<8;i++){
      data[i]=233;
    }*/

}

void loop() {
  /*// test
      Serial.println(toff);
      
      int j=0;
      for(int i=0;i<2;i++){
        Serial.println(
        String(
          //23(233/10)  .    3(233-230(23(233/10)*10))
          (data[j]/10)+dot+(data[j]-((data[j]/10)*10))+space+
          (data[j+1]/10)+dot+(data[j+1]-((data[j+1]/10)*10))+space+
          (data[j+2]/10)+dot+(data[j+2]-((data[j+2]/10)*10))+space+
          (data[j+3]/10)+dot+(data[j+3]-((data[j+3]/10)*10))
        )
      );
      j=4;
    }
    delay(2000);*/
  digitalWrite(LED_BUILTIN,HIGH);
  delay(1000);
  readDS();
  Serial.println(String((dstemp[0]/10)+dot+(dstemp[0]-((dstemp[0]/10)*10))));
  Serial.println(String((dstemp[1]/10)+dot+(dstemp[1]-((dstemp[1]/10)*10))));
  delay(1000);
  digitalWrite(LED_BUILTIN,LOW);
  readDHT();
  Serial.println(String((dhtemp[0]/10)+dot+(dhtemp[0]-((dhtemp[0]/10)*10))));
  Serial.println(String((dhtemp[1]/10)+dot+(dhtemp[1]-((dhtemp[1]/10)*10))));
  delay(2000);
  controlOUT();
  if((millis()-startTime)>3600000 || firstRun){ 
    Serial.println("A"); //debug
    formatData();
    startTime=millis();
    firstRun=0;
  }
}
