#ifndef ICM20648_H
#define ICM20648_H

#include "Arduino.h"
#include "Wire.h"

#define SLAVE_ADDRESS 0x68 //b1101000
#define BANK_0 (0 << 7)
#define BANK_1 (1 << 7)
#define BANK_2 (2 << 7)
#define BANK_3 (3 << 7)

#define REG_WHO_AM_I            (BANK_0 | 0x00)

#define REG_USER_CTRL           (BANK_0 | 0x03)
#define BIT_DMP_EN               0x80
#define BIT_FIFO_EN              0x40
#define BIT_I2C_MST_EN           0x20
#define BIT_I2C_IF_DIS           0x10
#define BIT_DMP_RST              0x08
#define BIT_DIAMOND_DMP_RST      0x04

#define REG_LP_CONFIG           (BANK_0 | 0x05) 
#define BIT_I2C_MST_CYCLE        0x40
#define BIT_ACCEL_CYCLE          0x20
#define BIT_GYRO_CYCLE           0x10


#define REG_PWR_MGMT_1          (BANK_0 | 0x06)

#define REG_I2C_MST_ODR_CONFIG  (BANK_3 | 0x00)
#define REG_I2C_MST_CTRL        (BANK_3 | 0x01)

class ICM20648{
  public:
    ICM20648();
    int init();
    uint8_t readRegister(uint8_t);
    void writeRegister(uint8_t, uint8_t);
    void setPowerMode(boolean);

  private:
    boolean _i2c;
};

#endif
