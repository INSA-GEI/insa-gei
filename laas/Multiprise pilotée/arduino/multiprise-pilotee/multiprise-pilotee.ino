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

  /*io= new IOControl(myserial);
  if (io==NULL) {
    while(1);
  }*/

  // Wait until IO are correctly enabled
  while ((io=new IOControl(myserial))==NULL);

  // Wait until command manager is correctly setup
  while ((cmd = new Command(io)) == NULL);

  commandAvailable=false;

  myserial.println("Ready\n\r");
}

void loop() {
  delay(20);

  /* wait for a command */
  while (myserial.available()) {
    s.concat((char)myserial.read());
    //myserial.printf("length= %i (%02X)\n", s.length(), (int)s[s.length()-1]);

    // if cmd length is more then CMD_MAX_LENGTH, drop it
    if (s.length() > CMD_MAX_LENGTH) {
      s = "";
    } else {
      // if cmd ending char is present, command is correctly received 
      if (s.length()>0 && 
         ((s[s.length()-1]=='\n') || (s[s.length()-1]=='\r'))) {

        commandAvailable=true;
      }
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
