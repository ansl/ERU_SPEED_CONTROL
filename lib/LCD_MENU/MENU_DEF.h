
#ifndef MENU_DEF_h
#define MENU_DEF_h
#include <avr/pgmspace.h>



//////////////////////////////////////////////////////////////////
// const char PROGMEM M1_HEADER[]  = "ERU";
//     const uint8_t PROGMEM M1_PICKER_mode_0 = 2;
//     const char PROGMEM M1_PICKER_name_0[]  = "LIGHT"; 
//     const char PROGMEM M1_PICKER_state_string0_0[]  = "OFF"; 
//     const char PROGMEM M1_PICKER_state_string1_0[]  = "ON"; 
//     const uint8_t PROGMEM M1_PICKER_decimals_0 = 0; 
//     const char PROGMEM M1_PICKER_unit_0[]  = ""; 
//     const float PROGMEM M1_PICKER_min_value_0 = -10000; 
//     const float PROGMEM M1_PICKER_max_value_0 = 10000; 
//     const uint16_t PROGMEM M1_PICKER_inc_short_0 = 1; 
//     const uint16_t PROGMEM M1_PICKER_inc_long_0 = 10; 
//     const uint16_t PROGMEM M1_PICKER_tblink_0 = 300;
//     const uint8_t PROGMEM M1_PICKER_EEPROM_ADDR_0 = 0;
/////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////
//VARIABLES
uint16_t screen_saver_delay=10000;
uint8_t screen_saver_refresh=100;
// bool menu_flag=1;
unsigned long t_last_button_input=0;

