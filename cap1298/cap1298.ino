#include <SoftwareSerial.h>
#include <Wire.h>

#define PIN_ALERT 3

#define SLAVE_ADDRESS 0x28
#define REG_PRODUCT_ID 0xFD
#define REG_MANUFACTURE_ID 0xFE
#define REG_REVISION 0xFF
#define REG_MULTI 0x2A
#define REG_STANDBY_CONFIG 0x41
#define REG_INPUT_STATUS 0x03
#define REG_MAIN 0x00
#define BIT_INT 0b00000001

SoftwareSerial ds(4, 5); //RX, TX

uint8_t readRegister(uint8_t);
void writeRegister(uint8_t, uint8_t);
boolean initCAP1298();
uint8_t touched();
void callback_alert();

void setup() {
  ds.begin(9600);
  ds.println("Ready");
  Wire.begin();

  //attachInterrupt(digitalPinToInterrupt(PIN_ALERT), callback_alert, FALLING);
  
  if(!initCAP1298()){
    ds.println("Error:initialization");
    exit(0);
  }

 
}

void loop() {
  //ds.print("Touched: 0b");
  //ds.println(touched(), BIN);
  //ds.print("ALERT#: ");
 
  if(digitalRead(PIN_ALERT) == 0){
    ds.print("Touched: 0b");
    ds.println(touched(), BIN);
  }
  ds.print(".");
  delay(1000);
  
}

uint8_t readRegister(uint8_t reg){
  Wire.beginTransmission(SLAVE_ADDRESS); 
  Wire.write(reg);
  Wire.endTransmission(false);

  Wire.requestFrom(SLAVE_ADDRESS, 1, true);
  return Wire.read();
}

void writeRegister(uint8_t reg, uint8_t value){
  Wire.beginTransmission(SLAVE_ADDRESS);
  Wire.write(reg);
  Wire.write(value);
  Wire.endTransmission();
}

boolean initCAP1298(){
  
  uint8_t pid = readRegister(REG_PRODUCT_ID);
  uint8_t mid =readRegister(REG_MANUFACTURE_ID);
  uint8_t rev = readRegister(REG_REVISION);
  ds.print("Product ID: 0x");
  ds.println(pid, HEX);
  ds.print("Manufacture ID: 0x");
  ds.println(mid, HEX);
  ds.print("Revision : 0x");
  ds.println(rev, HEX);
  if(pid != 0x71 || mid != 0x5D || rev != 0x01 ){
    return false;
  }
  //Clear reset flag
  writeRegister(REG_MAIN, readRegister(REG_MAIN)&~BIT_INT);
  
  writeRegister(REG_MULTI, 0);
  writeRegister(REG_STANDBY_CONFIG, 0x30);

  return true;
}

uint8_t touched(){
  uint8_t t = readRegister(REG_INPUT_STATUS);
  //Clear touch flag
  if(t){
    writeRegister(REG_MAIN, readRegister(REG_MAIN)&~BIT_INT);
  }
  
  return t;
}

/*
void callback_alert(){
  noInterrupts();
  ds.println("interrupt");
  ds.print("Touched: 0b");
  ds.println(touched(), BIN);
  interrupts();
}
*/

