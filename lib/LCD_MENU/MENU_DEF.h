
#ifndef MENU_DEF_h
#define MENU_DEF_h
#include <avr/pgmspace.h>

static char lcd_buff[20];
const char PROGMEM M1_HEADER[]  = "ERU"; // "String 0" etc are strings to store - change to suit.
const char PROGMEM M1_PICKER_name_0[]  = "LIGHT"; // "String 0" etc are strings to store - change to suit.
const char PROGMEM M1_PICKER_name_1[]  = "SPINDLE"; // "String 0" etc are strings to store - change to suit.
const char PROGMEM M1_PICKER_name_2[]  = "CONFIG"; // "String 0" etc are strings to store - change to suit.

const char PROGMEM M11_HEADER[]  = "SPINDLE CONTROL"; // "String 0" etc are strings to store - change to suit.
const char PROGMEM M11_PICKER_name_0[]  = "SPINDLE PID"; // "String 0" etc are strings to store - change to suit.
const char PROGMEM M11_PICKER_name_1[]  = "EXT PWM"; // "String 0" etc are strings to store - change to suit.
const char PROGMEM M11_PICKER_name_2[]  = "SPINDLE PWR %:"; // "String 0" etc are strings to store - change to suit.

const char PROGMEM M110_HEADER[]  = "SPINDLE PID"; // "String 0" etc are strings to store - change to suit.
const char PROGMEM M110_PICKER_name_0[]  = "Target:"; // "String 0" etc are strings to store - change to suit.

const char PROGMEM M111_HEADER[]  = "EXT PWM"; // "String 0" etc are strings to store - change to suit.
const char PROGMEM M111_PICKER_name_0[]  = "Link:"; // "String 0" etc are strings to store - change to suit.

const char PROGMEM M112_HEADER[]  = "SPINDLE PWR %:"; // "String 0" etc are strings to store - change to suit.
const char PROGMEM M112_PICKER_name_0[]  = "Target:"; // "String 0" etc are strings to store - change to suit.

const char PROGMEM M11_HEADER[]  = "SPINDLE CONTROL"; // "String 0" etc are strings to store - change to suit.
const char PROGMEM M11_PICKER_name_0[]  = "SPINDLE PID"; // "String 0" etc are strings to store - change to suit.
const char PROGMEM M11_PICKER_name_1[]  = "EXT PWM"; // "String 0" etc are strings to store - change to suit.
const char PROGMEM M11_PICKER_name_2[]  = "SPINDLE PWR %:"; // "String 0" etc are strings to store - change to suit.


#endif

const char* const M_Header_table[] PROGMEM = {M1_HEADER,
                                                M11_HEADER,
                                                    M110_HEADER, M111_HEADER, M112_HEADER
                                            };
const char* const M_Picker_table[] PROGMEM = {M1_PICKER_name_0,M1_PICKER_name_1,M1_PICKER_name_2,
                                                M11_PICKER_name_0,M11_PICKER_name_1,M11_PICKER_name_2,
                                                    M110_PICKER_name_0,
                                                    M111_PICKER_name_0,
                                                    M112_PICKER_name_0
                                            };