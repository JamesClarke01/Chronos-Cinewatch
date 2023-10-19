#include <stdio.h>
#include <unistd.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>
#include "APUtils.h"

//#define PACKET_LEN 22
#define PACKET_START 0xFF
#define CMD_WRITE 0x31
#define CMD_CINESYNC 0x08

#define START_POS 0
#define WRITE_POS 1
#define LEN_POS 2
#define CINESYNC_POS 3
#define PACKETNUM_POS 4

#define DAY_POS 5
#define MONTH_POS 6

#define OUTAMOUNT_POS 5

struct out {
	uint8_t screen;
	uint8_t hour;
	uint8_t minute;
};

void printPacket(uint8_t* packet) {
	int size = packet[2];
	
	for (int i = 0; i < size; i++) {
		printf("%d:\t%d\n", i, packet[i]); 
	}
	printf("\n");
};

int main(int argc, char *argv[]) {
	int fd = 0;
	uint8_t packetLen;
	uint8_t day;
	uint8_t month;
	uint8_t outAmount;
	struct out outArray[200];
	uint8_t packet[22];
	char str[3];
	char c;
	FILE* file;
	int fullPacketAmount;
	int outArrayPos;
	int screenPos;
	int lastPacketOutAmount;
	int i, j;

	//arguements
	bool printMode = 0;
	bool testMode = 0;

	//check arguments
	for(int i = 1; i <argc; i++) {
		if (strcmp(argv[i], "-p") == 0) {
			printMode = 1;
		} else if (strcmp(argv[i], "-t") == 0) {
			testMode = 1;
		}
	}

	//read file
	if (testMode) {
		file = fopen("testData", "r");
	} else {
		file = fopen("cineData", "r");
	}
	
	//Day
	str[0] = fgetc(file);
	str[1] = fgetc(file);
	str[2] = '\0';
	day = atoi(str);

	//Month
	str[0] = fgetc(file);
	str[1] = fgetc(file);
	str[2] = '\0';
	month = atoi(str);

	outAmount = 0;
	while (fgetc(file) == ',') {
	
		outArray[outAmount].screen = fgetc(file) - '0'; //screen num

		//hour
		str[0] = fgetc(file);

		str[1] = fgetc(file);
		str[2] = '\0';
		outArray[outAmount].hour = atoi(str); 

		//min
		str[0] = fgetc(file);
		str[1] = fgetc(file);
		str[2] = '\0';
		outArray[outAmount].minute = atoi(str);

		outAmount++;
	}
	
	fclose(file);
	
	//Start sending packets:
	APConnect(&fd);

	APOn(&fd);

	sleep(3); //wait for AP to turn on

	//PACKET 1
	packet[START_POS] = PACKET_START;
	packet[WRITE_POS] = CMD_WRITE;
	packet[LEN_POS] = 7;
	packet[CINESYNC_POS] = CMD_CINESYNC;
	
	packet[PACKETNUM_POS] = 0;
	packet[DAY_POS] = day;
	packet[MONTH_POS] = month;
	
	write(fd, packet, sizeof(packet));
	sleep(3);

	if (printMode) {
		printf("Packet 0 Sent:\n");
		printPacket(packet);
	}

	
	//Send packet here
	
	//FULL PACKETS
	outArrayPos = 0;
	fullPacketAmount = floor(outAmount/5); //find the amount of packets that will have all 5 outs populated
	
	for (i = 0; i < fullPacketAmount; i++) {
		packet[LEN_POS] = 21;
		packet[PACKETNUM_POS] = i+1;
		packet[OUTAMOUNT_POS] = 5;
		
		for (j = 0; j < 5; j++) {
			screenPos = 6 + (j*3);
			packet[screenPos] = outArray[outArrayPos].screen;
			packet[screenPos+1] = outArray[outArrayPos].hour;
			packet[screenPos+2] = outArray[outArrayPos].minute;
			outArrayPos++;
		}	

		write(fd, packet, sizeof(packet));
		sleep(3);

		if (printMode) {
			printf("Packet %d Sent:\n", i+1);
			printPacket(packet);
		}
	}

	//LAST PACKET
	if (outAmount % 5 > 0) {
		lastPacketOutAmount = outAmount % 5;
		packet[LEN_POS] = 6 + (lastPacketOutAmount * 3);
		packet[PACKETNUM_POS] = floor(outAmount/5) + 1;
		packet[OUTAMOUNT_POS] = lastPacketOutAmount;
		
		for (i = 0; i < lastPacketOutAmount; i++) {
			screenPos = 6 + (i*3);
			packet[screenPos] = outArray[outArrayPos].screen;
			packet[screenPos+1] = outArray[outArrayPos].hour;
			packet[screenPos+2] = outArray[outArrayPos].minute;
			outArrayPos++;
		}

		write(fd, packet, sizeof(packet));
		sleep(3);

		if (printMode) {
			printf("Packet %d Sent:\n", floor(outAmount/5)+1);
			printPacket(packet);
		}
	}	
}
