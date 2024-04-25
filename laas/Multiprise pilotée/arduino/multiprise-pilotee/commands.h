#ifndef __COMMAND_H__
#define __COMMAND_H__

#include <Arduino.h>
#include "io.h"

class Command {
public:
  Command(IOControl *io);

  String process(String s);
private:
  bool echoEnabled=false;
  IOControl *io;
};

#endif /* __COMMAND_H__ */