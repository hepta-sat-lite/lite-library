#include "LITE_CDH.h"
LITE_CDH::LITE_CDH(PinName mosi, PinName miso, PinName sclk, PinName cs, const char* name, PinName analog_sw):SDFileSystem( mosi, miso, sclk, cs, name), LiteAnalogSW(analog_sw){
}
