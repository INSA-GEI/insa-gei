#include "commands.h"
#include "config.h"
#include "io.h"

#define CMD_AT      "AT"

#define CMD_ATE     "ATE="
#define REQ_ATE     "ATE?"

#define CMD_ATR     "ATR"

#define CMD_ATAC    "ATAC="
#define REQ_ATAC    "ATAC?"
#define CMD_ATACx   "ATAC"

#define CMD_AT3V    "AT3V="
#define REQ_AT3V    "AT3V?"
#define CMD_AT3Vx   "AT3V"

#define CMD_AT5V    "AT5V="
#define REQ_AT5V    "AT5V?"
#define CMD_AT5Vx   "AT5V"

#define CMD_ATVAR   "ATVAR="
#define REQ_ATVAR   "ATVAR?"

#define CMD_ATVPOL  "ATVPOL="
#define REQ_ATVPOL  "ATVPOL?"

#define REQ_ATVER   "ATVER?"

#define ANS_OK      "OK"
#define ANS_ERR     "ERROR"

#define CHAR_TO_ASCII(x)  (x+0x30)
#define ASCII_TO_CHAR(x)  (x-0x30)

typedef enum {
  noError=0,
  unknownCommand,
  invalidParameter,
  hardwareError,
  debugError
} ErrorStatus_Typedef;

Command::Command(IOControl *io) {
  this->io = io;
}

