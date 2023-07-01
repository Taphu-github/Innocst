# Innocst
INNOcst is a cansat developed by me and my friends to participate in the cansat competition held by the Bhutan spaceweek. The component we used are Arduino UNO*2, NRF24l01 PA+ LA+ *2, BMP280 and NEO6. 

This are my team members: Tenzin Kuenkhyab, Ugyen Lhendrup, Yeshi Tshomo, Sonam Yuden and myself( Tandin Phuntsho)
PIN CONNECTION OF THE SATELLITE
BMP280-ARDUINO UNO[ lib used is Adafruit bmp280 ]
SDA-A5
SCI-A4
VCC-3.3v/5v
GND-GND

NEO6-ARDUINO UNO[ lib used is Tinygps++ ]
RX-D4
TX-D3
VCC-3.3v/5v
GND-GND
NEO-6 requires line of sight and some time to get lcoked in, therefore you might want to wait for few mins while keeping under the clear sky. 

NRF24l01-ARDUINO UNO[ lib used is RF24 ]
VCC-5v
CE-9
CSN-10
SCK-13
MOSI-11
MISO-12
GND-GND

You might have seen that NRF24 should not be given 5v or else the sensor will break but the NRF24l01 PA+ LA+ needs higher power to transfer the signals since it has external antenna. If the NRF24l01 is not working quite well even with 5v, Try attaching/soldering a 100ohm capacitor to the VCC and GND of the NRF24l01. This is only for the NRF24l01 PA+ LA+, other version of the sensor might only require 3.3v. 

