#ifndef _SENDER_H_
#define _SENDER_H_

int open_port(const char* port);
int close_port(int fd);
int setup_port(int fd);

#endif
