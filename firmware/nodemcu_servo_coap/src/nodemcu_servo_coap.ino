#include <SPI.h>
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include <coap.h>
#include <Servo.h>

const char *ssid = "IAiOT";
const char *password = "IA#iot@2017";

Servo myservo;
int pos = 0;
int servoState = 0;

// // CoAP client response callback
void callback_response(CoapPacket &packet, IPAddress ip, int port);

// // CoAP server endpoint url callback
void callback_light(CoapPacket &packet, IPAddress ip, int port);

// UDP and CoAP class
WiFiUDP udp;
Coap coap(udp);

// LED STATE
bool LEDSTATE;

// // CoAP server endpoint URL
void callback_light(CoapPacket &packet, IPAddress ip, int port)
{
  Serial.println("[Light] ON/OFF");

  // send response
  char p[packet.payloadlen + 1];
  memcpy(p, packet.payload, packet.payloadlen);
  p[packet.payloadlen] = NULL;

  String message(p);

  Serial.println(ip);
  Serial.println(port);
  Serial.println(packet.messageid);
  Serial.println(message);

  if (message.equals("0"))
  {
    LEDSTATE = false;
    servoState = HIGH;
  }
  else if (message.equals("1"))
  {
    LEDSTATE = true;
    servoState = LOW;
  }
  
  if (LEDSTATE)
  {
    digitalWrite(LED_BUILTIN, HIGH);
    coap.sendResponse(ip, port, packet.messageid, "1");
  }
  else
  {
    digitalWrite(LED_BUILTIN, LOW);
    coap.sendResponse(ip, port, packet.messageid, "0");
  }
}

// CoAP client response callback
void callback_response(CoapPacket &packet, IPAddress ip, int port)
{
  Serial.println("[Coap Response got]");

  char p[packet.payloadlen + 1];
  memcpy(p, packet.payload, packet.payloadlen);
  p[packet.payloadlen] = NULL;

  Serial.println(p);
}

void setup_coap()
{

  // add server url endpoints.
  // can add multiple endpoint urls.
  // exp) coap.server(callback_switch, "switch");
  //      coap.server(callback_env, "env/temp");
  //      coap.server(callback_env, "env/humidity");
  Serial.println("Setup Callback Light");
  coap.server(callback_light, "light");

  // client response callback.
  // this endpoint is single callback.
  Serial.println("Setup Response Callback");
  coap.response(callback_response);

  // start coap server/client
  coap.start();
}

void setup_wifi()
{

  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void setup()
{
  // initialize digital pin LED_BUILTIN as an output.
  Serial.begin(9600);
  Serial.println("Setup");
  myservo.attach(4);
  LEDSTATE = true;
  pinMode(LED_BUILTIN, OUTPUT);

  setup_wifi();
  setup_coap();
}

void setup_loop()
{
    if (servoState == HIGH)
    {
        for (pos = 0; pos <= 180; pos += 1)
        { // goes from 0 degrees to 180 degrees
            // in steps of 1 degree
            myservo.write(pos); // tell servo to go to position in variable 'pos'
            delay(15);          // waits 15ms for the servo to reach the position
        }
        for (pos = 180; pos >= 0; pos -= 1)
        {                       // goes from 180 degrees to 0 degrees
            myservo.write(pos); // tell servo to go to position in variable 'pos'
            delay(15);          // waits 15ms for the servo to reach the position
        }
        servoState = LOW;        
    }
}

// the loop function runs over and over again forever
void loop()
{
  // digitalWrite(LED_BUILTIN, HIGH); // turn the LED on (HIGH is the voltage level)
  // Serial.println("HIGH");
  // delay(1000);                    // wait for a second
  // digitalWrite(LED_BUILTIN, LOW); // turn the LED off by making the voltage LOW
  // Serial.println("LOW");
  delay(1000); // wait for a second
  coap.loop();
  setup_loop();
  Serial.println("-");
}