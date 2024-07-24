#ifndef MBED_LITECDH_H
#define MBED_LITECDH_H
#include "SDFileSystem.h"
#include "LiteAnalogSW.h"
#include "mbed.h"

class LITE_CDH : public SDFileSystem, public LiteAnalogSW{
public:
    
    LITE_CDH(PinName mosi, PinName miso, PinName sclk, PinName cs, const char* name, PinName analog_sw);
  
private:  
};

#endif