#include "main.h"


//Either use the Include down below if you need the credentials in a seperate file 
//Or uncomment and provide creds here 
#include "credentials.c"
// const char* SSID = "";
// const char* PSK = "";
// const char* MQTT_BROKER = "";


const byte C1PIN = D5;
const byte C2PIN = D6; 
const byte C3PIN = D7; 


//Ticker blinker(changeState, 1000, 0, MILLIS);

 
WiFiClient espClient;
PubSubClient client(espClient);
long lastMsg = 0;
char msg[50];
int value = 0;

 
void setup() {
    pinMode(C1PIN, OUTPUT);
    pinMode(C2PIN, OUTPUT);
    pinMode(C3PIN, OUTPUT);

    Serial.begin(9600);

    //blinker.start(); 
    analogWrite(C1PIN,10);

    setup_wifi();

    analogWrite(C2PIN,10);


    //blinker.attach(0.2, changeState);
    client.setServer(MQTT_BROKER, 1883);

    analogWrite(C3PIN,10);


    client.setCallback(callback);

    analogWrite(C1PIN,0);
    analogWrite(C2PIN,0);
    analogWrite(C3PIN,0);


    //blinker.detach(); 
}
 
void setup_wifi() {
    delay(10);
    Serial.println();
    Serial.print("Connecting to ");
    Serial.println(SSID);
 
    WiFi.begin(SSID, PSK);
 
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
 
    Serial.println("");
    Serial.println("WiFi connected");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());
}
 
void callback(char* topic, byte* payload, unsigned int length) {
    Serial.print("Received message [");
    Serial.print(topic);
    Serial.print("] ");
    char msg[length+1];
    for (int i = 0; i < length; i++) {
        Serial.print((char)payload[i]);
        msg[i] = (char)payload[i];
    }
    Serial.println();
 
    //msg[length] = '&#92;&#48;';
    Serial.print("'");
    Serial.print(msg);
    Serial.println("'"); 

    Serial.println(strcmp(msg,"off"));

    byte pin = D5; 

    if(strcmp(topic, "/paulsroom/lamps/3lamp/c1") == 0) pin = C1PIN; 
    if(strcmp(topic, "/paulsroom/lamps/3lamp/c2") == 0) pin = C2PIN; 
    if(strcmp(topic, "/paulsroom/lamps/3lamp/c3") == 0) pin = C3PIN; 


    byte potval = atof(msg)*100; 

 

    if(strcmp(msg,"off")==0){
        digitalWrite(pin, LOW);
    } else if(potval >= 0 && potval < 86)
    {
        analogWrite(pin, potval);
        Serial.println(potval);
    }
}
 
void reconnect() {
    while (!client.connected()) {
        Serial.println("Reconnecting MQTT...");
        if (!client.connect("ESP8266Client")) {
            Serial.print("failed, rc=");
            Serial.print(client.state());
            Serial.println(" retrying in 5 seconds");
            delay(5000);
        }
    }
    client.subscribe("/paulsroom/lamps/3lamp/c1");
    client.subscribe("/paulsroom/lamps/3lamp/c2");
    client.subscribe("/paulsroom/lamps/3lamp/c3");


    Serial.println("MQTT Connected...");
}
 
void loop() {
    if (!client.connected()) {
        reconnect();
    }
    client.loop();
}

void changeState()
{
  digitalWrite(LED_BUILTIN, !(digitalRead(LED_BUILTIN)));  //Invert Current State of LED  
}






int main(void) {
    char * some_value;
    char my_value = '!';

    some_value = &my_value; 

    return 0;
}