/* 
 *  Created: 19/04/2022
 *  Last edited 18/04/2023
 */

#include <Arduino.h>
#include <WiFi.h>
#include <string.h>
#include <tcpClient.h>
#include <Adafruit_SSD1306.h>
#include <Wire.h>

const char* ssid = "ssid";
const char* password = "password";
char address[] = "192.168.43.11";

#define PORT_NUMBER 50885
#define DEV_NAME "ZEsp-32"    // set the name we want to give your device
#define I2C_ADDR 0x3c     // address of the oled display
#define WIDTH 128
#define HEIGHT 64
#define OLED_RESET -1

void receptionHandler(char* data, uint32_t size);
void reconnect();

TcpClient *client;
char *clientName;

uint8_t pos = 0;   // character position
char rx_buf[256];
Adafruit_SSD1306 display(WIDTH, HEIGHT, &Wire, OLED_RESET);

void setup() {
  
  Serial.begin(9600);
  Serial.setDebugOutput(true);
  Serial.println();

  if(!display.begin(SSD1306_SWITCHCAPVCC, I2C_ADDR)) { 
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }

  display.display();
  delay(2000);
  display.clearDisplay();
  display.display();
  display.setTextSize(2);  /*OLED screen text size defined*/
  display.setTextColor(WHITE); /*OLED screen text color*/
  display.setCursor(0, 20); /*Display static text*/
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.print("Client configured at ");
  Serial.print(WiFi.localIP());
  Serial.println();

  client = new TcpClient(DEV_TYPE::DISP, DEV_NAME, address, PORT_NUMBER);
  client->setReceptionHandler(receptionHandler);
  client->setDisconnectHandler(reconnect);

  client->connectTo();

  memset(rx_buf, '\0', 256);
}

void loop() {
  // put your main code here, to run repeatedly:
  delay(10000);  
}

void receptionHandler(char *data, uint32_t size)
{
  /* The structure of each text transmission is text + '\0' */
  for(int i=0; i<size; i++){
    if(data[i] == '\n'){
      display.clearDisplay();
      display.setCursor(0, 20); /*Display static text*/
      display.print(rx_buf);
      display.display();
      pos = 0;
      memset(rx_buf, '\0', 256);
      return;
    } else{
      rx_buf[pos] = data[i];
      pos ++;
    }
  }
}

void reconnect()  // to be called on disconnection
{
  sleep(1);      // wait about 1 s
  client->connectTo();
}