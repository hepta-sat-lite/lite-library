#include "LITE_CDH.h"
LITE_CDH::LITE_CDH(PinName mosi, PinName miso, PinName sclk, PinName cs, const char* name) : SDFileSystem( mosi, miso, sclk, cs, name){
}
