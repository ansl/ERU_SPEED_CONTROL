#ifndef LCD_MENU_h
#define LCD_MENU_h
#include <Arduino.h>
#include <math.h>
#include <LiquidCrystal_I2C.h>


class Menu;
class Picker;


class contact
{ public:
		contact(int pin,int dbounce,int short_pulse,int long_pulse);
		contact(int pin,int dbounce,int short_pulse);
		int type(void);

  private:

	unsigned long _t_pulse_start;
	unsigned long _t_long_end;
  	int _pin;
  	int _dbounce;
	int _short_pulse;
	int _long_pulse;
	bool _init_pulse;
};

class Menu {
   public:
	  Menu(int n_pick,int n_header_lines, LiquidCrystal_I2C* LCD,int up,int down,int set,int back);  // simple constructor
	  Menu(int n_pick,int n_header_lines, LiquidCrystal_I2C* LCD,int up,int down,int set,int back,int next);  // simple constructor		
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
	  int *pick_pos;
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
      int pos;//position in the menu list
      int mode; //default type of picker =1 ; 1:next menu ->  2:on/off can be customized with the status_string 3:input variable value + unit(customizable)
	  bool enabled; //flag to define if hte picker is enabled
      bool state; //for picker type 2 0=OFF 1=0N
      String state_string0; //string for state =0
      String state_string1; //string for state =1
	  //
      float value; //value displayed on pickers type 3
	  float new_value; // variablle to store the new value during hte changing procedure
	  int decimals; //number of decimals of the Value displayed
      String unit; // Unit of value
	  float min_value;
	  float max_value;
	  float inc_short;// increment of vaue while short click
	  float inc_long;// increment of vaue while long click
      
	  bool blink; 
	  int tblink;
	  //
      Menu *child; // address of hte Child menu
	  Menu *parent; // address of hte Parent menu
   //private:
};


int count_loop(int val, int max_val, int min_val);
void init_array(int arr[],int size,int res);
void move_up(int arr[], int size);
void move_down(int arr[], int size);
int n_digit(int num);
int n_digit(float num);
float  round_n (float val,int dig);
float range_loop(float x, float _max_x,float _min_x,int decimals);
int  num_disp_length (float val,int dig);
int  num_disp_length (int val);

extern Menu *Panel;

#endif