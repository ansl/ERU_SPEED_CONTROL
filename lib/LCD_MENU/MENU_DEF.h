
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
uint16_t screen_saver_delay=2500;// time to popup the screensaver (02)

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


    
    const uint8_t PROGMEM M1_PICKER_mode_0 = 1;//default type of picker =1 ; 1:next menu ->  2:on/off can be customized with the status_string 3:input variable value + unit(customizable)
    const char PROGMEM M1_PICKER_name_0[]  = "Spindle setup";

           const char PROGMEM M11_HEADER[]  = "SPINDLE CONTROL"; // "String 0" etc are strings to store - change to suit.
                const uint8_t PROGMEM M11_PICKER_mode_0 = 1;//default type of picker =1 ; 1:next menu ->  2:on/off can be customized with the status_string 3:input variable value + unit(customizable)
                const char PROGMEM M11_PICKER_name_0[]  = "Target speed";
                    const char PROGMEM M110_HEADER[]  = "TARGET SPEED";
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
                const char PROGMEM M11_PICKER_name_1[]  = "Ext pwm"; 
                    const char PROGMEM M111_HEADER[]  = "ERU PWM SOURCE:";
                        const uint8_t PROGMEM M111_PICKER_mode_0 = 2;
                        const char PROGMEM M111_PICKER_name_0[]  = "Link:"; 
                        const char PROGMEM M111_PICKER_state_string0_0[]  = "Disconnected"; 
                        const char PROGMEM M111_PICKER_state_string1_0[]  = "Connected"; 

    const uint8_t PROGMEM M1_PICKER_mode_1 = 2;//default type of picker =1 ; 1:next menu ->  2:on/off can be customized with the status_string 3:input variable value + unit(customizable)
    const char PROGMEM M1_PICKER_name_1[]  = "Light"; 
    const char PROGMEM M1_PICKER_state_string0_1[]  = "off"; 
    const char PROGMEM M1_PICKER_state_string1_1[]  = "on"; 

    const uint8_t PROGMEM M1_PICKER_mode_2 = 2;//default type of picker =1 ; 1:next menu ->  2:on/off can be customized with the status_string 3:input variable value + unit(customizable)
    const char PROGMEM M1_PICKER_name_2[]  = "Coolant"; 
    const char PROGMEM M1_PICKER_state_string0_2[]  = "off"; 
    const char PROGMEM M1_PICKER_state_string1_2[]  = "on"; 

    const uint8_t PROGMEM M1_PICKER_mode_3 = 1;//default type of picker =1 ; 1:next menu ->  2:on/off can be customized with the status_string 3:input variable value + unit(customizable)
    const char PROGMEM M1_PICKER_name_3[]  = "Configuration"; 
        const char PROGMEM M12_HEADER[]  = "CONFIGURATION";     

            const uint8_t PROGMEM M12_PICKER_mode_0 = 3;
            const char PROGMEM M12_PICKER_name_0[]  = "T max:"; 
            const uint8_t PROGMEM M12_PICKER_decimals_0 = 1; 
            const char PROGMEM M12_PICKER_unit_0[]  = ""; 
            const float PROGMEM M12_PICKER_min_value_0 = 0; 
            const float PROGMEM M12_PICKER_max_value_0 = 200; 
            const float PROGMEM M12_PICKER_inc_short_0 = 1; 
            const float PROGMEM M12_PICKER_inc_long_0 = 10; 
            const uint16_t PROGMEM M12_PICKER_tblink_0 = 300;
            const uint8_t PROGMEM M12_PICKER_EEPROM_ADDR_0 = 13;        

    



#endif
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////
//MENU MATRIXES
const PROGMEM char* const M_Header_table[] = {M1_HEADER,
                                                M11_HEADER,
                                                    M110_HEADER, M111_HEADER, 
                                                M12_HEADER
                                            };

const uint8_t M_Picker_mode_table[] PROGMEM = {     M1_PICKER_mode_0,
                                                         M11_PICKER_mode_0,
                                                            M110_PICKER_mode_0,
                                                         M11_PICKER_mode_1,
                                                            M111_PICKER_mode_0,
                                                    M1_PICKER_mode_1,
                                                    M1_PICKER_mode_2,
                                                    M1_PICKER_mode_3,
                                                         M12_PICKER_mode_0,
                                            };