char MENU_Header[21];
uint8_t PICKER_mode;
char PICKER_name[11];
char PICKER_state_string0[21];
char PICKER_state_string1[21];
uint8_t PICKER_decimals;
char PICKER_unit[4];
float PICKER_min_value;
float PICKER_max_value;
float PICKER_inc_short;
float PICKER_inc_long;
uint16_t PICKER_tblink;
uint8_t PICKER_EEPROM_ADDR;
const char PROGMEM null[]="";
////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////
//MENU DEFINITIONS
const char PROGMEM M1_HEADER[]  = "ERU";

    const uint8_t PROGMEM M1_PICKER_mode_0 = 2;//default type of picker =1 ; 1:next menu ->  2:on/off can be customized with the status_string 3:input variable value + unit(customizable)
    const char PROGMEM M1_PICKER_name_0[]  = "LIGHT"; 
    const char PROGMEM M1_PICKER_state_string0_0[]  = "OFF"; 
    const char PROGMEM M1_PICKER_state_string1_0[]  = "ON"; 
    
    const uint8_t PROGMEM M1_PICKER_mode_1 = 1;//default type of picker =1 ; 1:next menu ->  2:on/off can be customized with the status_string 3:input variable value + unit(customizable)
    const char PROGMEM M1_PICKER_name_1[]  = "SPINDLE SETUP";

           const char PROGMEM M11_HEADER[]  = "SPINDLE CONTROL"; // "String 0" etc are strings to store - change to suit.
                const uint8_t PROGMEM M11_PICKER_mode_0 = 1;//default type of picker =1 ; 1:next menu ->  2:on/off can be customized with the status_string 3:input variable value + unit(customizable)
                const char PROGMEM M11_PICKER_name_0[]  = "PID";
                    const char PROGMEM M110_HEADER[]  = "SPINDLE PID";
                        const uint8_t PROGMEM M110_PICKER_mode_0 = 3;
                        const char PROGMEM M110_PICKER_name_0[]  = "Target:"; 
                        const uint8_t PROGMEM M110_PICKER_decimals_0 = 0; 
                        const char PROGMEM M110_PICKER_unit_0[]  = "RPM"; 
                        const float PROGMEM M110_PICKER_min_value_0 = 0; 
                        const float PROGMEM M110_PICKER_max_value_0 = 10000; 
                        const float PROGMEM M110_PICKER_inc_short_0 = 100; 
                        const float PROGMEM M110_PICKER_inc_long_0 = 500; 
                        const uint16_t PROGMEM M110_PICKER_tblink_0 = 300;

                const uint8_t PROGMEM M11_PICKER_mode_1 = 1;//default type of picker =1 ; 1:next menu ->  2:on/off can be customized with the status_string 3:input variable value + unit(customizable)
                const char PROGMEM M11_PICKER_name_1[]  = "EXT PWM"; 
                    const char PROGMEM M111_HEADER[]  = "ERU PWM SOURCE:";
                        const uint8_t PROGMEM M111_PICKER_mode_0 = 2;
                        const char PROGMEM M111_PICKER_name_0[]  = "Link:"; 
                        const char PROGMEM M111_PICKER_state_string0_0[]  = "Disconnected"; 
                        const char PROGMEM M111_PICKER_state_string1_0[]  = "Connected"; 

                const uint8_t PROGMEM M11_PICKER_mode_2 = 1;//default type of picker =1 ; 1:next menu ->  2:on/off can be customized with the status_string 3:input variable value + unit(customizable)
                const char PROGMEM M11_PICKER_name_2[]  = "PWR %"; 
                    const char PROGMEM M112_HEADER[]  = "Power % ";
                        const uint8_t PROGMEM M112_PICKER_mode_0 = 3;
                        const char PROGMEM M112_PICKER_name_0[]  = "Duty:"; 
                        const uint8_t PROGMEM M112_PICKER_decimals_0 = 0; 
                        const char PROGMEM M112_PICKER_unit_0[]  = "%"; 
                        const float PROGMEM M112_PICKER_min_value_0 = 0; 
                        const float PROGMEM M112_PICKER_max_value_0 = 100; 
                        const float PROGMEM M112_PICKER_inc_short_0 = 1; 
                        const float PROGMEM M112_PICKER_inc_long_0 = 10; 
                        const uint16_t PROGMEM M112_PICKER_tblink_0 = 300;


    const uint8_t PROGMEM M1_PICKER_mode_2 = 1;//default type of picker =1 ; 1:next menu ->  2:on/off can be customized with the status_string 3:input variable value + unit(customizable)
    const char PROGMEM M1_PICKER_name_2[]  = "CONFIG"; 
        const char PROGMEM M12_HEADER[]  = "CONFIGURATION";

            const uint8_t PROGMEM M12_PICKER_mode_0 = 3;
            const char PROGMEM M12_PICKER_name_0[]  = "P:"; 
            const uint8_t PROGMEM M12_PICKER_decimals_0 = 4; 
            const char PROGMEM M12_PICKER_unit_0[]  = ""; 
            const float PROGMEM M12_PICKER_min_value_0 = 0; 
            const float PROGMEM M12_PICKER_max_value_0 = 100; 
            const float PROGMEM M12_PICKER_inc_short_0 = 0.01; 
            const float PROGMEM M12_PICKER_inc_long_0 = 0.1; 
            const uint16_t PROGMEM M12_PICKER_tblink_0 = 300;
            const uint8_t PROGMEM M12_PICKER_EEPROM_ADDR_0 = 1;

            const uint8_t PROGMEM M12_PICKER_mode_1 = 3;
            const char PROGMEM M12_PICKER_name_1[]  = "D:"; 
            const uint8_t PROGMEM M12_PICKER_decimals_1 = 4; 
            const char PROGMEM M12_PICKER_unit_1[]  = ""; 
            const float PROGMEM M12_PICKER_min_value_1 = 0; 
            const float PROGMEM M12_PICKER_max_value_1 = 100; 
            const float PROGMEM M12_PICKER_inc_short_1 = 0.1; 
            const float PROGMEM M12_PICKER_inc_long_1 = 1; 
            const uint16_t PROGMEM M12_PICKER_tblink_1 = 300;
            const uint8_t PROGMEM M12_PICKER_EEPROM_ADDR_1 = 5;

            const uint8_t PROGMEM M12_PICKER_mode_2 = 3;
            const char PROGMEM M12_PICKER_name_2[]  = "I:"; 
            const uint8_t PROGMEM M12_PICKER_decimals_2 = 4; 
            const char PROGMEM M12_PICKER_unit_2[]  = ""; 
            const float PROGMEM M12_PICKER_min_value_2 = 0; 
            const float PROGMEM M12_PICKER_max_value_2 = 100; 
            const float PROGMEM M12_PICKER_inc_short_2 = 0.01; 
            const float PROGMEM M12_PICKER_inc_long_2 = 1; 
            const uint16_t PROGMEM M12_PICKER_tblink_2 = 300;
            const uint8_t PROGMEM M12_PICKER_EEPROM_ADDR_2 = 9;         

            const uint8_t PROGMEM M12_PICKER_mode_3 = 3;
            const char PROGMEM M12_PICKER_name_3[]  = "T max:"; 
            const uint8_t PROGMEM M12_PICKER_decimals_3 = 4; 
            const char PROGMEM M12_PICKER_unit_3[]  = ""; 
            const float PROGMEM M12_PICKER_min_value_3 = 0; 
            const float PROGMEM M12_PICKER_max_value_3 = 200; 
            const float PROGMEM M12_PICKER_inc_short_3 = 1; 
            const float PROGMEM M12_PICKER_inc_long_3 = 10; 
            const uint16_t PROGMEM M12_PICKER_tblink_3 = 300;
            const uint8_t PROGMEM M12_PICKER_EEPROM_ADDR_3 = 13;        

    const uint8_t PROGMEM M1_PICKER_mode_3 = 2;//default type of picker =1 ; 1:next menu ->  2:on/off can be customized with the status_string 3:input variable value + unit(customizable)
    const char PROGMEM M1_PICKER_name_3[]  = "COOLANT"; 
    const char PROGMEM M1_PICKER_state_string0_3[]  = "OFF"; 
    const char PROGMEM M1_PICKER_state_string1_3[]  = "ON"; 



