
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

	info[0]=0xfd;
        while(time!=50){
                delay(3330);
                readDHT11(0,&temp,&hum);
		people=readInfrared(1);

                time++;
                info[1]=21;
                *(int*)(info+2)=874;
		*(int*)(info+4)=871;
                *(int*)(info+6)=0;
                *(int*)(info+10)=0;
                *(int*)(info+14)=temp;
                *(int*)(info+18)=hum;
                *(int*)(info+22)=0;
                *(info+26)=(char)people;
		
		/*		
		for(i=0;i<27;i++){
			printf("0x%02x ",info[i]);
		}
		printf("\n");
		
		printf("send data: \n humidity = %d\n temperature = %d\n people = %d\n",hum,temp,people);
                */
        	write(fd,info,27);
	}
        close_port(fd);
        return 0;
}
