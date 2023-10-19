// *************************************************************************************************
// Cinewatch functions.
// *************************************************************************************************

// *************************************************************************************************
// Include section

// system
#include "project.h"
#include <string.h>

// driver
#include "stopwatch.h"
#include "ports.h"
#include "display.h"
#include "timer.h"
#include "vti_as.h"
#include "timer.h"
#include "radio.h"

// logic
#include "menu.h"
#include "cinema.h"
#include "acceleration.h"
#include "rfsimpliciti.h"
#include "bluerobin.h"
#include "simpliciti.h"
#include "clock.h"
#include "date.h"
#include "alarm.h"
#include "temperature.h"
#include "vti_ps.h"
#include "altitude.h"


// *************************************************************************************************
// Defines section


// *************************************************************************************************
// Global Variable section
u8 cinemaDisplay;
u8 outPos;	
u8 outAmount;
struct out outArray[MAX_OUTS];

// *************************************************************************************************
// @fn          reset_cinema
// @brief       Reset cinema to start value.
// @param       none
// @return      none
// *************************************************************************************************
void reset_cinema(void)
{	
	cinemaDisplay = DISPLAY_DEFAULT_VIEW;
	outPos = 0;
    outAmount = 0;       
}

// *************************************************************************************************
// @fn          sx_cinema
// @brief       Main Cinema Button press
// @param       line		LINE1, LINE2
// @return      none
// *************************************************************************************************
void sx_cinema(u8 line)
{     
	if (outPos == outAmount-1) {
		outPos = 0;
	} else {
		outPos++;
	}
}

void gotoCurrentOut() {
	for (int i = 0; i < outAmount; i++) {
		if (outArray[i].hour >= sTime.hour) {
			if (outArray[i].minute >= sTime.minute) {
				outPos = i;
				break;
			}     
		}
	}
}


// *************************************************************************************************
// @fn          mx_cinema
// @brief       Sets current out to next one
// @param       u8 line	LINE2
// @return      none
// *************************************************************************************************
void mx_cinema(u8 line)
{
	gotoCurrentOut();
}

// *************************************************************************************************
// @fn          display_sync
// @brief       SimpliciTI display routine. 
// @param       u8 line			LINE2
//				u8 update		DISPLAY_LINE_UPDATE_FULL
// @return      none
// *************************************************************************************************
void display_cinema(u8 line, u8 update)
{
	unsigned char outStr[7];
    unsigned char *tempStr;    


    if (update == DISPLAY_LINE_UPDATE_FULL)	
	{
        outStr[0] = ' ';

        //get screen number 
        tempStr = itoa(outArray[outPos].screen,1, 0);
        outStr[1] = tempStr[0];
        
        //get hour
        tempStr = itoa(outArray[outPos].hour, 2, 0);
        outStr[2] = tempStr[0];  
        outStr[3] = tempStr[1];
        
        //get min
        tempStr = itoa(outArray[outPos].minute, 2, 0);
        outStr[4] = tempStr[0];
        outStr[5] = tempStr[1];
        outStr[6] = '\0';

        display_chars(LCD_SEG_L2_5_0, outStr, SEG_ON);
	}
}


