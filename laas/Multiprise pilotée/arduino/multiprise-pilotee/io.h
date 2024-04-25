#ifndef __IO_H__
#define __IO_H__

#include <Arduino.h>
#include <SPI.h>
#include "SerialUSB.h"

class SoftSPI {
public:
  SoftSPI();
  ~SoftSPI();

  void begin(void);
  void end(void);
  
  void setDebugSerial(SerialUSB &serial);

  void send(uint8_t data);
private:
  SerialUSB debugSerial;
};

class IOControl {
public:
  IOControl(SerialUSB &serial);
  ~IOControl();

  bool resetAll(void);
  
  bool setACChannel(uint8_t channel);   // set individual channel (1 to 4) or all channels if channel=0
  bool clearACChannel(uint8_t channel); // set individual channel (1 to 4) or all channels if channel=0

  bool setDC3VChannel(uint8_t channel); // set individual channel (1 to 4) or all channels if channel=0
  bool clearDC3VChannel(uint8_t channel); // set individual channel (1 to 4) or all channels if channel=0

  bool setDC5VChannel(uint8_t channel); // set individual channel (1 to 4) or all channels if channel=0
  bool clearDC5VChannel(uint8_t channel); // set individual channel (1 to 4) or all channels if channel=0

  typedef enum {
    vpolOff=0,
    vpolPositive,
    vpolNegative
  } VPOLValue_Typedef;

  bool setVPOLChannel(VPOLValue_Typedef val);

  bool setVARChannel(float val);

  bool getACChannel(uint8_t channel) {
    if ((channel>=1) && (channel<=4))
      return (this->statusAC&(1<<channel));
    else
      return false;
  }

  bool getDC3VChannel(uint8_t channel) {
    if ((channel>=1) && (channel<=3))
      return (this->statusDC3V&(1<<channel));
    else
      return false;
  } 

  bool getDC5VChannel(uint8_t channel) {
    if (channel==1)
      return (this->statusDC5V&(1<<channel));
    else
      return false;
  } 

  uint8_t getACAllChannels(void) {
    return this->statusAC;
  }

  uint8_t getDC3VAllChannels(void) {
    return this->statusDC3V;
  }

  uint8_t getDC5VAllChannels(void) {
    return this->statusDC5V;
  }

  VPOLValue_Typedef getVPOLChannel(void) {
    return this->statusVPOL;
  } 

  float getVARChannel(void) {
    return this->statusVAR;
  }

private:
  uint8_t statusAC=0;
  uint8_t statusDC3V=0;
  uint8_t statusDC5V=0;
  VPOLValue_Typedef statusVPOL=vpolOff;
  float statusVAR=0.0;

  SerialUSB debugSerial;
  SoftSPI   myspi;

  void writeRelay(uint8_t val);
  void writeDC(uint8_t val_3V, uint8_t val_5V, VPOLValue_Typedef vpol);
  void writeVARDC(uint8_t val);
};

#endif /* __IO_H__ */