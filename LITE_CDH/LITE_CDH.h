#ifndef MBED_LITECDH_H
#define MBED_LITECDH_H
#include "SDFileSystem.h"
#include "mbed.h"

class LITE_CDH : public SDFileSystem{
public:
    
    LITE_CDH(PinName mosi, PinName miso, PinName sclk, PinName cs, const char* name);
  
private:  
};

#endif