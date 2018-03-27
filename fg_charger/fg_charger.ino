/*
 Charger: BQ24295
 Fuel Gauge: LC709203F
*/

#include <Wire.h>

//Charger
#define CHG_SLAVE_ADDRESS 0x6B

#define CHG_REG_ISCR  0x00 //Input Source Control Register
#define CHG_REG_POCR  0x01 //Power-On Configuration Register
#define CHG_REG_CCCR  0x02 //Charge Current Control Register
#define CHG_REG_PTCCR 0x03 //Pre-charge/Termination Current Control Register
#define CHG_REG_CVCR  0x04 //Charge Voltage Conrtol Register
#define CHG_REG_CTCR  0x05 //Charge termination/Timer Control Register
#define CHG_REG_BTRCR 0x06 //Boost voltage/Thermal Regulation Control Register
#define CHG_REG_MOCR  0x07 //Misc Operation Control Register
#define CHG_REG_SSR   0x08 //System Status Register
#define CHG_REG_NFR   0x09 //New Fault Register
#define CHG_REG_VPRSR 0x0A //Vender/Part/Revision Status Register

//Fuel Gauge
#define FG_SLAVE_ADDRESS 0x0B

#define FG_REG_BRSOC  0x04 //Before RSOC
#define FG_REG_TB     0x06 //Thermistor B
#define FG_REG_IRSOC  0x07 //Initial RSOC
#define FG_REG_CT     0x08 //Cell Temperature
#define FG_REG_CV     0x09 //Cell Voltage
#define FG_REG_CD     0x0A //Current Direction
#define FG_REG_APA    0x0B //Adjustment Pack Application
#define FG_REG_APT    0x0C //Adjustment Pack Thermistor
#define FG_REG_RSOC   0x0D //RSOC
#define FG_REG_ITE    0x0F //Indicator to Empty
#define FG_REG_ICV    0x11 //IC Version
#define FG_REG_COTP   0x12 //Change Of The Parameter
#define FG_REG_ALRSOC 0x13 //Alarm Low RSOC
#define FG_REG_ALCV   0x14 //Alarm Low Cell Voltage
#define FG_REG_ICPM   0x15 //IC Power Mode
#define FG_REG_SB     0x16 //Status Bit
#define FG_REG_NOTP   0x1A //Number of The Parameter

//CRC8-ATM
#define CRC_BIT_LENGTH 8
#define CRC8_ATM 0x07

//Prototype
uint8_t readRegister(int, uint8_t);
void writeRegister(int, uint8_t, uint8_t);
boolean readWordRegister(int, uint8_t, uint16_t*);
uint8_t getCRC8(const void *, size_t size );

boolean initBQ24295();
boolean initLC709203F();

//Global
uint8_t buff[5];

void setup() {
  Serial.begin(9600);
  Serial.println("Ready");
  Wire.begin();

  if(!initBQ24295()){
    Serial.println("Error:initialization");
    exit(0);
  }
  
  initLC709203F();
 
}

void loop() {

  
}

uint8_t readRegister(int address, uint8_t reg){
  Wire.beginTransmission(address); 
  Wire.write(reg);
  Wire.endTransmission(false);

  Wire.requestFrom(address, 1, true);
  return Wire.read();
}

void writeRegister(int address, uint8_t reg, uint8_t val){
  Wire.beginTransmission(address);
  Wire.write(reg);
  Wire.write(val);
  Wire.endTransmission();
}

boolean readWordRegister(int address, uint8_t reg, uint16_t *val){
  //uint16_t val = 0;
  uint8_t crc = 0;
  Wire.beginTransmission(address); 
  Wire.write(reg);
  Wire.endTransmission(false);

  Wire.requestFrom(address, 3, true);

  buff[0] = (address << 1) | 0x00; //Slave Address & Write
  buff[1] = reg; // Register
  buff[2] = (address << 1) | 0x01; //Slave Address & Read
  buff[3] = Wire.read();
  buff[4] = Wire.read();
  crc = Wire.read();
  
  if(crc != getCRC8(buff, 5)) return false;
  *val = buff[3] | (buff[4] << 8);

  return true;
}

