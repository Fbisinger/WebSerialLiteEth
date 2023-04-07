#ifndef WebSerial_h
#define WebSerial_h

#include <functional>

#include "Arduino.h"
#include "stdlib_noniso.h"

#if defined(ESP32)
#define HARDWARE "ESP32"
#include "AsyncTCP.h"
#include "AsyncWebServer_ESP32_W5500.h"
#endif

typedef std::function<void(AsyncWebSocketClient *)> ConnHandler;
typedef std::function<void(AsyncWebSocketClient *)> DisconnHandler;
typedef std::function<void(uint8_t *data, size_t len)> RecvMsgHandler;
typedef std::function<void(AsyncWebSocketClient *, uint8_t *data,
                           size_t length)>
    RecvMsgHandlerPlus;
typedef std::function<void(AsyncWebSocketClient *, uint16_t code,
                           const char *reason, size_t reason_length)>
    ErrHandler;

// Uncomment to enable webserial debug mode
// #define WEBSERIAL_DEBUG

class WebSerialClass : public Print {
 public:
  void begin(AsyncWebServer *server, const char *url = "/webserial");

  void onConnect(ConnHandler callbackFunc);
  void onDisconnect(DisconnHandler callbackFunc);
  void onMessage(RecvMsgHandler callbackFunc);
  void onMessage(RecvMsgHandlerPlus callbackFunc);
  void onError(ErrHandler callbackFunc);

  // Print

  size_t write(uint8_t);
  size_t write(const uint8_t* buffer, size_t size);

 private:
  AsyncWebServer *_server;
  AsyncWebSocket *_ws;
  ConnHandler _connCallback = NULL;
  DisconnHandler _disconnCallback = NULL;
  RecvMsgHandler _recvMsgCallback = NULL;
  RecvMsgHandlerPlus _recvMsgCallbackPlus = NULL;
  ErrHandler _errCallback = NULL;

#if defined(WEBSERIAL_DEBUG)
  void DEBUG_WEB_SERIAL(const char *message);
#endif
};

extern WebSerialClass WebSerial;
#endif
