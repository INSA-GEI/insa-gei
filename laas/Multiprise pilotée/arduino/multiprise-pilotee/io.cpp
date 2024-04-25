#include "io.h"
#include <SPI.h>

#include "config.h"

SoftSPI::SoftSPI() {
}

SoftSPI::~SoftSPI() {
  this->end();
}

void SoftSPI::begin(void) {
  pinMode(DATAOUT, OUTPUT);
  pinMode(SPICLOCK, OUTPUT);

  digitalWrite(DATAOUT, LOW);
  digitalWrite(SPICLOCK, LOW);
}

void SoftSPI::end(void) {
  digitalWrite(DATAOUT, LOW);
  digitalWrite(SPICLOCK, LOW);

  pinMode(DATAOUT, INPUT);
  pinMode(SPICLOCK, INPUT);
}

void SoftSPI::setDebugSerial(SerialUSB &serial) {
  this->debugSerial = serial;
}

void SoftSPI::send(uint8_t data) {
  
  digitalWrite(DATAOUT, LOW);
  digitalWrite(SPICLOCK, LOW);

  //this->debugSerial.println("SoftSPI::Send: val=0x" + String(data,HEX));

  for (int i=7; i>=0; i--) {
    delay(1);
    
    if (data & (0x1<<i)) {
      digitalWrite(DATAOUT, HIGH);
      //this->debugSerial.println("SoftSPI::Send: bit " + String(i) + "=1");
    }
    else {
      digitalWrite(DATAOUT, LOW);
      //this->debugSerial.println("SoftSPI::Send: bit " + String(i) + "=0");
    }

    delay(1);

    //this->debugSerial.println("SoftSPI::Send: clock high");
    digitalWrite(SPICLOCK, HIGH);
    delay(1);

    //this->debugSerial.println("SoftSPI::Send: clock low");
    digitalWrite(SPICLOCK, LOW);
  }

  digitalWrite(DATAOUT, LOW);
}

IOControl::IOControl(SerialUSB &serial) {
  this->debugSerial = serial;

  pinMode(CS_RELAY, OUTPUT);
  pinMode(CS_PWR_DC, OUTPUT);
  pinMode(LATCH, OUTPUT);
  pinMode(RESET_IO, OUTPUT);
  pinMode(PWM_VAR_PWR, OUTPUT);

  digitalWrite(CS_RELAY, HIGH);
  digitalWrite(CS_PWR_DC, LOW);
  digitalWrite(LATCH, LOW);
  digitalWrite(RESET_IO, HIGH);
  digitalWrite(PWM_VAR_PWR, LOW);

  // this->myspi = &SPI;
  // this->myspi->setSCK(SPICLOCK);
  // this->myspi->setTX(DATAOUT);

  //SPI.setSCK(SPICLOCK);
  //SPI.setTX(DATAOUT);
  this->myspi.setDebugSerial(serial);
  this->myspi.begin();

//   this->myspi->begin();  //Enable SPI
}

IOControl::~IOControl() {
  this->myspi.end();  //Disable SPI
}

void IOControl::writeRelay(uint8_t val) {
  uint8_t data;

  //this->debugSerial.println("writeRelay param = 0x" + String(val, HEX));
  data = val>>1;

  //this->debugSerial.println("writeRelay = 0x" + String(data, HEX));

  digitalWrite(CS_RELAY, LOW);  //Assert CS_RELAY
  delay(1);

  this->myspi.send(data);

  delay(1);
  digitalWrite(CS_RELAY, HIGH);  //Desassert CS_RELAY
}

void IOControl::writeDC(uint8_t val_3V, uint8_t val_5V, VPOLValue_Typedef vpol) {
  uint8_t data;

  //this->debugSerial.println("WriteDC params: 0x" + String(val_3V, HEX) + "/0x" + String(val_5V, HEX) + "/0x" + String(vpol, HEX));

  data = ((val_5V>>1)<<3) | (val_3V>>1);

  if (vpol == vpolPositive) 
    data= (0xA<<4) | data;
  else if (vpol == vpolNegative) 
    data= (0x5<<4) | data;
  else
    data= 0x3<<4 | data;
     
  //this->debugSerial.println("WriteDC = 0x" + String(data, HEX));

  digitalWrite(CS_PWR_DC, HIGH);  //Assert CS_PWR_DC
  delay(1);

  this->myspi.send(data);

  delay(1);
  digitalWrite(CS_PWR_DC, LOW);  //Desassert CS_PWR_DC
  delay(1);
  digitalWrite(LATCH, HIGH);
  delay(1);
  digitalWrite(LATCH, LOW);
}

