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
    delay(5);
    
    if (data & (0x1<<i)) {
      digitalWrite(DATAOUT, HIGH);
      //this->debugSerial.println("SoftSPI::Send: bit " + String(i) + "=1");
    }
    else {
      digitalWrite(DATAOUT, LOW);
      //this->debugSerial.println("SoftSPI::Send: bit " + String(i) + "=0");
    }

    delay(5);

    //this->debugSerial.println("SoftSPI::Send: clock high");
    digitalWrite(SPICLOCK, HIGH);
    delay(5);

    //this->debugSerial.println("SoftSPI::Send: clock low");
    digitalWrite(SPICLOCK, LOW);
  }

  delay(5);
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
  const uint pwm_pin = 6; 
  gpio_set_function(pwm_pin, GPIO_FUNC_PWM); // fonction PWM sur GP6   
  this->slice = pwm_gpio_to_slice_num(pwm_pin); // slice associée à GP6   
  this->channel = pwm_gpio_to_channel(pwm_pin);  // channel associée à GP6 
  
  pwm_set_phase_correct (this->slice, false); // mode phase-correct non activé 
  
  pwm_set_clkdiv_int_frac (this->slice, 1, 0); // diviseur de fréquence = 38 + 3/16 
  pwm_set_wrap(this->slice, 10000);  // valeur wrap pour fixer la fréquence 
  pwm_set_chan_level(this->slice, this->channel, 10000 / 10); // rapport cyclique = 10% pour servo à 180° 
  
  pwm_set_enabled(this->slice, true); // activer le signal 
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
  delay(10);

  this->myspi.send(data);

  delay(10);
  digitalWrite(CS_PWR_DC, LOW);  //Desassert CS_PWR_DC
  delay(10);
  digitalWrite(LATCH, HIGH);
  delay(10);
  digitalWrite(LATCH, LOW);
}

void IOControl::writeVARDC(uint8_t val) {
  uint16_t duty;
  const uint16_t Computed_Duties[] = 
  {
    1280, 1300, 1320, 1335, 1350,                                 // 1.5V -> 1.9V
    1370, 1385, 1400, 1420, 1440, 1460, 1480, 1510, 1540, 1580,   // 2.0V -> 2.9V
    1620, 1670, 1730, 1810, 1900, 2000, 2140, 2320, 2520, 2860,   // 3.0V -> 3.9V
    3400, 3980, 4720, 5400, 6200, 6900, 7560, 8380, 9300, 10000,  // 4.0V -> 4.9V
    10000                                                         // 5.0V
  };
  
  //this->debugSerial.println("writeVARDC = " + String(val));

  if (val == 0)
    duty=0;
  else
    duty = Computed_Duties[val-15];
  
  //this->debugSerial.println("duty = " + String(duty));

  pwm_set_chan_level(this->slice, this->channel, duty);
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