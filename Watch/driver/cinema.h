// *************************************************************************************************
// Include section


// *************************************************************************************************
// Prototypes section
extern void reset_cinema(void);
extern void display_cinema(u8 line, u8 update);
extern void sx_cinema(u8 line);
extern void mx_cinema(u8 line);
extern void gotoCurrentOut();

// *************************************************************************************************
// Defines section
#define MAX_OUTS 100

// *************************************************************************************************
// Global Variable section
struct out
{
	u8  screen;
	u8  hour;
    u8  minute;
};


extern u8 cinemaDisplay;
extern u8 outPos;
extern u8 outAmount;
extern struct out outArray[MAX_OUTS];



// *************************************************************************************************
// Extern section
