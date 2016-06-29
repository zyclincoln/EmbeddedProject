#include <wiringPi.h>
#include <stdio.h>
#include "dht11.h"
int readDHT11(const int pinNo, int* temperature, int* humidity){
        int data[5];
        int circle=0, flag=0,num=0,bit=0;
        int cache[300];
        pinMode(pinNo,OUTPUT);
        digitalWrite(pinNo,LOW);
        delay(18);
        digitalWrite(pinNo,HIGH);
        delayMicroseconds(30);
        pinMode(pinNo,INPUT);
        for(circle=0;circle<300;circle++){
                delayMicroseconds(15);
                if(digitalRead(pinNo)==HIGH){
                        cache[circle]=1;
                }
                else{
                        cache[circle]=0;
                }
        }
        pinMode(pinNo,OUTPUT);
        digitalWrite(pinNo,HIGH);
        flag=0;
        for(circle=9;circle<300;circle++){
                if(cache[circle]==1){
                        flag++;
                }
                else{
                        if(flag==0){
                                flag=0;
                        }
                        else if(flag<3){
                                data[num]=data[num]<<1;
                                bit++;
                        }
                        else{
                                data[num]=data[num]<<1 | 0x0001;
                                bit++;
                        }
                        flag=0;
                        if(bit==8){
                                bit=0;
                                num++;
                        }
                        if(num==5){
                                cache[0]=data[0] & 0x00FF;
                                cache[1]=data[2] & 0x00FF;
                                cache[2]=data[4] & 0x00FF;
                                if(((cache[0]+cache[1]) & 0x00FF) == (cache[2]&0xFF)){
                                		*temperature=cache[1];
                                		*humidity=cache[0];
                                        return 1;
                                }
                                else{
                                        return 0;
                                }
                        }
                }
        }
        return 0;
}

