#include "mbed.h"
#include "LITE_EPS.h"
#include "LITE_CDH.h"
#include "LITE_SENSOR.h"
#include "LITE_COM.h"

LITE_EPS eps(PA_0,PA_4);
LITE_CDH cdh(PB_5, PB_4, PB_3, PA_8, "sd");
LITE_SENSOR sensor(PA_7,PB_7,PB_6);
LITE_COM com(PA_9,PA_10,9600);


int main() {
  
}
