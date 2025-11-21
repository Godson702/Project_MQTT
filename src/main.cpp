#include <Arduino.h>
#include <PubSubClient.h>
#include "secret.h"
#include "WiFi.h"

const char *ssid = WIFI_SSID;
const char *password = WIFI_PASSWORD;

// put function declarations here:
int myFunction(int, int);
const int tempSensePin = A1;
const int button = D0;
const int coussin = D4;
const int LEDB = 25;
const int LEDR = 17;
const int LEDG = 16;
String topic = "RASPBERRY/temperature";
String subscribtion = "switch";

const char *host = "maisonneuve.aws.thinger.io";

void callback(char *subscribtion, byte *payload, unsigned int length)
{
  Serial.print("Message arrived [");
  Serial.print(subscribtion);
  Serial.print("] ");
  for (unsigned int i = 0; i < length; i++)
  {
    Serial.print((char)payload[i]);
  }


  if (payload[0] == '1' )
  {
    Serial.print((char)payload[0]);
    digitalWrite(LED_BUILTIN, LOW);
  }
  else  
  {
    digitalWrite(LED_BUILTIN, HIGH);
  }
}

WiFiClient wificlient;
PubSubClient client(host, 1883, callback, wificlient);
String translateEncryptionType(wifi_auth_mode_t encryptionType)
{

  switch (encryptionType)
  {
  case (WIFI_AUTH_OPEN):
    return "Open";
  case (WIFI_AUTH_WEP):
    return "WEP";
  case (WIFI_AUTH_WPA_PSK):
    return "WPA_PSK";
  case (WIFI_AUTH_WPA2_PSK):
    return "WPA2_PSK";
  case (WIFI_AUTH_WPA_WPA2_PSK):
    return "WPA_WPA2_PSK";
  case (WIFI_AUTH_WPA2_ENTERPRISE):
    return "WPA2_ENTERPRISE";
  default:
    return "UNKNOWN";
  }
  return "UNKNOWN";
}

void connectToNetwork()
{
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED)
  {
    delay(1000);
    Serial.println("Establishing connection to WiFi..");
  }

  Serial.println("Connected to network");
}

void scanNetworks()
{

  int numberOfNetworks = WiFi.scanNetworks();

  // Add delay so the terminal can catch up
  delay(3000);

  Serial.print("Number of networks found: ");
  Serial.println(numberOfNetworks);

  for (int i = 0; i < numberOfNetworks; i++)
  {

    Serial.print("Network name: ");
    Serial.println(WiFi.SSID(i));

    Serial.print("Signal strength: ");
    Serial.println(WiFi.RSSI(i));

    Serial.print("MAC address: ");
    Serial.println(WiFi.BSSIDstr(i));

    Serial.print("Encryption type: ");
    String encryptionTypeDescription = translateEncryptionType(WiFi.encryptionType(i));
    Serial.println(encryptionTypeDescription);
    Serial.println("-----------------------");
  }
}

void Clear()
{
  Serial1.write(0xFE);
  Serial1.write(0x51);
  delay(100);
}

void setup()
{
  Serial.begin(9600);

  Serial1.begin(9600, SERIAL_8N1, D7, D6);
  pinMode(LED_BUILTIN, OUTPUT);

  // Print MAC address
  Serial.println("MCU MAC address: " + WiFi.macAddress());

  scanNetworks();
  connectToNetwork();

  Serial.println(WiFi.macAddress());
  Serial.println(WiFi.localIP());

  // Connect to Thinger.io
  if (client.connect("RASPBERRY", "Godson", "nado2375"))
  {
    Serial.println("Connected to Thinger.io");
  }
  else
  {
    Serial.print("Connection failed, rc=");
    Serial.print(client.state());
  }
  Clear();

  client.subscribe(subscribtion.c_str());
}

static unsigned long lastMsg = 0;
void loop()
{

  // scanNetworks();
  client.loop();

  unsigned long now = millis();
  if (now - lastMsg > 2000)
  {
    lastMsg = now;
    float Vout = analogRead(tempSensePin) / 310.48; // Voltage at pin
    float temp = (Vout - 0.5) / 0.100;              // convert the analog reading to voltage and then to temperature in Celsius

    String topic = "RASPBERRY/temperature";
    String payload = String(temp);

    client.publish(topic.c_str(), payload.c_str());
    Serial.println("Published: " + payload);
//    Clear();
//    Serial1.print("T:");
//    Serial1.print(temp, 1);
//    Serial1.println(" C");
  }
}
