#ifndef __SERIAL_H__
#define __SERIAL_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>

int serial_open(const char *device, int baud);
uint8_t serial_recv(int fd);
void serial_send(int fd, uint8_t byte);

#ifdef __cplusplus
}
#endif

#endif /* __SERIAL_H__ */
