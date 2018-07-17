#include "ICM20648.h"

ICM20648::ICM20648(){
  _i2c = true;
}

int ICM20648::init(){
  
  uint8_t did = readRegister(REG_WHO_AM_I);
  Serial.print("Device ID: 0x");
  Serial.println(did, HEX);

  setPowerMode(true);
  
  uint8_t rslt = readRegister(REG_LP_CONFIG);
  Serial.println(rslt, HEX);
  
  return 1;  

}

void ICM20648::setPowerMode(boolean lpmode){
  //lpmode => ture, low power mode
  //lpmode => false, high powerformance (low noise) mode
  uint8_t data;
  
  if(lpmode){
    data = BIT_I2C_MST_CYCLE | BIT_ACCEL_CYCLE | BIT_GYRO_CYCLE;
  }else{
    data = BIT_I2C_MST_CYCLE;
  }
  writeRegister(REG_LP_CONFIG, data);
  
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
