#include "serial.h"

int serial_open(const char *device, int baud)
{	
    struct termios tio;
    memset(&tio, 0, sizeof(tio));

    tio.c_cflag = CS8 | CREAD | CLOCAL;
    tio.c_cc[VMIN] = 1;
    tio.c_cc[VTIME] = 5;

    int fd = open(device, O_RDWR | O_NONBLOCK);

    cfsetospeed(&tio, baud);
    cfsetispeed(&tio, baud);
    tcsetattr(fd, TCSANOW, &tio);

    return fd;
}

uint8_t serial_recv(int fd)
{
    uint8_t byte;
    read(fd, &byte, 1);
    return byte;
}

void serial_send(int fd, uint8_t byte)
{
    write(fd, &byte, 1);
}
