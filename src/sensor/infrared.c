#include <stdio.h>
#include <wiringPi.h>

int readInfrared(const int pinNo);

int main(){
        int time=0;
        wiringPiSetup();
        while(1){
                delay(100);
                if(readInfrared(1)){
                        printf("%d: detect somebody\n",time);
                }
                else{
                        printf("%d: didn't detect anyone\n",time);
                }
                time++;
        }
        return 0;

}

int readInfrared(const int pinNo){
        pinMode(pinNo,INPUT);
        if(digitalRead(pinNo)==HIGH){
                return 1;
        }
        else{
                return 0;
        }
}