/*********
  Rui Santos
  Complete project details at http://randomnerdtutorials.com  
*********/

// Load Wi-Fi library
#include <WiFi.h>
#include <EEPROM.h>

// Replace with your network credentials
const char* ssid = "Izatou*";
const char* password = "izatou404";

// Set web server port number to 80
WiFiServer server(80);

// Variable to store the HTTP request
String header;

const int freq = 5000;
const int resolution = 12;

// Current time
unsigned long currentTime = millis();
// Previous time
unsigned long previousTime = 0; 
// Define timeout time in milliseconds (example: 2000ms = 2s)
const long timeoutTime = 2000;
int val=300,red_=22,red=0;
void setup() {
  Serial.begin(115200);
  EEPROM.begin(1);
  
  ledcSetup(red, freq, resolution);
  ledcAttachPin(red_, red);
  ledcWrite(red, val);
  pinMode(34,INPUT);  
  // Connect to Wi-Fi network with SSID and password
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  //val = EEPROM.read(1);
  Serial.println(val);
  // Print local IP address and start web server
  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  server.begin();
}
int y,x;
void loop(){
  ledcWrite(red, val);
  x = analogRead(34);y = map(x, 0, 4095, 0, 1023);
  Serial.print(x);Serial.print(",");Serial.println(y);
  
  
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
              val=val+1;
              EEPROM.write(1,val);
              EEPROM.commit();
            } else if (header.indexOf("GET /27/off") >= 0) {
              //Serial.println("Kurang PWM");
              val=val-1;
              EEPROM.write(1,val);
              EEPROM.commit();}
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
            client.println("<p>Change PWM Value : </p>" +(String)val);
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
  //  Serial.println("Client disconnected.");
    //Serial.println("");
  }
  delay(100);
}
