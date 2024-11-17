/*********
  Rui Santos
  Complete project details at https://RandomNerdTutorials.com/ttgo-lora32-sx1276-arduino-ide/
*********/

//Libraries for LoRa
#include <SPI.h>
#include <LoRa.h>
#include <ESP32Servo.h>

//Libraries for OLED Display
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

//define the pins used by the LoRa transceiver module
#define SCK 5
#define MISO 19
#define MOSI 27
#define SS 18
#define RST 23
#define DIO0 26
#define propPin 12
#define magicPin 13



//433E6 for Asia
//866E6 for Europe
//915E6 for North America
#define BAND 866E6

//OLED pins
#define OLED_SDA 21
#define OLED_SCL 22
#define OLED_RST -1
#define SCREEN_WIDTH 128  // OLED display width, in pixels
#define SCREEN_HEIGHT 64  // OLED display height, in pixels

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RST);

String LoRaData;
Servo sg90;

void setup() {
  //initialize Serial Monitor
  Serial.begin(115200);

  //reset OLED display via software
  pinMode(OLED_RST, OUTPUT);
  pinMode(propPin, OUTPUT);
  pinMode(magucPin, OUTPUT);


  digitalWrite(OLED_RST, LOW);

  delay(20);

  digitalWrite(OLED_RST, HIGH);

  //initialize OLED
  Wire.begin(OLED_SDA, OLED_SCL);
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3c, false, false)) {  // Address 0x3C for 128x32
    Serial.println(F("SSD1306 allocation failed"));
    for (;;)
      ;  // Don't proceed, loop forever
  }

  display.clearDisplay();
  display.setTextColor(WHITE);
  display.setTextSize(1);
  display.setCursor(0, 0);
  display.print("LORA RECEIVER ");
  display.display();

  Serial.println("LoRa Receiver Test");

  //SPI LoRa pins
  SPI.begin(SCK, MISO, MOSI, SS);
  //setup LoRa transceiver module
  LoRa.setPins(SS, RST);
  pinMode(DIO0, INPUT);  // Sett opp DIO0 som input hvis det er nødvendig

  if (!LoRa.begin(BAND)) {
    Serial.println("Starting LoRa failed!");
    while (1)
      ;
  }
  Serial.println("LoRa Initializing OK!");
  display.setCursor(0, 10);
  display.println("LoRa Initializing OK!");
  display.display();

  sg90.attach(2);
}



void loop() {

  //try to parse packet
  int packetSize = LoRa.parsePacket();
  if (packetSize) {
    //received a packet
    Serial.print("Received packet ");

    //read packet
    while (LoRa.available()) {
      LoRaData = LoRa.readString();
      int venstre = LoRaData.substring(0, 1).toInt();
      int opp = LoRaData.substring(1, 2).toInt();
      int ned = LoRaData.substring(2, 3).toInt();
      int ikkeVenstre = LoRaData.substring(3).toInt();
      Serial.print(LoRaData);

      if (ikkeVenstre == 1) {
        display.print(LoRaData);
        sg90.write(180);
      } else if (venstre == 1) {
        display.print(LoRaData);
        sg90.write(90);
      } else {
        sg90.write(135);
      }

      if (opp == 1) {
        digitalWrite(propPin, HIGH);
      }
      // else if (LoRaData == "Down"){

      // }
      else {
        digitalWrite(propPin, LOW);
      }
      if (ned == 1) {
        digitalWrite(propPin, HIGH);
        digitalWrite(magicPin, HIGH);
      } else {

      }
    }

    //print RSSI of packet
    int rssi = LoRa.packetRssi();
    Serial.print(" with RSSI ");
    Serial.println(rssi);

    // Dsiplay information
    display.clearDisplay();
    display.setCursor(0, 0);
    display.print("LORA RECEIVER");
    display.setCursor(0, 20);
    display.print("Received packet:");
    display.setCursor(0, 30);
    display.print(LoRaData);
    display.setCursor(0, 40);
    display.print("RSSI:");
    display.setCursor(30, 40);
    display.print(rssi);
    display.display();

    delay(200);
  }
}