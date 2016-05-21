#include <memory.h>
#include <string.h>
#include <stdio.h>
#include <sys/types.h>
#include <fcntl.h>
#include <termios.h>

int open_port(const char* port);
int close_port(int fd);
int setup_port(int fd);
int read_frame(int fd, int frame_size, char* head, int head_size , char* buf);

int open_port(const char* port){
	int fd=open(port,O_RDWR | O_NOCTTY);
	if(fd<0){
		return -1;
	}
	else{
		return fd;
	}
}

int close_port(int fd){
	close(fd);
	return 0;
}

int setup_port(int fd){
	struct termios tio;
	memset(&tio,0,sizeof(tio));
	tio.c_cflag= B9600|CS8|CLOCAL|CREAD;
	tio.c_iflag=IGNPAR;
	tio.c_oflag=0;
	tio.c_lflag=0;
	tio.c_cc[VMIN]=1;
	tcflush(fd,TCIFLUSH);
	if(tcsetattr(fd,TCSANOW,&tio)==-1){
		printf("port setup failed.\n");
	}
	return 0;
}

int read_frame(int fd, int frame_size, char* head, int head_size, char* buf){
	char localbuffer[8];
	int receive_length,localbuffer_index,head_index=0;
	int get_head=0,current_length=0;
	while(receive_length=read(fd,localbuffer,8)){
		if(get_head==0){
			for(localbuffer_index=0; localbuffer_index<receive_length; localbuffer_index++){
				if(localbuffer[localbuffer_index]==head[head_index]){
					head_index++;
					if(head_size==head_index){
						get_head=1;
						memcpy(buf,head,head_size);
						memcpy(buf+head_size,localbuffer+localbuffer_index+1,receive_length-1-localbuffer_index);
						current_length=head_size;
						current_length+=receive_length-localbuffer_index-1;
						break;
					}
				}
				else{
					head_index=0;
				}
			}
		}
		else{
			memcpy(buf+current_length,localbuffer,receive_length);
			current_length+=receive_length;
			if(current_length==frame_size){
				return 0;
			}
		}
	}
}

int main(){
	int fd=open_port("/dev/ttyUSB0"),receive_length,package_no;
	int tem,hum,dst,sst;
	char people,length;
	char buf[100],i;
	if(fd==-1){
		printf("ERROR: open port ERROR.\n");
	}
	setup_port(fd);
	package_no=0;
	while(1){
		read_frame(fd,28,"fd",2,buf);
		printf("receive:\n");
		for(i=0;i<28;i++){
		printf("0x%02x ",buf[i]);
		}
		printf("\n");
		length=*(buf+2);
		dst=*(buf+3)+*(buf+4)<<8;
		tem=*(int*)(buf+15);
		hum=*(int*)(buf+19);
		people=*(buf+27);
		printf("info:\n target address: %d, source address: %d, length = %d, humidity = %d %%, tempegature = %d'C, ",dst,sst,length,hum,tem);
		if(people==1){
			printf("detect someone.\n");
		}
		else{
			printf("detect nobody.\n");
		}
	}
	close_port(fd);
}
