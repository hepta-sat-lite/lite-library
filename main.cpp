#include "mbed.h"
#include "LITE_EPS.h"
#include "LITE_CDH.h"
#include "LITE_SENSOR.h"
#include "LITE_COM.h"

LITE_EPS    eps(PA_0,PA_4);
LITE_CDH    cdh(PB_5, PB_4, PB_3, PA_8, "sd", PA_3);
LITE_SENSOR sensor(PA_7,PB_7,PB_6);
LITE_COM    com(PA_9,PA_10,9600);
DigitalOut  condition(PB_1);
RawSerial   sat(USBTX,USBRX,9600);
Timer       sattime;

int rcmd = 0, cmdflag = 0; //command variable

//getting command and command flag
void commandget()
{
    rcmd = sat.getc();
    cmdflag = 1;
}
//interrupting process by command receive
void receive(int rcmd, int cmdflag)
{
    sat.attach(commandget,Serial::RxIrq);
}
//initializing
void initialize()
{
    rcmd = 0;
    cmdflag = 0;
    condition = 0;
}

int main() {
    sat.printf("From Sat : Nominal Operation\r\n");
    int flag = 0; //condition flag
    float batvol, temp; //voltage, temperature 
    sattime.start();
    receive(rcmd,cmdflag); //interrupting
    eps.turn_on_regulator();//turn on 3.3V conveter
    while(1){
        condition = !condition;
        //senssing HK data
        eps.vol(&batvol);
        // sensor.temp_sense(&temp);
        //Transmitting HK data to Ground Station(sat)
        // sat.printf("HEPTASAT::Condition = %d, Time = %f [s], batvol = %2f [V], temp = %2f [deg C]\r\n",flag,sattime.read(),batvol,temp);
        wait_ms(1000);
        //Power Saving Mode 
        if(batvol <= 3.5){
            eps.shut_down_regulator();
            sat.printf("Power saving mode ON\r\n"); 
            flag = 1;
        } else if((flag == 1) & (batvol > 3.7)) {
            eps.turn_on_regulator();
            sat.printf("Power saving mode OFF\r\n");
            flag = 0;
        }
        if(cmdflag == 1){
            if(rcmd == 'a'){
                // check analog switch function
                sat.printf("\r\nAnalog SW Check Mode\r\n");
                for(int j=0;j<5;j++){
                    if(j% 2 == 0){
                        sat.printf("Turn off Analog SW\r\n");
                        cdh.turn_off_analogSW();
                    }else{
                        sat.printf("Turn ON Analog SW\r\n");
                        cdh.turn_on_analogSW();
                    }
                    wait(1);
                }
            }else if(rcmd == 'b'){
                // check saving to sd card
                sat.printf("\r\nSaving letter to SD memory\r\n");
                char str[100];
                mkdir("/sd/mydir", 0777);
                FILE *fp = fopen("/sd/mydir/satdata.txt","w");
                if(fp == NULL) {
                    error("Could not open file for write\r\n");
                }
                for(int i = 0; i < 10; i++) {
                    eps.vol(&batvol);
                    fprintf(fp,"%f\r\n",batvol);
                    condition = 1;
                    wait_ms(1000);
                }
                fclose(fp);
                fp = fopen("/sd/mydir/satdata.txt","r");
                for(int i = 0; i < 10; i++) {
                    fgets(str,100,fp);
                    sat.puts(str);
                }
                fclose(fp);
            }else if(rcmd == 'c'){
                //check 9axis sensor
                sat.printf("\r\n9 axis sensor Check Mode\r\n");
                sensor.init();
                for(int i = 0; i < 10; i++){
                    sensor.getAccGyro();
                    sat.printf(">>>> ax = %f, ay = %f, az = %f\r\n", sensor.getAX(), sensor.getAY(), sensor.getAZ());
                    sat.printf(">>>> gx = %f, gy = %f, gz = %f\r\n", sensor.getGX(), sensor.getGY(), sensor.getGZ());
                    sensor.getMag();
                    // sensor.icm20948MagCheck();
                    // sensor.icm20948MagRead();
                    sat.printf(">>>> mx = %f, my = %f, mz = %f\r\n\r\n", sensor.getMX(), sensor.getMY(), sensor.getMZ());
                    wait(1);
                }
            }else if(rcmd == 'd'){
                // check uplink and downlink
                sat.printf("\r\nXbee Check Mode\r\n");
                cdh.turn_on_analogSW();
                sat.printf("Hit 'a' from sat with Xbee\r\n");
                com.xbee_receive(&rcmd,&cmdflag);
                sat.printf("rcmd=%d, cmdflag=%d\r\n",rcmd, cmdflag);
                if (cmdflag == 1) {
                    if (rcmd == 'a') {
                        sat.printf("Command Get %d\r\n",rcmd);
                        com.printf("HEPTA Uplink OK\r\n");
                    }
                    com.initialize();
                }
                wait(1.0);
            }
            initialize(); //initializing
        }
    }
}
