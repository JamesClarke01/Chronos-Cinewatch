#include <errno.h>

#include <fcntl.h> 

#include <string.h>

#include <termios.h>

#include <unistd.h>
#include <stdio.h>
#include <stdint.h>

#include "APUtils.h"

// TI eZ430 Chronos commands

uint8_t AP_ON_COMMAND[]    = { 0xFF, 0x07, 0x03 };
uint8_t ACC_DATA_REQUEST[] = { 0xFF, 0x08, 0x07, 0x00, 0x00, 0x00, 0x00 };
uint8_t AP_OFF_COMMAND[]   = { 0xFF, 0x09, 0x03 };
uint8_t SET_WATCH[]        = { 0xFF, 0x31, 0x16, 0x03, 0x84, 0x14, 0x2D, 0x07, 0xd9, 0x09, 0x01, 0x06, 0x1e,
							   0x00, 0xdc, 0x01, 0xf4, 0x00, 0x00, 0x00, 0x00, 0x00};

/*
Serial Setup Code gotten from here:
https://stackoverflow.com/questions/6947413/how-to-open-read-and-write-from-serial-port-in-c
*/
int set_interface_attribs (int fd, int speed, int parity)
{
        struct termios tty;

        if (tcgetattr (fd, &tty) != 0)

        {
                printf("error %d from tcgetattr", errno);
                return -1;
        }


        cfsetospeed (&tty, speed);
        cfsetispeed (&tty, speed);


        tty.c_cflag = (tty.c_cflag & ~CSIZE) | CS8;     // 8-bit chars

        // disable IGNBRK for mismatched speed tests; otherwise receive break

        // as \000 chars

        tty.c_iflag &= ~IGNBRK;         // disable break processing

        tty.c_lflag = 0;                // no signaling chars, no echo,

                                        // no canonical processing

        tty.c_oflag = 0;                // no remapping, no delays

        tty.c_cc[VMIN]  = 0;            // read doesn't block

        tty.c_cc[VTIME] = 5;            // 0.5 seconds read timeout



        tty.c_iflag &= ~(IXON | IXOFF | IXANY); // shut off xon/xoff ctrl



        tty.c_cflag |= (CLOCAL | CREAD);// ignore modem controls,

                                        // enable reading

        tty.c_cflag &= ~(PARENB | PARODD);      // shut off parity

        tty.c_cflag |= parity;

        tty.c_cflag &= ~CSTOPB;

        tty.c_cflag &= ~CRTSCTS;



        if (tcsetattr (fd, TCSANOW, &tty) != 0)

        {

                printf("error %d from tcsetattr", errno);

                return -1;

        }

        return 0;

}



void set_blocking (int fd, int should_block)
{

        struct termios tty;

        memset (&tty, 0, sizeof tty);

        if (tcgetattr (fd, &tty) != 0)

        {

                printf ("error %d from tggetattr", errno);

                return;

        }

        tty.c_cc[VMIN]  = should_block ? 1 : 0;

        tty.c_cc[VTIME] = 5;            // 0.5 seconds read timeout



        if (tcsetattr (fd, TCSANOW, &tty) != 0)

                printf ("error %d setting term attributes", errno);

}

int APConnect(int* fd)
{	
	char *portname = "/dev/ttyACM0";

	*fd = open (portname, O_RDWR | O_NOCTTY | O_SYNC);

	if (*fd < 0)

	{
        printf ("error %d opening %s: %s", errno, portname, strerror (errno));	       
		return 0;
	}
	
	set_interface_attribs (*fd, B115200, 0);  // set speed to 115,200 bps, 8n1 (no parity)

	set_blocking (*fd, 0);                // set no blocking

	tcflush(*fd, TCIOFLUSH); //Flushes everything from serial buffer
}

int APOn(int* fd)
{	
	write (*fd, AP_ON_COMMAND, sizeof(AP_ON_COMMAND));  //Turn AP on
	return 1;
}

int APOff(int* fd)
{
	write (*fd, AP_OFF_COMMAND, sizeof(AP_ON_COMMAND));
	return 1;
}


int setWatch(int* fd)
{
	write (*fd, SET_WATCH, sizeof(SET_WATCH));
	return 1;
}



