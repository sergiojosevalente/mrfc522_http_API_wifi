#include <SPI.h>
#include <MFRC522.h>

#include <WiFi.h>
#include <HTTPClient.h>

//Network SSID
const char* ssid = "id";
const char* password = "passworddisini";

#define SS_PIN 5
#define RST_PIN 0

int relay=2;

MFRC522 mfrc522(SS_PIN, RST_PIN);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  pinMode(relay,OUTPUT);
  digitalWrite(relay,HIGH);
  //cek koneksi wifi
  while(WiFi.status() != WL_CONNECTED)
  {
    //progress sedang mencari WiFi
    delay(500);
    Serial.print(".");
  }
  Serial.println("Wifi Connected");
  Serial.println("IP Address : ");
  Serial.println(WiFi.localIP());

  SPI.begin();
  mfrc522.PCD_Init();
  Serial.println("Dekatkan Kartu RFID Anda ke Reader");
  Serial.println();
}

void loop() {
  // put your main code here, to run repeatedly:

  if(! mfrc522.PICC_IsNewCardPresent())
    return ;

  if(! mfrc522.PICC_ReadCardSerial())
    return ;

  String IDTAG = "";
  for(byte i=0; i<mfrc522.uid.size; i++)
  {
    IDTAG += mfrc522.uid.uidByte[i];
  }

  if(WiFi.status()== WL_CONNECTED){
    HTTPClient http;
    String Link = "http://gunakanapi" + IDTAG;
    http.begin(Link.c_str());

    int httpCode = http.GET();
    String payload = http.getString();
    Serial.println(payload);
    if(payload.equalsIgnoreCase("success")){
      digitalWrite(relay,LOW);
    }
    http.end();
    delay(3000);
    digitalWrite(relay,HIGH);
    delay(2000);
  } else {
    Serial.println("WiFi Disconnected");
  }
}