String Command::process(String s) {
  String ans=ANS_OK;
  ErrorStatus_Typedef errorStatus=noError;

  /* process incoming string for command or request */
  
  s.toUpperCase(); // put string in upper case

  /*
   * Command ATE -> set echo flag
   */
  if (s.indexOf(CMD_ATE) == 0) {    
    if (s[strlen(CMD_ATE)]=='1') {  // if parameter is 1 => echo on
      this->echoEnabled=true;
    } else if (s[strlen(CMD_ATE)]=='0') { // if parameter is 0 => echo off
      this->echoEnabled=false;
    } else { // invalid param
      ans = ANS_ERR;
      errorStatus = invalidParameter;
    }
  } 
  
  /*
   * Request ATE? -> get current echo flag
   */
  else if (s.indexOf(REQ_ATE)==0) {
    if (this->echoEnabled)
      ans= String("1");
    else
      ans= String("0");
  } 
  
  /*
   * Command ATR -> Reset all output
   */
  else if (s.indexOf(CMD_ATR) == 0) {
    if (this->io->resetAll() != true) {  // reset all channels and raise an error if it fail
      ans = ANS_ERR;
      errorStatus = hardwareError;
    }
  } 
  
  /*
   * Request ATVER? -> get version
   */
  else if (s.indexOf(REQ_ATVER)==0) {
    ans = String (String(MAJOR_VER) + "." + String(MINOR_VER));
  } 
  
  /*
   * Command ATAC= -> Set AC outputs all on or off
   */
  else if (s.indexOf(CMD_ATAC) == 0) {
    if (s[strlen(CMD_ATAC)]=='0') {
      if (this->io->clearACChannel(0) != true) { // channel parameter is equal to 0 -> reset all channels
        ans = ANS_ERR;
        errorStatus = hardwareError;
      } 
    } else if (s[strlen(CMD_ATAC)]=='1') {
      if (this->io->setACChannel(0) != true) { // channel parameter is equal to 0 -> set all channels
        ans = ANS_ERR;
        errorStatus = hardwareError;
      } 
    } else { // Invalid parameter 
      ans= ANS_ERR;
      errorStatus = invalidParameter;
    }
  } 
  
  /*
   * Request ATAC? -> get AC status
   */
  else if (s.indexOf(REQ_ATAC) == 0) {
    ans = String((int)this->io->getACAllChannels());
  } 

  /*
   * Command ATACx= -> Set AC outputs all on or off
   *
   * Command format is ATAC(ch)=(val)
   * Ex: ATAC1=0  (clear AC channel 1)
   *
   * For parsing: pos 4 is channel and pos 6 is value
   */
  else if (s.indexOf(CMD_ATACx) == 0) {
    char channel = s[4];  // pos 4 = channel number (1 to AC_CHANNEL_MAX)
    char value = s[6];    // pos 6 = value (0 or 1)

    if ((channel<CHAR_TO_ASCII(1)) || (channel>CHAR_TO_ASCII(AC_CHANNEL_MAX))) { // channel not in ascii range [1..AC_CHANNEL_MAX]
      ans= ANS_ERR;
      errorStatus=invalidParameter;
    }
    else {
      channel = ASCII_TO_CHAR(channel); // set channel in integer range [1..4] not ascii range ['1'..'4']

      if (value == '0') {
        if (this->io->clearACChannel(channel) != true) {
          ans= ANS_ERR;
          errorStatus=hardwareError;
        }
      } else if (value == '1') {
        if (this->io->setACChannel(channel) != true) {
          ans= ANS_ERR;
          errorStatus=hardwareError;
        }
      } else { // Value not in range ['0'..'1']
        ans= ANS_ERR;
        errorStatus=invalidParameter;
      }
    }
  }

  /*
   * Command AT3V= -> Set 3V outputs all on or off
   */
  else if (s.indexOf(CMD_AT3V) == 0) {
    if (s[strlen(CMD_AT3V)]=='0') {
      if (this->io->clearDC3VChannel(0) != true) { // channel parameter is equal to 0 -> reset all channels
        ans = ANS_ERR;
        errorStatus = hardwareError;
      } 
    } else if (s[strlen(CMD_AT3V)]=='1') {
      if (this->io->setDC3VChannel(0) != true) { // channel parameter is equal to 0 -> set all channels
        ans = ANS_ERR;
        errorStatus = hardwareError;
      } 
    } else { // Invalid parameter 
      ans= ANS_ERR;
      errorStatus = invalidParameter;
    }
  } 
  
  /*
   * Request AT3V? -> get DC 3V status
   */
  else if (s.indexOf(REQ_AT3V) == 0) {
    ans = String((int)this->io->getDC3VAllChannels());
  } 

  /*
   * Command AT3Vx= -> Set 3V single output on or off
   *
   * Command format is AT3V(ch)=(val)
   * Ex: AT3V1=0  (clear DC 3V channel 1)
   *
   * For parsing: pos 4 is channel and pos 6 is value
   */
  else if (s.indexOf(CMD_AT3Vx) == 0) {
    char channel = s[4];  // pos 4 = channel number (1 to DC3V_CHANNEL_MAX)
    char value = s[6];    // pos 6 = value (0 or 1)

    if ((channel<CHAR_TO_ASCII(1)) || (channel>CHAR_TO_ASCII(DC3V_CHANNEL_MAX))) { // channel not in ascii range [1..DC3V_CHANNEL_MAX]
      ans= ANS_ERR;
      errorStatus=invalidParameter;
    }
    else {
      channel = ASCII_TO_CHAR(channel); // set channel in integer range [1..3] not ascii range ['1'..'3']

      if (value == '0') {
        if (this->io->clearDC3VChannel(channel) != true) {
          ans= ANS_ERR;
          errorStatus=hardwareError;
        }
      } else if (value == '1') {
        if (this->io->setDC3VChannel(channel) != true) {
          ans= ANS_ERR;
          errorStatus=hardwareError;
        }
      } else { // Value not in range ['0'..'1']
        ans= ANS_ERR;
        errorStatus=invalidParameter;
      }
    }
  }
  
  /*
   * Command AT5V= -> Set 5V outputs all on or off
   */
  else if (s.indexOf(CMD_AT5V) == 0) {
    if (s[strlen(CMD_AT5V)]=='0') {
      if (this->io->clearDC5VChannel(0) != true) { // channel parameter is equal to 0 -> reset all channels
        ans = ANS_ERR;
        errorStatus = hardwareError;
      } 
    } else if (s[strlen(CMD_AT5V)]=='1') {
      if (this->io->setDC5VChannel(0) != true) { // channel parameter is equal to 0 -> set all channels
        ans = ANS_ERR;
        errorStatus = hardwareError;
      } 
    } else { // Invalid parameter 
      ans= ANS_ERR;
      errorStatus = invalidParameter;
    }
  } 
  
  /*
   * Request AT5V? -> get DC 5V status
   */
  else if (s.indexOf(REQ_AT5V) == 0) {
    ans = String((int)this->io->getDC5VAllChannels());
  } 

  /*
   * Command AT5Vx= -> Set 5V single output on or off
   *
   * Command format is AT5V(ch)=(val)
   * Ex: AT5V1=0  (clear DC 5V channel 1)
   *
   * For parsing: pos 4 is channel and pos 6 is value
   */
  else if (s.indexOf(CMD_AT5Vx) == 0) {
    char channel = s[4];  // pos 4 = channel number (1 to DC3V_CHANNEL_MAX)
    char value = s[6];    // pos 6 = value (0 or 1)

    if ((channel<CHAR_TO_ASCII(1)) || (channel>CHAR_TO_ASCII(DC5V_CHANNEL_MAX))) { // channel not in ascii range [1..DC5V_CHANNEL_MAX]
      ans= ANS_ERR;
      errorStatus=invalidParameter;
    }
    else {
      channel = ASCII_TO_CHAR(channel); // set channel in integer range [1..1] not ascii range ['1'..'1']

      if (value == '0') {
        if (this->io->clearDC5VChannel(channel) != true) {
          ans= ANS_ERR;
          errorStatus=hardwareError;
        }
      } else if (value == '1') {
        if (this->io->setDC5VChannel(channel) != true) {
          ans= ANS_ERR;
          errorStatus=hardwareError;
        }
      } else { // Value not in range ['0'..'1']
        ans= ANS_ERR;
        errorStatus=invalidParameter;
      }
    }
  }

  /*
   * Command ATVPOL= -> Set VPOL output Vpos, Vneg or off
   */
  else if (s.indexOf(CMD_ATVPOL) == 0) {
    if (s[strlen(CMD_ATVPOL)]=='+') {
      if (this->io->setVPOLChannel(IOControl::vpolPositive) != true) { // parameter is equal to + -> set positive 5V
        ans = ANS_ERR;
        errorStatus = hardwareError;
      } 
    } else if (s[strlen(CMD_ATVPOL)]=='-') {
      if (this->io->setVPOLChannel(IOControl::vpolNegative) != true) { // parameter is equal to - -> set negative 5V
        ans = ANS_ERR;
        errorStatus = hardwareError;
      } 
    } else if (s[strlen(CMD_ATVPOL)]=='0') {
      if (this->io->setVPOLChannel(IOControl::vpolOff) != true) { // parameter is equal to 0 -> set VPOL off
        ans = ANS_ERR;
        errorStatus = hardwareError;
      } 
    } else { // Invalid parameter 
      ans= ANS_ERR;
      errorStatus = invalidParameter;
    }
  } 
  
  /*
   * Request ATVPOL? -> get VPOL status
   */
  else if (s.indexOf(REQ_ATVPOL) == 0) {
    IOControl::VPOLValue_Typedef val = this->io->getVPOLChannel();

    if (val == IOControl::vpolPositive)
      ans = String("VPOS");
    else if (val == IOControl::vpolNegative)
      ans = String("VNEG");
    else 
      ans = String("OFF");
  } 

  /*
   * Command CMD_ATVAR= -> Set VAR output 
   */
  else if (s.indexOf(CMD_ATVAR) == 0) {
    float value;
    s.remove(0,strlen(CMD_ATVAR)); // remove leading command

    value = s.toFloat();

    if ((value!=0.0) &&
        ((value<1.5) || (value>5.0))) {
      ans= ANS_ERR;
      errorStatus = invalidParameter;
    }
    else if (this->io->setVARChannel(value) != true) {
      ans= ANS_ERR;
      errorStatus = hardwareError;
    }
  } 

  /*
   * Request ATVAR? -> get VAR status
   */
  else if (s.indexOf(REQ_ATVAR) == 0) {
    float val = this->io->getVARChannel();

    ans = String(val);
  } 

  /*
   * Command AT -> simple ping
   */
  else if (s.indexOf(CMD_AT)==0) {
    if ((s[2]!='\n') && (s[2]!='\r') && (s[2]!=' ')) {
      ans = ANS_ERR;
      errorStatus = unknownCommand;
    }
  } 
  
  /*
   * Unknown command
   */
  else {
    ans = ANS_ERR;
    errorStatus = unknownCommand;
  }

  /* 
   * If answer to received command or request is ERROR, add error code
   */
  if (ans == ANS_ERR) {
    ans.concat(" (" + String((int)errorStatus) + ')');
  }

  if (this->echoEnabled) {
    ans = s+ans;
  }

  return ans;
}