//#include "Common.h"
#include "SerialUSB.h"

#include "commands.h"
#include "io.h"
#include "config.h"

SerialUSB myserial;
String s;
bool commandAvailable;

IOControl *io;
Command *cmd;

void setup() {
  // put your setup code here, to run once:
  myserial.begin(9600);
  //myserial.ignoreFlowControl();

  io= new IOControl(myserial);
  if (io==NULL) {
    while(1);
  }

  cmd = new Command(io);

  commandAvailable=false;
}

void loop() {
  delay(20);

  /* wait for a command */
  while (myserial.available()) {
    s.concat((char)myserial.read());
    //myserial.printf("length= %i (%02X)\n", s.length(), (int)s[s.length()-1]);

    if ((s[s.length()-1]=='\n') || (s[s.length()-1]=='\r')) {
      commandAvailable=true;
    }
  }

  if (commandAvailable) {
    String ans;

    //myserial.print(s);
    //myserial.print('\n');
    
    ans = cmd->process(s);
 
    myserial.print(ans);
    myserial.print('\n');
    
    commandAvailable =false;
    s = "";
  }
}
