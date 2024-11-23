//Deler av denne koden er hentet fra lenken under men er modifisert siden koden ikke brukte samme chip og derfor hadde feil instillinger. 

/*********
  Rui Santos
  Complete project details at https://RandomNerdTutorials.com/ttgo-lora32-sx1276-arduino-ide/
*********/

//Libraries for LoRa
#include <SPI.h>
#include <LoRa.h>
//#include <Servo.h>

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

//433E6 for Asia
//866E6 for Europe
//915E6 for North America
#define BAND 866E6

//OLED pins
#define OLED_SDA 21
#define OLED_SCL 22 
#define OLED_RST -1
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels



int variabel = 0;
int count = 0;



Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RST);

//kode for knapper
// set pin numbers for the five buttons:
const int leftButton = 02; //venstre
const int upButton = 15; // opp
const int downButton = 12; // ned
const int rightButton = 14; // høgre
//

void setup() {
  //initialize Serial Monitor
  Serial.begin(115200);

  //reset OLED display via software
  pinMode(OLED_RST, OUTPUT);
  digitalWrite(OLED_RST, LOW);
  delay(20);
  digitalWrite(OLED_RST, HIGH);

  //initialize OLED
  Wire.begin(OLED_SDA, OLED_SCL);
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C, false, false)) { // Address 0x3C for 128x32
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }
  //srkiver til display på senderen, ikke vikrig for funksjon bare fdor testing og sånt
  display.clearDisplay();
  display.setTextColor(WHITE);
  display.setTextSize(1);
  display.setCursor(0,0);
  display.print("LORA SENDER ");
  display.display();
  
  Serial.println("LoRa Sender Test");

  //SPI LoRa pins
  SPI.begin(SCK, MISO, MOSI, SS);
  //setup LoRa transceiver module
  //LoRa.setPins(SS, RST, DIO0);
  LoRa.setPins(SS, RST);
  pinMode(DIO0, INPUT); // Sett opp DIO0 som input hvis det er nødvendig
// Loop for å vente på at lora sytarter
  if (!LoRa.begin(BAND)) {
    Serial.println("Starting LoRa failed!");
    while (1);
  }
  //test tekst 
  Serial.println("LoRa Initializing OK!");
  display.setCursor(0,10);
  display.print("LoRa Initializing OK!");
  display.display();
  delay(2000);

  //kode for knapper
  pinMode(leftButton, INPUT_PULLDOWN);
  pinMode(upButton, INPUT_PULLDOWN);
  pinMode(downButton, INPUT_PULLDOWN);
  pinMode(rightButton, INPUT_PULLDOWN);

  Serial.begin(115200);
}



void loop() {
   
  Serial.print("Sending packet: ");


  //Send LoRa packet to receiver
  LoRa.beginPacket();//s7tart av datapakke
  //Debugg screen
  display.clearDisplay();
  display.setCursor(0,0);
  display.println("LORA SENDER");
  display.setCursor(0,20);
  display.setTextSize(1);
  display.print("LoRa packet sent.");
  display.setCursor(0,30);
  display.setCursor(50,30);  
  display.print(count);
  display.setCursor(0,40);

  // dataoppsett for sending av info, for hver knapp sendes det 1 eller 0 for på eller av
  if (digitalRead(leftButton) == HIGH) {
    LoRa.print("1");
  }
  else{
    LoRa.print("0");

  }
  if (digitalRead(upButton) == HIGH) {
    LoRa.print("1");
  }
  else{
    LoRa.print("0");

  }
  if (digitalRead(downButton) == HIGH) {
    LoRa.print("1");
  }
  else{
    LoRa.print("0");

  }
  if (digitalRead(rightButton) == HIGH) {
    LoRa.print("1");
  }
  else{
    LoRa.print("0");

  }
  display.display();
  LoRa.endPacket();//sender datapakke
  count ++;

  
  delay(300);//denne burde være dynamisk for å følge loven men det går fint siden det ikke er et 24t system
}
