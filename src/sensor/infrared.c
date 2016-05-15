#include <stdio.h>
#include <wiringPi.h>
int readInfrared(const int pinNo){
        pinMode(pinNo,INPUT);
        if(digitalRead(pinNo)==HIGH){
                return 1;
        }
        else{
                return 0;
        }
}