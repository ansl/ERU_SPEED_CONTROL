#ifndef LCD_MENU_h
#define LCD_MENU_h
#include <Arduino.h>
#include <math.h>
#include <LiquidCrystal_I2C.h>
#include <EEPROM.h>


class Menu;
class Picker;


class contact
{ public:
		contact(uint8_t pin,uint16_t dbounce,uint16_t short_pulse,uint16_t long_pulse);
		contact(uint8_t pin,uint16_t dbounce,uint16_t short_pulse);
		uint8_t type(void);

  private:

	unsigned long _t_pulse_start;
	unsigned long _t_long_end;
  	uint8_t _pin;
  	uint16_t _dbounce;
	uint16_t _short_pulse;
	uint16_t _long_pulse;
	bool _init_pulse;
};

class Menu {
   public:
	  Menu(uint8_t n_pick,uint8_t n_header_lines, LiquidCrystal_I2C* LCD,uint8_t up,uint8_t down,uint8_t set,uint8_t back);  // simple constructor
	  Menu(uint8_t n_pick,uint8_t n_header_lines, LiquidCrystal_I2C* LCD,uint8_t up,uint8_t down,uint8_t set,uint8_t back,uint8_t next);  // simple constructor		
	  void check_button(void);
      void print_menu(void);
	  void disable_all(void); //disables all the pickers picker.enabled=0
	  void reset_value(void); //reset the picker.Value=0

	  uint8_t navigation_mode;//defines if it is a 4-5 buttons navigation mode
	  bool event; //determines if there was an change in the menu
	  uint8_t n_rows;
	  uint8_t n_cols;
	  
	  uint8_t header_lines;
      String header;
	  
	  Picker *pick;
	  uint8_t *pick_pos;
      uint8_t n_pickers;
	  uint8_t cursor_pos;

	  contact *UP;
	  contact *DOWN;
	  contact *SET;
	  contact *BACK;
	  contact *NEXT;
	  bool setup_mode;

	  unsigned long t_old;

	  LiquidCrystal_I2C *lcD;
	  
	  bool active;
	  
   private:

};


class Picker {
   public:
      Picker();
      void print_picker(void);
      String picker_name; //Picker name
      uint8_t pos;//position in the menu list
      uint8_t mode; //default type of picker =1 ; 1:next menu ->  2:on/off can be customized with the status_string 3:input variable value + unit(customizable)
	  bool enabled; //flag to define if hte picker is enabled
      bool state; //for picker type 2 0=OFF 1=0N
      String state_string0; //string for state =0
      String state_string1; //string for state =1
	  //
      float value; //value displayed on pickers type 3
	  float new_value; // variablle to store the new value during hte changing procedure
	  uint8_t decimals; //number of decimals of the Value displayed
      String unit; // Unit of value
	  float min_value;
	  float max_value;
	  uint16_t inc_short;// increment of vaue while short click
	  uint16_t inc_long;// increment of vaue while long click
    
	  bool blink; 
	  uint16_t tblink;
	  //
      Menu *child; // address of hte Child menu
	  Menu *parent; // address of hte Parent menu
	  bool EEPROM_ACTIVE;
	  uint8_t EEPROM_ADDR;
	  uint8_t ref;
   //private:
};

// class Status_screen{
// 	public:
// 		Status_screen();
// 		float Temp;
// 		float Temp_max;
// 		bool Temp_alarm;
// 		int rpm_actual;
// 		int rpm_target;
		
// };

int count_loop(int val, int max_val, int min_val);
void init_array(int arr[],int size,int res);
void init_array(uint8_t arr[],uint8_t size,uint8_t res);
void move_up(int arr[], int size);
void move_down(int arr[], int size);
void move_up(uint8_t arr[], uint8_t size);
void move_down(uint8_t arr[], uint8_t size);
int n_digit(float num);
float  round_n (float val,uint8_t dig);
float  round_n (float val,int dig);
float range_loop(float x, float _max_x,float _min_x,int decimals);
float range_loop(float x, float _max_x,float _min_x,uint8_t decimals);
int  num_disp_length (float val,uint8_t dig);
int  num_disp_length (float val,int dig);
int  num_disp_length (int val);

extern Menu *Panel;

#endif