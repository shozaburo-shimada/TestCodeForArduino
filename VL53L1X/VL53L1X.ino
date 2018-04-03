#include <Wire.h>

#define SLAVE_ADDRESS 0x29

void writeReg(uint8_t reg, uint8_t value);
void writeReg16Bit(uint8_t reg, uint16_t value);
void writeReg32Bit(uint8_t reg, uint32_t value);
uint8_t readReg(uint8_t reg);
uint8_t readReg2(uint8_t reg);
uint16_t readReg16Bit(uint8_t reg);
uint32_t readReg32Bit(uint8_t reg);


void setup() {
  Serial.begin(9600);
  Serial.println("Ready");

  Wire.begin();
}

void loop() {
  for(uint8_t i = 0; i < 0xFF; i++){
    uint8_t val = readReg2(i);
    Serial.print(i);
    Serial.print(": ");
    Serial.print(val);
    Serial.println();
    delay(100);
  }
}

uint8_t last_status;

// Write an 8-bit register
void writeReg(uint8_t reg, uint8_t value){
  Wire.beginTransmission(SLAVE_ADDRESS);
  Wire.write(reg);
  Wire.write(value);
  last_status = Wire.endTransmission();
}

// Write a 16-bit register
void writeReg16Bit(uint8_t reg, uint16_t value){
  Wire.beginTransmission(SLAVE_ADDRESS);
  Wire.write(reg);
  Wire.write((value >> 8) & 0xFF); // value high byte
  Wire.write( value       & 0xFF); // value low byte
  last_status = Wire.endTransmission();
}

// Write a 32-bit register
void writeReg32Bit(uint8_t reg, uint32_t value){
  Wire.beginTransmission(SLAVE_ADDRESS);
  Wire.write(reg);
  Wire.write((value >> 24) & 0xFF); // value highest byte
  Wire.write((value >> 16) & 0xFF);
  Wire.write((value >>  8) & 0xFF);
  Wire.write( value        & 0xFF); // value lowest byte
  last_status = Wire.endTransmission();
}

// Read an 8-bit register
uint8_t readReg(uint8_t reg){
  uint8_t value;

  Wire.beginTransmission(SLAVE_ADDRESS);
  Wire.write(reg);
  last_status = Wire.endTransmission();

  Wire.requestFrom(SLAVE_ADDRESS, (uint8_t)1);
  value = Wire.read();

  return value;
}

// Read an 8-bit register
uint8_t readReg2(uint8_t reg){
  uint8_t value;

  Wire.beginTransmission(SLAVE_ADDRESS);
  Wire.write(0x00);
  Wire.write(reg);
  last_status = Wire.endTransmission();

  Wire.requestFrom(SLAVE_ADDRESS, (uint8_t)1);
  value = Wire.read();

  return value;
}

// Read a 16-bit register
uint16_t readReg16Bit(uint8_t reg){
  uint16_t value;

  Wire.beginTransmission(SLAVE_ADDRESS);
  Wire.write(reg);
  last_status = Wire.endTransmission();

  Wire.requestFrom(SLAVE_ADDRESS, (uint8_t)2);
  value  = (uint16_t)Wire.read() << 8; // value high byte
  value |=           Wire.read();      // value low byte

  return value;
}

// Read a 32-bit register
uint32_t readReg32Bit(uint8_t reg){
  uint32_t value;

  Wire.beginTransmission(SLAVE_ADDRESS);
  Wire.write(reg);
  last_status = Wire.endTransmission();

  Wire.requestFrom(SLAVE_ADDRESS, (uint8_t)4);
  value  = (uint32_t)Wire.read() << 24; // value highest byte
  value |= (uint32_t)Wire.read() << 16;
  value |= (uint16_t)Wire.read() <<  8;
  value |=           Wire.read();       // value lowest byte

  return value;
}