const char* const M_Picker_name_table[] PROGMEM = {
                                                    M1_PICKER_name_0,//spindle control
                                                         M11_PICKER_name_0, //Speed target
                                                            M110_PICKER_name_0,//Speed target
                                                         M11_PICKER_name_1,//EXT PWM
                                                            M111_PICKER_name_0,//ERU PWM SOURCE
                                                    M1_PICKER_name_1,//light
                                                    M1_PICKER_name_2,//coolant
                                                    M1_PICKER_name_3,//config
                                                         M12_PICKER_name_0,//Tmax
                                            };

const char* const M_Picker_state_string0_table[] PROGMEM = {
                                                    null,//spindle control
                                                         null,//Speed target
                                                            null,//Speed target
                                                         null,//EXT PWM
                                                            M111_PICKER_state_string0_0,//ERU PWM SOURCE
                                                    M1_PICKER_state_string0_1,//light
                                                    M1_PICKER_state_string0_2,//coolant
                                                    null,//config
                                                         null//Tmax
                                            };
const char* const M_Picker_state_string1_table[] PROGMEM = {
                                                   null,//spindle control
                                                         null,//Speed target
                                                            null,//Speed target
                                                         null,//EXT PWM
                                                            M111_PICKER_state_string1_0,//ERU PWM SOURCE
                                                    M1_PICKER_state_string1_1,//light
                                                    M1_PICKER_state_string1_2,//coolant
                                                    null,//config
                                                         null//Tmax
                                            };

const uint8_t M_Picker_decimals_table[] PROGMEM = {
                                                    0,//spindle control
                                                         0,//Speed target
                                                            M110_PICKER_decimals_0,//Speed target
                                                         0,//EXT PWM
                                                            0,//ERU PWM SOURCE
                                                    0,//light
                                                    0,//coolant
                                                    0,//config
                                                         M12_PICKER_decimals_0//Tmax
                                            };
const char* const M_Picker_unit_table[] PROGMEM = {
                                                    null,//spindle control
                                                         null,//Speed target
                                                            M110_PICKER_unit_0,//Speed target
                                                         null,//EXT PWM
                                                            null,//ERU PWM SOURCE
                                                    null,//light
                                                    null,//coolant
                                                    null,//config
                                                         M12_PICKER_unit_0//Tmax
                                            };

const float M_Picker_min_value_table[] PROGMEM = {
                                                   0,//spindle control
                                                         0,//Speed target
                                                            M110_PICKER_min_value_0,//Speed target
                                                         0,//EXT PWM
                                                            0,//ERU PWM SOURCE
                                                    0,//light
                                                    0,//coolant
                                                    0,//config
                                                         M12_PICKER_min_value_0//Tmax
                                            };
const float M_Picker_max_value_table[] PROGMEM = {
                                                   0,//spindle control
                                                         0,//Speed target
                                                            M110_PICKER_max_value_0,//Speed target
                                                         0,//EXT PWM
                                                            0,//ERU PWM SOURCE
                                                    0,//light
                                                    0,//coolant
                                                    0,//config
                                                         M12_PICKER_max_value_0//Tmax
                                            };

const float M_Picker_inc_short_table[] PROGMEM = {
                                                    0,//spindle control
                                                         0,//Speed target
                                                            M110_PICKER_inc_short_0,//Speed target
                                                         0,//EXT PWM
                                                            0,//ERU PWM SOURCE
                                                    0,//light
                                                    0,//coolant
                                                    0,//config
                                                         M12_PICKER_inc_short_0//Tmax
                                            };
const float M_Picker_inc_long_table[] PROGMEM = {
                                                    0,//spindle control
                                                         0,//Speed target
                                                            M110_PICKER_inc_long_0,//Speed target
                                                         0,//EXT PWM
                                                            0,//ERU PWM SOURCE
                                                    0,//light
                                                    0,//coolant
                                                    0,//config
                                                         M12_PICKER_inc_long_0//Tmax
                                            };
const uint16_t M_Picker_tblink_table[] PROGMEM =  {
                                                    0,//spindle control
                                                         0,//Speed target
                                                            M110_PICKER_tblink_0,//Speed target
                                                         0,//EXT PWM
                                                            0,//ERU PWM SOURCE
                                                    0,//light
                                                    0,//coolant
                                                    0,//config
                                                         M12_PICKER_tblink_0//Tmax
                                            };
const uint8_t M_Picker_EEPROM_ADDR_table[] PROGMEM = {
                                                    0,//spindle control
                                                         0,//Speed target
                                                            0,//Speed target
                                                         0,//EXT PWM
                                                            0,//ERU PWM SOURCE
                                                    0,//light
                                                    0,//coolant
                                                    0,//config
                                                         M12_PICKER_EEPROM_ADDR_0//Tmax
                                            };


////////////////////////////////////////////////////////////////

