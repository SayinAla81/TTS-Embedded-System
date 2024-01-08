#include <WiFi.h>
#include <PubSubClient.h>
#include "Arduino.h"
#include "String.h"
#include "XT_DAC_Audio.h";
#include "HTTPClient.h";

WiFiClient espClient;
PubSubClient client(espClient);

// WiFi
String ssid = "ssid";
String password = "pass";

// // MQTT Broker
const char *mqtt_broker = "broker.emqx.io";
const char *topic = "speech";
const char *mqtt_username = "emqx";
const char *mqtt_password = "public";
const int mqtt_port = 1883;

int counter = 0;
String receivedMessage, payload, lastry;

void setup()
{
  // setup wifi
  Serial.begin(4800);
  WiFi.disconnect();
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.println("Connecting...");
    delay(1500);
  }
  Serial.println("Coneected to wifi");

  // setup mqtt
  client.setServer(mqtt_broker, mqtt_port);
  client.setCallback(callback);
  while (!client.connected())
  {
    String client_id = "mqttx_5ad70d12";
    client_id += String(WiFi.macAddress());
    Serial.printf("The client %s connects to the public MQTT broker\n", client_id.c_str());
    if (client.connect(client_id.c_str(), mqtt_username, mqtt_password))
    {
      Serial.println("Public EMQX MQTT broker connected");
      delay(5000);
    }
    else
    {
      Serial.print("failed with state ");
      Serial.print(client.state());
      delay(5000);
    }
  }
}

void CallApi(String receivedMessage)
{
  String persianText = receivedMessage;
  persianText.trim();
  if (persianText.length() > 0)
  {
    String Url = "https://one-api.ir/tts/?token=116231:65805a94ad157&action=microsoft&lang=fa-IR-DilaraNeural&q=";
    Url += persianText;
    Serial.println("Requesting TTS from: " + Url);
    delay(5000);

    if (makeRequest(Url))
    {
      Serial.println("Request successful");
      stringToHex();
      Serial.println(lastry.length());
      unsigned char resultArray[lastry.length()];
      stringToUnsignedCharArray(lastry, resultArray, sizeof(resultArray));

      XT_Wav_Class ForceWithYou(resultArray);
      XT_DAC_Audio_Class DacAudio(26, 0);
      DacAudio.FillBuffer();
      if (ForceWithYou.Completed)
        DacAudio.PlayWav(&ForceWithYou);
    }
    else
    {
      Serial.println("Request failed");
    }
    delay(5000);
  }
}

void stringToUnsignedCharArray(const String &inputString, unsigned char *outputArray, size_t size)
{
  size_t length = inputString.length();
  if (size < length)
    return;
  for (size_t i = 0; i < length; ++i)
    outputArray[i] = (unsigned char)inputString.charAt(i);
}

void stringToHex()
{
  int hex_dec;
  lastry = "";
  for (const auto &item : payload)
  {
    hex_dec = int(item);
    char hexaDeciNum[100];
    int i = 0;
    while (hex_dec != 0)
    {
      int temp = 0;
      temp = hex_dec % 16;
      if (temp < 10)
        hexaDeciNum[i] = temp + 48, i++;
      else
        hexaDeciNum[i] = temp + 55, i++;
      hex_dec = hex_dec / 16;
    }
    for (int j = i - 1; j >= 0; j--)
      lastry += hexaDeciNum[j];
  }
}

bool makeRequest(String url)
{
  HTTPClient http;

  http.begin(url);
  int httpCode = http.GET();
  if (httpCode > 0)
  {
    if (httpCode == HTTP_CODE_OK)
    {
      payload = http.getString();
      Serial.println("payload");
      return true;
    }
    else
      Serial.printf("HTTP request failed with error code %d\n", httpCode);
  }
  else
  {
    Serial.println("HTTP request failed");
  }
  http.end();
  return false;
}

void callback(char *topic, byte *payload, unsigned int length)
{
  Serial.print("Message arrived in topic: ");
  Serial.println(topic);
  Serial.print("Message: ");
  for (int i = 0; i < length; i++)
  {
    Serial.print((char)payload[i]);
    receivedMessage += (char)payload[i];
  }
  Serial.println();
  Serial.println("-----------------------");
  Serial.print("Received Message: ");
  Serial.println(receivedMessage);
}

void loop()
{
  client.subscribe(topic);
  Serial.println("--------------------");
  Serial.println(receivedMessage);
  if (!receivedMessage.isEmpty())
  {
    CallApi(receivedMessage);
    client.publish(topic, receivedMessage.c_str());
    receivedMessage = "";
  }
  client.loop();

}