
#include <stdio.h>
#include <wiringPi.h>
#include <string.h>
#include "includes.h"

int main(){
        int temp,hum,people,time=0;
        int fd,i;
        char info[100];
        
	wiringPiSetup();
        fd=open_port("/dev/ttyAMA0");
        setup_port(fd);

	info[0]='f';
        info[1]='d';
        while(time!=10){
                delay(3330);
                readDHT11(0,&temp,&hum);
		people=readInfrared(1);

                time++;
                info[2]=28;
                info[3]=0x0f;
                info[4]=0xff;
                info[5]=0;
                info[6]=0;
                *(int*)(info+7)=0x00000000;
                *(int*)(info+11)=0x00000000;
                *(int*)(info+15)=temp;
                *(int*)(info+19)=hum;
                *(int*)(info+23)=0x00000000;
                *(info+27)=(char)people;
		
		for(i=0;i<28;i++){
			printf("0x%02x ",info[i]);
		}
		printf("\n");
		
		printf("send data: \n humidity = %d\n temperature = %d\n people = %d\n",hum,temp,people);
                
        	write(fd,info,28);
	}
        close_port(fd);
        return 0;
}
