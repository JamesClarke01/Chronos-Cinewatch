// *************************************************************************************************
// Cinewatch sync functions.
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
#include "cinesync.h"
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
u8 cinesyncDisplay;
struct cineDateStruct cineDate;

// *************************************************************************************************
// @fn          reset_cinema
// @brief       Reset cinema to start value.
// @param       none
// @return      none
// *************************************************************************************************
void reset_cinesync(void)
{	
	cinesyncDisplay = DISPLAY_DEFAULT_VIEW;
}

// *************************************************************************************************
// @fn          sx_cinema
// @brief       Main Cinema Button press
// @param       line		LINE1, LINE2
// @return      none
// *************************************************************************************************
void sx_cinesync(u8 line)
{     
	// Stop acceleration sensor
	as_stop();

	// Turn on beeper icon to show activity
	display_symbol(LCD_ICON_BEEPER1, SEG_ON_BLINK_ON);
	display_symbol(LCD_ICON_BEEPER2, SEG_ON_BLINK_ON);
	display_symbol(LCD_ICON_BEEPER3, SEG_ON_BLINK_ON);

	// Debounce button event
	Timer0_A4_Delay(CONV_MS_TO_TICKS(BUTTONS_DEBOUNCE_TIME_OUT));

	// Prepare radio for RF communication
	open_radio();

	// Set SimpliciTI mode
	sRFsmpl.mode = SIMPLICITI_SYNC;
	
	// Set SimpliciTI timeout to save battery power
	sRFsmpl.timeout = SIMPLICITI_TIMEOUT;
	
	cinemaDisplay = DISPLAY_DEFAULT_VIEW;

	if (simpliciti_link()) {		
		simpliciti_main_sync();
		cinesyncDisplay = DISPLAY_ALTERNATIVE_VIEW;	
	}
	
	// Set SimpliciTI state to OFF
	sRFsmpl.mode = SIMPLICITI_OFF;

	// Powerdown radio
	close_radio();
	
	// Clear last button events
	Timer0_A4_Delay(CONV_MS_TO_TICKS(BUTTONS_DEBOUNCE_TIME_OUT));
	BUTTONS_IFG = 0x00;  
	button.all_flags = 0;
	
	// Clear icons
	display_symbol(LCD_ICON_BEEPER1, SEG_OFF_BLINK_OFF);
	display_symbol(LCD_ICON_BEEPER2, SEG_OFF_BLINK_OFF);
	display_symbol(LCD_ICON_BEEPER3, SEG_OFF_BLINK_OFF);
	
	// Force full display update
	display.flag.full_update = 1;	
}

// *************************************************************************************************
// @fn          display_sync
// @brief       SimpliciTI display routine. 
// @param       u8 line			LINE2
//				u8 update		DISPLAY_LINE_UPDATE_FULL
// @return      none
// *************************************************************************************************
void display_cinesync(u8 line, u8 update)
{
	u8 * str;
	if (update == DISPLAY_LINE_UPDATE_FULL)	
	{
		if (cinesyncDisplay == DISPLAY_DEFAULT_VIEW) {
			display_chars(LCD_SEG_L2_5_0, " NDATA", SEG_ON);
		} else {
			// Convert day to string
			str = itoa(cineDate.day, 2, 0);
			display_chars(switch_seg(line, LCD_SEG_L1_3_2, LCD_SEG_L2_3_2), str, SEG_ON);
			
			// Convert month to string
			str = itoa(cineDate.month, 2, 0);

			display_chars(switch_seg(line, LCD_SEG_L1_1_0, LCD_SEG_L2_1_0), str, SEG_ON);
						
			// Display "." to separate day and month
			display_symbol(switch_seg(line, LCD_SEG_L1_DP1, LCD_SEG_L2_DP), SEG_ON);
		}
	}
}
