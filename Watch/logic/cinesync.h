// *************************************************************************************************
// Include section

// *************************************************************************************************
// Prototypes section
extern void reset_cinesync(void);
extern void display_cinesync(u8 line, u8 update);
extern void sx_cinesync(u8 line);

// *************************************************************************************************
// Defines section


// *************************************************************************************************
// Global Variable section
struct cineDateStruct
{
	u8  day;
	u8  month;
};

extern struct cineDateStruct cineDate;

extern u8 cinesyncDisplay;


// *************************************************************************************************
// Extern section
