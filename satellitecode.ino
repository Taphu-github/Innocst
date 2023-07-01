#include <Arduino.h>
#include "string.h"
//for BMP280
#include <Wire.h>
#include <SPI.h>
#include <Adafruit_BMP280.h>

#define BMP_SCK  (13)
#define BMP_MISO (12)
#define BMP_MOSI (11)
#define BMP_CS   (10)
Adafruit_BMP280 bmp; // I2C
//Adafruit_BMP280 bmp(BMP_CS); // hardware SPI
//Adafruit_BMP280 bmp(BMP_CS, BMP_MOSI, BMP_MISO,  BMP_SCK);

//for GY-GPS6MV2
#include <TinyGPSPlus.h>
#include <SoftwareSerial.h>

//for Nrf24
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

//for BMP280

float temperature=0.0, pressure=0.0, altitude=0.0;            // Create the temperature, pressure and altitude variables


//for GY-GPS6MV2
static const int RXPin = 4, TXPin = 3;
static const uint32_t GPSBaud = 4800;
TinyGPSPlus gps;// The TinyGPSPlus object
SoftwareSerial ss(RXPin, TXPin);// The serial connection to the GPS device
double latitude=0.00;
double longitude=0.00;
int day=0,month=0,year=0,hour=0,minute=0,second=0;

//for Nrf24
RF24 radio(9, 10);  // CE, CSN pins
const byte address[6]="00001";


void setup() {
  Serial.print("Starting : ");
  
  Serial.begin(9600);

  

  //bmp280
  /* Initialize BMP280 */
  if (!bmp.begin(0x76)) {
    Serial.println("Error initializing BMP280");
  }
  /* Default settings from datasheet. */
  bmp.setSampling(Adafruit_BMP280::MODE_NORMAL,     /* Operating Mode. */
                  Adafruit_BMP280::SAMPLING_X2,     /* Temp. oversampling */
                  Adafruit_BMP280::SAMPLING_X16,    /* Pressure oversampling */
                  Adafruit_BMP280::FILTER_X16,      /* Filtering. */
                  Adafruit_BMP280::STANDBY_MS_500); /* Standby time. */               // Start BMP280 continuous conversion in NORMAL_MODE  

  //for GY-GPS6MV2
  ss.begin(GPSBaud);

    //nrf24
  radio.begin();
  radio.openWritingPipe(address);  // Set the transmitting pipe address
  radio.setPALevel(RF24_PA_LOW);
  radio.stopListening();
  //radio.setAutoAck(false);
}



void loop() {
  // put your main code here, to run repeatedly:
  // This sketch displays information every time a new sentence is correctly encoded.
  while (ss.available() > 0)
    if (gps.encode(ss.read()))
      displayInfo();

  if (millis() > 1000 && gps.charsProcessed() < 10)
  {
    Serial.println(F("No GPS detected: check wiring."));
   // while(true);
  }

  //bmp280
  if(bmp.readTemperature() || bmp.readPressure() || bmp.readAltitude(1013.25)){
    temperature=bmp.readTemperature();
    pressure=bmp.readPressure();
    altitude=bmp.readAltitude(1013.25); /* Adjusted to local forecast! */
   // Serial.println(bmp.readTemperature());
  }else{
    temperature=0.0;
    pressure=0.0;
    altitude=0.0; /* Adjusted to local forecast! */
  }



  if (1) {//dtostrf()itoa()
    char temp[6],pres[8],lat[10],lon[10],alt[7];
    dtostrf(temperature,4,2,temp);
    dtostrf(pressure,6,2,pres);
    dtostrf(latitude,8,2,lat);
    dtostrf(longitude,8,2,lon);
    dtostrf(altitude,5,2,alt);
    String textMessages = "temp: "+String(temperature)+" pres: "+String(pressure);
    String textMessage0=" Alt(aproximate): "+String(altitude);
    String textMessage1="Latitude: "+String(lat)+" Longitude: "+String(lon);// Message to be transmitted
    
    char charArray[textMessages.length() + 1]; // Add 1 for the null character '\0'
    char charArray1[textMessage0.length() + 1]; // Add 1 for the null character '\0'
    char charArray2[textMessage1.length() + 1]; // Add 1 for the null character '\0'
    textMessages.toCharArray(charArray, sizeof(charArray));
    textMessage0.toCharArray(charArray1, sizeof(charArray1));
    textMessage1.toCharArray(charArray2, sizeof(charArray2));
    int d=radio.write(&charArray, sizeof(charArray));
    delay(1000);
    bool e= radio.write(&charArray1, sizeof(charArray1));
    delay(1000);
    bool f=radio.write(&charArray2, sizeof(charArray2));
    
    
    //bool e=radio.write(&textMessage0, sizeof(textMessage0));
    //bool f=radio.write(&textMessage1, sizeof(textMessage1));
    Serial.println(d);
    Serial.println(e);
    Serial.println(f);
    Serial.println("Message sent: " + String(textMessages)+" "+String(textMessage0)+" "+String(textMessage1));
      // Delay between transmissions
  } 
  //Serial.println();
  //delay(300);
  delay(1000);
}

void displayInfo()
{
 // Serial.print(F("Location: ")); 
  if (gps.location.isValid())
  {
    latitude=gps.location.lat();
    longitude=gps.location.lng();
    
  }
  else
  {
    //Serial.print(F("INVALID"));
  }

 // Serial.println(F("  Date/Time: "));
  if (gps.date.isValid())
  {
    month=gps.date.month();
    day=gps.date.day();
    year=gps.date.year();
  }
  else
  {
    month=99;
    day=99;
    year=99;
  }

 // Serial.print(F(" "));
  if (gps.time.isValid())
  {

    hour=gps.time.hour();
    minute=gps.time.minute();
    second=gps.time.second();
  
  }
  else
  {
    hour=99;
    minute=99;
    second=99;
  }

}