#endif
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////
//MENU MATRIXES
const PROGMEM char* const M_Header_table[] = {M1_HEADER,
                                                M11_HEADER,
                                                    M110_HEADER, M111_HEADER, M112_HEADER, 
                                                M12_HEADER
                                            };

const uint8_t M_Picker_mode_table[] PROGMEM = {M1_PICKER_mode_0,
                                                    M1_PICKER_mode_1,
                                                         M11_PICKER_mode_0,
                                                            M110_PICKER_mode_0,
                                                         M11_PICKER_mode_1,
                                                            M111_PICKER_mode_0,
                                                         M11_PICKER_mode_2,
                                                            M112_PICKER_mode_0,
                                                    M1_PICKER_mode_2,
                                                         M12_PICKER_mode_0,
                                                         M12_PICKER_mode_1,
                                                         M12_PICKER_mode_2,
                                                         M12_PICKER_mode_3,
                                                    M1_PICKER_mode_3
                                            };
const char* const M_Picker_name_table[] PROGMEM = {M1_PICKER_name_0,
                                                    M1_PICKER_name_1,
                                                         M11_PICKER_name_0,
                                                            M110_PICKER_name_0,
                                                         M11_PICKER_name_1,
                                                            M111_PICKER_name_0,
                                                         M11_PICKER_name_2,
                                                            M112_PICKER_name_0,
                                                    M1_PICKER_name_2,
                                                         M12_PICKER_name_0,
                                                         M12_PICKER_name_1,
                                                         M12_PICKER_name_2,
                                                         M12_PICKER_name_3,
                                                    M1_PICKER_name_3
                                            };

const char* const M_Picker_state_string0_table[] PROGMEM = {M1_PICKER_state_string0_0,
                                                    null,
                                                         null,
                                                            null,
                                                         null,
                                                            M111_PICKER_state_string0_0,
                                                         null,
                                                            null,
                                                    null,
                                                         null,
                                                         null,
                                                         null,
                                                         null,
                                                     M1_PICKER_state_string0_3
                                            };
const char* const M_Picker_state_string1_table[] PROGMEM = {M1_PICKER_state_string1_0,
                                                    null,
                                                         null,
                                                            null,
                                                         null,
                                                            M111_PICKER_state_string1_0,
                                                         null,
                                                            null,
                                                    null,
                                                         null,
                                                         null,
                                                         null,
                                                         null,
                                                    M1_PICKER_state_string1_3
                                            };