void IOControl::writeVARDC(uint8_t val) {
  uint8_t data=val;

  //TODO
  //this->debugSerial.println("writeVARDC = " + String(data));
}

bool IOControl::resetAll(void) {
  int val = 0;

  if (this->clearACChannel(0) != true)
    val++;

  // optimisation for juste one call to writeDC (for resetting dc3v dc5v and VPOL, all on the same SPI)
  this->statusDC3V=0;
  this->statusDC5V=0;
  this->statusVPOL=vpolOff;

  this->writeDC(this->statusDC3V, this->statusDC5V, this->statusVPOL);

  if (this->setVARChannel(0.0) != true)
    val++;

  if (val)
    return false;
  else
    return true;
}

bool IOControl::setACChannel(uint8_t channel) {
  bool status = true;

  if (channel == 0) {
    this->statusAC = 0x2 + 0x4 + 0x8 + 0x10;
    
    this->writeRelay(this->statusAC);
  } else {
    if ((channel < 1) || (channel > AC_CHANNEL_MAX))
      status = false;
    else {
      this->statusAC |= (1 << channel);

      this->writeRelay(this->statusAC);
    }
  }

  return status;
}

bool IOControl::clearACChannel(uint8_t channel) {
  bool status = true;

  if (channel == 0) {
    this->statusAC = 0;

    this->writeRelay(this->statusAC);
  } else {
    if ((channel < 1) || (channel > AC_CHANNEL_MAX))
      status = false;
    else {
      this->statusAC &= ~(1 << channel);

      this->writeRelay(this->statusAC);
    }
  }

  return status;
}

bool IOControl::setDC3VChannel(uint8_t channel) {
  bool status = true;

  if (channel == 0) {
    this->statusDC3V = 0x2 + 0x4 + 0x8;

    this->writeDC(this->statusDC3V, this->statusDC5V, this->statusVPOL);
  } else {
    if ((channel < 1) || (channel > DC3V_CHANNEL_MAX))
      status = false;
    else {
      this->statusDC3V |= (1 << channel);

      this->writeDC(this->statusDC3V, this->statusDC5V, this->statusVPOL);
    }
  }

  return status;
}

bool IOControl::clearDC3VChannel(uint8_t channel) {
  bool status = true;

  if (channel == 0) {
    this->statusDC3V = 0;

    this->writeDC(this->statusDC3V, this->statusDC5V, this->statusVPOL);
  } else {
    if ((channel < 1) || (channel > DC3V_CHANNEL_MAX))
      status = false;
    else {
      this->statusDC3V &= ~(1 << channel);

      this->writeDC(this->statusDC3V, this->statusDC5V, this->statusVPOL);
    }
  }

  return status;
}

bool IOControl::setDC5VChannel(uint8_t channel) {
  bool status = true;

  if (channel == 0) {
    this->statusDC5V = 0x2;

    this->writeDC(this->statusDC3V, this->statusDC5V, this->statusVPOL);
  } else {
    if ((channel < 1) || (channel > DC5V_CHANNEL_MAX))
      status = false;
    else {
      this->statusDC5V |= (1 << channel);

      this->writeDC(this->statusDC3V, this->statusDC5V, this->statusVPOL);
    }
  }

  return status;
}

bool IOControl::clearDC5VChannel(uint8_t channel) {
  bool status = true;

  if (channel == 0) {
    this->statusDC5V = 0;

    this->writeDC(this->statusDC3V, this->statusDC5V, this->statusVPOL);
  } else {
    if ((channel < 1) || (channel > DC5V_CHANNEL_MAX))
      status = false;
    else {
      this->statusDC5V &= ~(1 << channel);

      this->writeDC(this->statusDC3V, this->statusDC5V, this->statusVPOL);
    }
  }

  return status;
}

bool IOControl::setVPOLChannel(VPOLValue_Typedef val) {
  this->statusVPOL = val;

  this->writeDC(this->statusDC3V, this->statusDC5V, this->statusVPOL);

  return true;
}

bool IOControl::setVARChannel(float val) {
  this->statusVAR = val;

  this->writeVARDC((uint8_t)(this->statusVAR*10));

  return true;
}