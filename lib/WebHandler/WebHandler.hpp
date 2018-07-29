#ifndef __WEB_HANDLER_HPP__
#define __WEB_HANDLER_HPP__

class WebHandler
{
private:
  bool initialized;
  void setup();

public:
  WebHandler();

  void handle();
};

extern WebHandler webHandler;

#endif
