/*
  Sketch modified by Ktechnik on Youtube:
  https://www.youtube.com/channel/UC8d0s5nv6YUfsSpE1L8X2ZQ
*/
//Define libraries to use Supla with ESP-01s
//You must download and install library for Supla. There you have it:
//https://drive.google.com/file/d/0B3DJSAQwtTAyVGVZLWpkUTlLcWc/view

#include <srpc.h>
#include <log.h>
#include <eh.h>
#include <proto.h>
#include <IEEE754tools.h>
// We define our own ethernet layer
#define SUPLADEVICE_CPP
#include <SuplaDevice.h>
#include <lck.h>
#include "DHT.h"
#include <WiFiClient.h>
#include <ESP8266WiFiType.h>
#include <ESP8266WiFi.h>
#include <ESP8266WiFiScan.h>
#include <ESP8266WiFiMulti.h>
#include <WiFiServer.h>
#include <ESP8266WiFiGeneric.h>
#include <WiFiClientSecure.h>
#include <ESP8266WiFiAP.h>
#include <ESP8266WiFiSTA.h>
#include <WiFiUdp.h>

WiFiClient client;

const char* ssid     = "ssid"; //Setting SSID for your WiFi
const char* password = "pass"; //Setting password for your WiFi

#define DHTPIN 2 //DHT pin
#define DHTTYPE DHT22 // DHT type (DHT11, DHT21, DHT22)

DHT dht(DHTPIN, DHTTYPE);//Setting DHT sensor

//Downloading data from sensor
void get_temperature_and_humidity(int channelNumber, double *temp, double *humidity){

  *temp = dht.readTemperature();
  *humidity = dht.readHumidity();

  if ( isnan(*temp) || isnan(*humidity) ) {
    *temp = 0;
    *humidity = 0;
  }
}

void setup() {
  dht.begin(); //initiating sensor

  Serial.begin(115200);
  delay(10);

  SuplaDevice.setTemperatureHumidityCallback(&get_temperature_and_humidity);
  //Function to give to Supla data of sensor

  char GUID[SUPLA_GUID_SIZE] = {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
  // Setting GUID to connect to Supla (you can get it from https://www.supla.org/arduino/get-guid)


  uint8_t mac[6] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00}; //Setting mac of your device
  SuplaDevice.addDHT22();//Add sensor
  SuplaDevice.begin(GUID, mac, "svr30.supla.org", 1234, "1234"); // Setting connection to Supla in order GUID, mac, sever adress, Location ID, password
}
void loop() {
  SuplaDevice.iterate();//start using Supla

}
//Functions with must be there to correct operation

int supla_arduino_tcp_read(void *buf, int count) {
  _supla_int_t size = client.available();

  if ( size > 0 ) {
    if ( size > count ) size = count;
    return client.read((uint8_t *)buf, size);
  };

  return -1;
};

int supla_arduino_tcp_write(void *buf, int count) {
  return client.write((const uint8_t *)buf, count);
};

bool supla_arduino_svr_connect(const char *server, int port) {
  return client.connect(server, 2015);
}

bool supla_arduino_svr_connected(void) {
  return client.connected();
}

void supla_arduino_svr_disconnect(void) {
  client.stop();
}

void supla_arduino_eth_setup(uint8_t mac[6], IPAddress *ip) {
  //Connection to WiFi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.print("\nlocalIP: ");
  Serial.println(WiFi.localIP());
  Serial.print("subnetMask: ");
  Serial.println(WiFi.subnetMask());
  Serial.print("gatewayIP: ");
  Serial.println(WiFi.gatewayIP());
}
SuplaDeviceCallbacks supla_arduino_get_callbacks(void) {
  SuplaDeviceCallbacks cb;

  cb.tcp_read = &supla_arduino_tcp_read;
  cb.tcp_write = &supla_arduino_tcp_write;
  cb.eth_setup = &supla_arduino_eth_setup;
  cb.svr_connected = &supla_arduino_svr_connected;
  cb.svr_connect = &supla_arduino_svr_connect;
  cb.svr_disconnect = &supla_arduino_svr_disconnect;
  cb.get_temperature = NULL;
  cb.get_temperature_and_humidity = NULL;
  cb.get_rgbw_value = NULL;
  cb.set_rgbw_value = NULL;

  return cb;
}