const uint8_t M_Picker_decimals_table[] PROGMEM = {0,
                                                    0,
                                                         0,
                                                            M110_PICKER_decimals_0,
                                                         0,
                                                            0,
                                                         0,
                                                            M112_PICKER_decimals_0,
                                                    0,
                                                         M12_PICKER_decimals_0,
                                                         M12_PICKER_decimals_1,
                                                         M12_PICKER_decimals_2,
                                                         M12_PICKER_decimals_3,
                                                    0
                                            };
const char* const M_Picker_unit_table[] PROGMEM = {null,
                                                    null,
                                                         null,
                                                            M110_PICKER_unit_0,
                                                         null,
                                                            null,
                                                         null,
                                                            M112_PICKER_unit_0,
                                                    null,
                                                         M12_PICKER_unit_0,
                                                         M12_PICKER_unit_1,
                                                         M12_PICKER_unit_2,
                                                         M12_PICKER_unit_3,
                                                    null
                                            };

const float M_Picker_min_value_table[] PROGMEM = {0,
                                                    0,
                                                         0,
                                                            M110_PICKER_min_value_0,
                                                         0,
                                                            0,
                                                         0,
                                                            M112_PICKER_min_value_0,
                                                    0,
                                                         M12_PICKER_min_value_0,
                                                         M12_PICKER_min_value_1,
                                                         M12_PICKER_min_value_2,
                                                         M12_PICKER_min_value_3,
                                                    0
                                            };
const float M_Picker_max_value_table[] PROGMEM = {0,
                                                    0,
                                                         0,
                                                            M110_PICKER_max_value_0,
                                                         0,
                                                            0,
                                                         0,
                                                            M112_PICKER_max_value_0,
                                                    0,
                                                         M12_PICKER_max_value_0,
                                                         M12_PICKER_max_value_1,
                                                         M12_PICKER_max_value_2,
                                                         M12_PICKER_max_value_3,
                                                    0
                                            };

const float M_Picker_inc_short_table[] PROGMEM = {0,
                                                    0,
                                                         0,
                                                            M110_PICKER_inc_short_0,
                                                         0,
                                                            0,
                                                         0,
                                                            M112_PICKER_inc_short_0,
                                                    0,
                                                         M12_PICKER_inc_short_0,
                                                         M12_PICKER_inc_short_1,
                                                         M12_PICKER_inc_short_2,
                                                         M12_PICKER_inc_short_3,
                                                    0
                                            };
const float M_Picker_inc_long_table[] PROGMEM = {0,
                                                    0,
                                                         0,
                                                            M110_PICKER_inc_long_0,
                                                         0,
                                                            0,
                                                         0,
                                                            M112_PICKER_inc_long_0,
                                                    0,
                                                         M12_PICKER_inc_long_0,
                                                         M12_PICKER_inc_long_1,
                                                         M12_PICKER_inc_long_2,
                                                         M12_PICKER_inc_long_3,
                                                    0
                                            };
const uint16_t M_Picker_tblink_table[] PROGMEM = {0,
                                                    0,
                                                         0,
                                                            M110_PICKER_tblink_0,
                                                         0,
                                                            0,
                                                         0,
                                                            M112_PICKER_tblink_0,
                                                    0,
                                                         M12_PICKER_tblink_0,
                                                         M12_PICKER_tblink_1,
                                                         M12_PICKER_tblink_2,
                                                         M12_PICKER_tblink_3,
                                                    0
                                            };
const uint8_t M_Picker_EEPROM_ADDR_table[] PROGMEM = {0,
                                                    0,
                                                         0,
                                                            0,
                                                         0,
                                                            0,
                                                         0,
                                                            0,
                                                    0,
                                                         M12_PICKER_EEPROM_ADDR_0,
                                                         M12_PICKER_EEPROM_ADDR_1,
                                                         M12_PICKER_EEPROM_ADDR_2,
                                                         M12_PICKER_EEPROM_ADDR_3,
                                                    0
                                            };

////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////
//SCREENSAVER FUNCTION


 void print_screen_saver(LiquidCrystal_I2C *LCD,float speed,float target,float temp){
   //Print static strings
   LCD->clear();
   LCD->setCursor(0,0);
	LCD->print("hola") ;
}