#include <memory.h>
#include <string.h>
#include <stdio.h>
#include <sys/types.h>
#include <fcntl.h>
#include <termios.h>

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
	tio.c_cflag= B38400|CS8|CLOCAL|CREAD;
	tio.c_iflag=IGNPAR;
	tio.c_oflag=0;
	tio.c_lflag=0;
	tcflush(fd,TCIFLUSH);
	tcsetattr(fd,TCSANOW,&tio);
	return 0;	
}
/*
int main(){
	int fd=open_port("/dev/ttyAMA0"),length,send_length,package_no;
	char buf[100];
	int i=0;
	if(fd==-1){
		printf("ERROR: open port ERROR.\n");
	}
	setup_port(fd);
	package_no=0;
	while(1){
		sprintf(buf,"fdaaaaaaa");
		package_no++;
		buf[9]=package_no;
		send_length=write(fd,buf,10);
		tcflush(fd,TCOFLUSH);
		printf("send:\n");
		for(i=0;i<10;i++){
			printf("%02x ",buf[i]);
		}
		printf("\n");
		sleep(3);
	}
	close_port(fd);
}
*/
