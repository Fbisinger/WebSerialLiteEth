/*
  WebSerialLiteEth Demo
  ------
  This example code works for  ESP32 Microcontrollers with W5500 Ethernet Controller
  WebSerial is accessible at your Ethernet Controller's <IPAddress>/webserial URL.

  Author: Fbisinger
*/
#include <Arduino.h>
#include <AsyncTCP.h>
#include <AsyncWebServer_ESP32_W5500.h>
#include <WebSerialLite.h>
#include <SPI.h>

#define _ASYNC_WEBSERVER_LOGLEVEL_       2

byte mac[6] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0x0D };

//////////////////////////////////////////////////////////

// Optional values to override default settings
// Don't change unless you know what you're doing
//#define ETH_SPI_HOST        SPI3_HOST
//#define SPI_CLOCK_MHZ       25

// Must connect INT to GPIOxx or not working
#define INT_GPIO 35
#define MISO_GPIO 19
#define MOSI_GPIO 23
#define SCK_GPIO 18
#define CS_GPIO 5

AsyncWebServer server(80);



/* Message callback of WebSerial */
void recvMsg(uint8_t *data, size_t len){
  WebSerial.println("Received Data...");
  String d = "";
  for(int i=0; i < len; i++){
    d += char(data[i]);
  }
  WebSerial.println(d);
}

void notFound(AsyncWebServerRequest *request)
{
  request->send(404, "text/plain", "Not found");
}

void setup() {
  Serial.begin(115200);
  while (!Serial && millis() < 5000);

  AWS_LOGWARN(F("Default SPI pinout:"));
  AWS_LOGWARN1(F("SPI_HOST:"), ETH_SPI_HOST);
  AWS_LOGWARN1(F("MOSI:"), MOSI_GPIO);
  AWS_LOGWARN1(F("MISO:"), MISO_GPIO);
  AWS_LOGWARN1(F("SCK:"),  SCK_GPIO);
  AWS_LOGWARN1(F("CS:"),   CS_GPIO);
  AWS_LOGWARN1(F("INT:"),  INT_GPIO);
  AWS_LOGWARN1(F("SPI Clock (MHz):"), SPI_CLOCK_MHZ);
  AWS_LOGWARN(F("========================="));

  ///////////////////////////////////

  // To be called before ETH.begin()
  ESP32_W5500_onEvent();
  
  ETH.begin( MISO_GPIO, MOSI_GPIO, SCK_GPIO, CS_GPIO, INT_GPIO, SPI_CLOCK_MHZ, ETH_SPI_HOST, mac );
  ESP32_W5500_waitForConnect();

  ///////////////////////////////////

  Serial.println(ETH.localIP());
  
  // Not Found Notification
  server.onNotFound(notFound);
  
  // WebSerial is accessible at "<IP Address>/webserial" in browser
  WebSerial.begin(&server);
  
  /* Attach Message Callback */
  WebSerial.onMessage(recvMsg);
  server.begin();
}

void loop() {
    delay(2000);
    WebSerial.printf("Millis=%lu\n", millis());
    WebSerial.printf("Free heap=[%u]\n", ESP.getFreeHeap());
}