void writeWordRegister(int address, uint8_t reg, uint16_t val){

  uint8_t crc = 0;

  buff[0] = (address << 1) | 0x00; //Slave Address & Write
  buff[1] = reg; // Register
  buff[3] = (uint8_t) (0xFF & val);
  buff[4] = (uint8_t) (0xFF & (val >> 8));
  crc = getCRC8(buff, 4);
  
  Wire.beginTransmission(address); 
  Wire.write(reg);
  Wire.write(buff[3]);
  Wire.write(buff[4]);
  Wire.write(crc);
  Wire.endTransmission();
  
}



boolean initBQ24295(){
  Serial.println("Initialize Charger");
  
  uint8_t vpr = readRegister(CHG_SLAVE_ADDRESS, CHG_REG_VPRSR);
  Serial.println(vpr, HEX);
  if(vpr != 0xC0 ){
    return false;
  }
  
  writeRegister(CHG_SLAVE_ADDRESS, CHG_REG_ISCR, 0xD8);
  Serial.print("REG00: ");
  Serial.println(readRegister(CHG_SLAVE_ADDRESS, CHG_REG_ISCR), HEX);

  writeRegister(CHG_SLAVE_ADDRESS, CHG_REG_POCR, 0x11);
  Serial.print("REG01: ");
  Serial.println(readRegister(CHG_SLAVE_ADDRESS, CHG_REG_POCR), HEX);

  writeRegister(CHG_SLAVE_ADDRESS, CHG_REG_CCCR, 0x60);
  Serial.print("REG02: ");
  Serial.println(readRegister(CHG_SLAVE_ADDRESS, CHG_REG_CCCR), HEX);

  writeRegister(CHG_SLAVE_ADDRESS, CHG_REG_PTCCR, 0x11);
  Serial.print("REG03: ");
  Serial.println(readRegister(CHG_SLAVE_ADDRESS, CHG_REG_PTCCR), HEX);
  
  writeRegister(CHG_SLAVE_ADDRESS, CHG_REG_CVCR, 0xB2);
  Serial.print("REG04: ");
  Serial.println(readRegister(CHG_SLAVE_ADDRESS, CHG_REG_CVCR), HEX);

  writeRegister(CHG_SLAVE_ADDRESS, CHG_REG_CTCR, 0x9C);
  Serial.print("REG05: ");
  Serial.println(readRegister(CHG_SLAVE_ADDRESS, CHG_REG_CTCR), HEX);

  writeRegister(CHG_SLAVE_ADDRESS, CHG_REG_BTRCR, 0x93);
  Serial.print("REG06: ");
  Serial.println(readRegister(CHG_SLAVE_ADDRESS, CHG_REG_BTRCR), HEX);

  writeRegister(CHG_SLAVE_ADDRESS, CHG_REG_MOCR, 0x4B);
  Serial.print("REG07: ");
  Serial.println(readRegister(CHG_SLAVE_ADDRESS, CHG_REG_MOCR), HEX);
  
  return true;
}

boolean initLC709203F(){
  uint16_t val;
  Serial.println("Initialize Fuel Gauge");

  if(readWordRegister(FG_SLAVE_ADDRESS, FG_REG_NOTP, &val)){
    Serial.print("Number of The Parameter: "); 
    Serial.println(val, HEX);
  }

  writeWordRegister(FG_SLAVE_ADDRESS, FG_REG_APA, 0x2D);
  if(readWordRegister(FG_SLAVE_ADDRESS, FG_REG_APA, &val)){
    Serial.print("APA: "); 
    Serial.println(val, HEX);
  }
  
  return true;
}

uint8_t getCRC8( const void *buff, size_t size ){
    uint8_t *p = (unsigned char *)buff;
    uint8_t crc8;
    
    for ( crc8 = 0x00 ; size != 0 ; size-- ){
        crc8 ^= *p++;
        
        for ( int i = 0 ; i < CRC_BIT_LENGTH ; i++ ){
            if ( crc8 & 0x80 ){
                crc8 <<= 1; crc8 ^= CRC8_ATM;
            }
            else{
                crc8 <<= 1;
            }
        }
    }
    return crc8;
}
