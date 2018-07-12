#include "ICM20648.h"

ICM20648::ICM20648(){
  _i2c = true;
}

int ICM20648::init(){
  
  uint8_t did = readRegister(REG_WHO_AM_I);
  Serial.print("Device ID: 0x");
  Serial.println(did, HEX);

  uint8_t ctrl = readRegister(REG_PWR_MGMT_1);
  Serial.println(ctrl, HEX);
  
  return 1;  

}

void ICM20648::setPowerMode(boolean mode){
  //mode => ture, low power mode
  //mode => false, high powerformance (low noise) mode
  
  if(mode){
    //writeRegister()  
  }else{
    
  }
  
}

uint8_t ICM20648::readRegister(uint8_t reg){
  uint8_t bank = reg >> 7;
  reg = 0x7F & reg;
  Serial.print("bank: ");
  Serial.print(bank);
  Serial.print(" reg: ");
  Serial.println(reg);
  
  Wire.beginTransmission(SLAVE_ADDRESS); 
  Wire.write(reg);
  Wire.endTransmission(false);

  Wire.requestFrom(SLAVE_ADDRESS, 1, true);
  return Wire.read();
}

void ICM20648::writeRegister(uint8_t reg, uint8_t value){
  Wire.beginTransmission(SLAVE_ADDRESS);
  Wire.write(reg);
  Wire.write(value);
  Wire.endTransmission();
}
