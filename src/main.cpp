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

const char *host = "maisonneuve.aws.thinger.io";
const char port = 80;
void callback(char* topic, byte* payload, unsigned int length) {
  // handle message arrived
}

WiFiClient wificlient;
PubSubClient client(host, 1883, callback, wificlient);

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





 enum code{
    zero,   //0
    un,     //1
    deux,   //1
    trois,   //3
    quatre  //4
};

int etat;

void setup() {
  // put your setup code here, to run once:
  pinMode(tempSensePin, INPUT);
  pinMode(button, INPUT_PULLUP);
  pinMode(coussin, OUTPUT);
  pinMode(LEDB, OUTPUT);
  pinMode(LEDR, OUTPUT);
  pinMode(LEDG, OUTPUT);
  Serial.begin(9600);
  etat = zero;
 // Connect to WiFi
  connectToNetwork();
  Serial.println(WiFi.localIP());

  // Connect to Thinger.io
  if (client.connect("RASPBERRY", "Godson", "nado2375")) {
    Serial.println("Connected to Thinger.io");
  } else {
    Serial.print("Connection failed, rc=");
    Serial.print(client.state());
  }





}


void loop() {
  float Vout = analogRead(tempSensePin)/310.48; // Voltage at pin
  float temp = (Vout-0.5)/0.010; // convert the analog reading to voltage and then to temperature in Celsius

  if(1)
  {
    switch (etat)
    {
    case zero:
      digitalWrite(LEDB, HIGH);
      delay(1000);
      digitalWrite(LEDB, LOW);
      delay(1000);
      Serial.print("temperature: ");
      Serial.println(temp);
      if (temp >= 24)
      {
        etat = zero;
      }
      else if (temp <= 23)
      {
        etat = un;
      }
      
      break;
    case un:
      digitalWrite(coussin, HIGH);
      digitalWrite(LEDR, HIGH);
      delay(500);
      digitalWrite(LEDR, LOW);
      delay(500);
      Serial.print("temperature: ");
      Serial.println(temp);
      
      if (temp >= 24)
      {
        etat = zero;

      }
      else if (temp <= 22)
      {
        etat = un;
      }
      
      break;

    default:
      break;
    }
   /* if(temp >= 24 ){
      digitalWrite(LED, HIGH);
      delay(1000);
      digitalWrite(LED, LOW);
      delay(1000);
      Serial.print("temperature: ");
      Serial.println(temp);

    }
    else if(temp <= 22){
      digitalWrite(LED, HIGH);
      delay(500);
      digitalWrite(LED, LOW);
      delay(500);
      Serial.print("temperature: ");
      Serial.println(temp);
    }
    
    
    Serial.print("temperature: ");
    Serial.println(temp);*/
  }
  else{digitalWrite(LEDG, LOW);}
  
  // put your main code here, to run repeatedly:
}

// put function definitions here:
int myFunction(int x, int y) {
  return x + y;
}