#include "ICM20648.h"

//extern int initICM20648();
//extern uint8_t readRegister(uint8_t);
//extern void writeRegister(uint8_t, uint8_t);

//SoftwareSerial ds(4 ,5); //RX, TX
ICM20648 icm = ICM20648();

void setup() {
  Serial.begin(9600);
  Serial.println("ICM-20648");
  /*
  ds.print("Device ID: 0x");
  uint8_t did = readRegister(REG_WHO_AM_I);
  ds.println(did, HEX);
  */

  if(!icm.init()){
    Serial.println("Error:initialization");
    while(1);  
  }


}

void loop() {
  // put your main code here, to run repeatedly:

}

