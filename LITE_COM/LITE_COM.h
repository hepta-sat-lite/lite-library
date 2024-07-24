#ifndef MBED_HEPTACOM_H
#define MBED_HEPTACOM_H
#include "HeptaXbee.h"

class LITE_COM : public HeptaXbee{
public:
    LITE_COM(
            PinName tx,
            PinName rx,
            int brate
    );
private:  
};

#endif