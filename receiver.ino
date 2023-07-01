#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

#define INTERVAL_MS_SIGNAL_LOST 1000 
#define INTERVAL_MS_SIGNAL_RETRY 250 

RF24 radio(9, 10, 2);  // CE, CSN pins
unsigned long lastSignalMillis = 0;
const byte address[6] = "00001";
void setup() {
  Serial.begin(9600);
 // radio.setAutoAck(false);
  bool stat=radio.begin();
  Serial.println(stat);

  //Append ACK packet from the receiving radio back to the transmitting radio 
	//radio.setAutoAck(false); //(true|false) 
	 //Set the transmission datarate 
	//bool ack=radio.setDataRate(RF24_2MBPS); //(RF24_250KBPS|RF24_1MBPS|RF24_2MBPS) 
	//Serial.println(ack);
   //Greater level = more consumption = longer distance 
	//radio.setPALevel(RF24_PA_HIGH); //(RF24_PA_MIN|RF24_PA_LOW|RF24_PA_HIGH|RF24_PA_MAX) 
	 //Default value is the maximum 32 bytes1 
	radio.setPayloadSize(32); 

  radio.openReadingPipe(0, address);  // Set the receiving pipe address
  radio.startListening();
  delay(5);
}

void loop() {
  bool anss=radio.isChipConnected();
  Serial.println(anss);
  bool ans=radio.available();
  Serial.println(ans);
  unsigned long currentMillis = millis(); 

   while (radio.available()) {                                   // While there is data ready
     char receivedMessage[32] ="";  // Buffer to store the received message
     radio.read(&receivedMessage, sizeof(receivedMessage));
     Serial.println("Data: "+String(receivedMessage));
	   lastSignalMillis = currentMillis; 
    }

  if (currentMillis - lastSignalMillis > INTERVAL_MS_SIGNAL_LOST) { 
	   lostConnection(); 
	 } 
   delay(1000);
} 
void lostConnection() 
{ 
	 Serial.println("We have lost connection, preventing unwanted behavior"); 
	 delay(INTERVAL_MS_SIGNAL_RETRY); 
} 
