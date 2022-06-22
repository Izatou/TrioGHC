#include <Arduino.h>
#include <Wire.h>
#include <U8g2lib.h>

// Load Wi-Fi library
#include <WiFi.h>
#include <EEPROM.h>

// Replace with your network credentials
const char* ssid = "INOSE-X*";
const char* password = "lekassembuh";

// Set web server port number to 80
WiFiServer server(80);

// Variable to store the HTTP request
String header;

// Current time
unsigned long currentTime = millis();
// Previous time
unsigned long previousTime = 0; 
// Define timeout time in milliseconds (example: 2000ms = 2s)
const long timeoutTime = 2000;
int val;

const int red_ = 23;  // red
const int green_ = 22;  // green
const int blue_ = 21;  // blue  
const int photo = 34;  // in Photdiode


int started = 0;
int tanda=1;
int pwm=40;



// setting PWM properties
const int freq = 5000;

const int red = 0;
const int green = 1;
const int blue = 2;

const int resolution = 8;
int timeku=0;
int flag_=0;

void setup() {
  Serial.begin(115200);
//    WiFi.begin(ssid, password);
//  while (WiFi.status() != WL_CONNECTED) {
//    delay(500);
////    Serial.print(".");
//  }

//  pinMode(red,OUTPUT);
//  pinMode(green,OUTPUT);
//  pinMode(blue,OUTPUT);  
  ledcSetup(red, freq, resolution);
  ledcSetup(green, freq, resolution);
  ledcSetup(blue, freq, resolution);
  
  ledcAttachPin(red_, red);
  ledcAttachPin(green_, green);
  ledcAttachPin(blue_, blue);
  
  pinMode(33,OUTPUT);  
  pinMode(photo,INPUT);  
  
  digitalWrite(33,LOW);

  ledcWrite(red, pwm);
  ledcWrite(green, 0);
  ledcWrite(blue, 0);
//  server.begin();
  timeku=millis();
}


//kalibrasi
void loop(void){
  Serial.println(analogRead(photo));
  
  if(millis()-timeku>=5000){
    flag_=flag_+1;
    timeku=millis();
  }

  if(flag_==1){
    ledcWrite(red, 0);
    ledcWrite(green, pwm);
  }
  else if(flag_==2){
    ledcWrite(blue, pwm);
    ledcWrite(green, 0);
  }
  else {
    flag_=0; 
    ledcWrite(red, pwm);
    ledcWrite(blue, 0);  
  }
  delay(10);
//  pwm_();
}
/*
void pwm_(){
 WiFiClient client = server.available();   // Listen for incoming clients
  if (client) {                             // If a new client connects,
    currentTime = millis();
    previousTime = currentTime;
    //Serial.println("New Client.");          // print a message out in the serial port
    String currentLine = "";                // make a String to hold incoming data from the client
    while (client.connected() && currentTime - previousTime <= timeoutTime) {  // loop while the client's connected
      currentTime = millis();
      if (client.available()) {             // if there's bytes to read from the client,
        char c = client.read();             // read a byte, then
//        Serial.write(c);                    // print it out the serial monitor
        header += c;
        if (c == '\n') {                    // if the byte is a newline character
          // if the current line is blank, you got two newline characters in a row.
          // that's the end of the client HTTP request, so send a response:
          if (currentLine.length() == 0) {
            // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
            // and a content-type so the client knows what's coming, then a blank line:
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println("Connection: close");
            client.println();
            
            // turns the GPIOs on and off
            if (header.indexOf("GET /26/off") >= 0) {
              //Serial.println("Tambah PWM");
              pwm=pwm+10;
//              EEPROM.write(1,pwm);
//              EEPROM.commit();
            } else if (header.indexOf("GET /27/off") >= 0) {
              //Serial.println("Kurang PWM");
              pwm=pwm-10;
//              EEPROM.write(1,pwm);
//              EEPROM.commit();}
//            } else if (header.indexOf("GET /27/on") >= 0) {
//              Serial.println("GPIO 27 on");
//              output27State = "on";
//              digitalWrite(output27, HIGH);
//            } else if (header.indexOf("GET /27/off") >= 0) {
//              Serial.println("GPIO 27 off");
//              output27State = "off";
//              digitalWrite(output27, LOW);
//            }
            
            // Display the HTML web page
            client.println("<!DOCTYPE html><html>");
            client.println("<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
            client.println("<link rel=\"icon\" href=\"data:,\">");
            // CSS to style the on/off buttons 
            // Feel free to change the background-color and font-size attributes to fit your preferences
            client.println("<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}");
            client.println(".button { background-color: #4CAF50; border: none; color: white; padding: 16px 40px;");
            client.println("text-decoration: none; font-size: 30px; margin: 2px; cursor: pointer;}");
            client.println(".button2 {background-color: #555555;}</style></head>");
            
            // Web Page Heading
            client.println("<body><h1>PWM Calibrator</h1>");
            
            // Display current state, and ON/OFF buttons for GPIO 26  
            client.println("<p>Change PWM Value : </p>" +(String)pwm);
            client.println("<p><a href=\"/26/off\"><button class=\"button button2\">+</button></a>");
            client.println("<a href=\"/27/off\"><button class=\"button button2\">-</button></a></p>");
            client.println("</body></html>");
            
            // The HTTP response ends with another blank line
            client.println();
            // Break out of the while loop
            break;
          } else { // if you got a newline, then clear currentLine
            currentLine = "";
          }
        } else if (c != '\r') {  // if you got anything else but a carriage return character,
          currentLine += c;      // add it to the end of the currentLine
        }
      }
    }
    // Clear the header variable
    header = "";
    // Close the connection
    client.stop();
//    Serial.println("Client disconnected.");
//    Serial.println("");
    }
  }
}


void loop(void) {  
//  display.clearBuffer();         // clear the internal memory

    display.setFont(u8g2_font_ncenB08_tr);
    display.drawStr(0, 10, "IGLUCO!");
    delay(1000);
    
    display.clearBuffer();
    display.drawStr(0, 10, "Menghubungkan Ke Wifi");
    display.sendBuffer();
    delay(1000);
    
    
    display.clearBuffer();
    display.drawStr(0, 10, "Memulai IGLUCO");
    display.sendBuffer();
    delay(1000);
    
    display.clearBuffer();
    display.drawStr(0, 10, "Masukkan Jari Anda!");
    delay(5000);
    display.sendBuffer();
    
    display.clearBuffer();
    display.drawStr(0, 10, "Mengambil Data!");
    delay(10000);
    display.sendBuffer();
    
    display.clearBuffer();
    display.drawStr(0, 10, "Silahkan Keluarkan Jari Anda!");
    delay(1000);
    display.sendBuffer();
}
*/
