#include "LCD_MENU.h"
#include <Arduino.h>
#include <math.h>
#include <LiquidCrystal_I2C.h>
#include <EEPROM.h>

contact::contact(int pin,int dbounce,int short_pulse,unsigned long long_pulse){
	_t_pulse_start=0;
	_t_long_end=0;
  	_pin=pin;
  	_dbounce=dbounce;
	_short_pulse=short_pulse;
	_long_pulse=long_pulse;
	_init_pulse=0;
}
contact::contact(int pin,int dbounce,int short_pulse){
	_t_pulse_start=0;
	_t_long_end=0;
  	_pin=pin;
  	_dbounce=dbounce;
	_short_pulse=short_pulse;
	_long_pulse=100000;
	_init_pulse=0;
}
int contact::type(){
	unsigned long t = millis();
	int type=0;
	
	
	if(digitalRead(_pin)==0 && _init_pulse==0){ //when a flank down is detetedit starts measuring how long is the contact pressed
		_init_pulse=1;
		_t_pulse_start=t;
    }
	else if	(digitalRead(_pin)==1 && _init_pulse==1 && abs(t-_t_pulse_start)>_dbounce && abs(t-_t_pulse_start)<=_short_pulse && abs(t-_t_long_end)>_short_pulse ){ //cond1: if the contactor is released before the debounce the value is not released until the debounce has passed and it is considered short. If it is released after de debound but not passing the limit of the short it is ocnsidered short
		_init_pulse=0;
		//Serial.print("short: ");
		//Serial.println(1);
		type=1;
		//return 1;
		
	}
	else if	(digitalRead(_pin)==1 && _init_pulse==1 && abs(t-_t_pulse_start)>_short_pulse && abs(t-_t_pulse_start)<=_long_pulse ){ //cond2: if the contactor is released before the debounce it does not return a value until the debounce has passed
		_init_pulse=0;
		//Serial.print("long: ");
		//Serial.println(2);
		_t_long_end=t;
		type=2;
		//return 2;
		
	}
	else if (_init_pulse==1 && abs(t-_t_pulse_start)>_long_pulse){//cond2: if the contactor it is pushed for more time than the short push the long push it finishes and restarts
		_init_pulse=0;
		_t_long_end=t;
		//Serial.print("long_repeticion: ");
		//Serial.println(2);
		type=2;
		//return 2;
		
	}
   
	return type;
};

Picker::Picker(){
      picker_name="Picker";
      pos=0;//initialied later when the menu constructor is called
      mode=1;// default type of picker =1 ; 1:next menu ->  2:on/off can be customized with the status_string 3:input variable value + unit(customizable)
      state=0;
      state_string0="OFF";
      state_string1="ON";
      value=0;
	  new_value=0; //New value to add to picker.value
	  decimals=0; // number of decimas to be displayed
      unit="UNK";
	  min_value=-10000;
	  max_value=10000;
	  inc_short=1;
	  inc_long=10;
      blink=0;
	  tblink=300;
	  enabled=0;
      child=0;
	  parent=0;
	  EEPROM_ACTIVE=0;
	  EEPROM_ADDR=0;
	  ref=0;

      //Menu Sub_Menu(1);
};

Menu::Menu(int n_pick, int n_header_lines, LiquidCrystal_I2C *LCD,int up, int down, int set,int back){
     
	 navigation_mode=4;//4 buttons navigation mode
	 event=0;
	 
	 header_lines=n_header_lines;
     header="Prueba";
	 
	 n_rows=LCD->_numlines;//total number of rows of the display  default 2
	 n_cols=LCD->_cols;//total number of columns of the display  default 16

     pick=new Picker[n_pick];
	 pick_pos=new int[n_pick];	
	 n_pickers=n_pick;
	 cursor_pos=0; //[0 -> (nrows-1)-header_lines]
	 
	 UP=new contact(up,100,300,600);
	 DOWN=new contact(down,100,300,600);
	 SET=new contact(set,100,300);
	 BACK=new contact(back,100,300);
	 setup_mode=0;
 	 
	 t_old=0; //t of the last blink
	 
	 lcD=LCD;
	 active=0;
 
	 
     for (int i=0;i<n_pick;i++){
         pick[i].pos=i;
		 pick_pos[i]=i;
     };

};

Menu::Menu(int n_pick, int n_header_lines, LiquidCrystal_I2C *LCD,int up, int down, int set,int back,int next){
    

	 navigation_mode=5;//5 buttons navigation mode
	 event=0;
	 
	 header_lines=n_header_lines;
     header="Prueba";
	 
	 n_rows=LCD->_numlines;//total number of rows of the display  default 2
	 n_cols=LCD->_cols;//total number of columns of the display  default 16

     pick=new Picker[n_pick];
	 pick_pos=new int[n_pick];	
	 n_pickers=n_pick;
	 cursor_pos=0; //[0 -> (nrows-1)-header_lines]
	 
	 UP=new contact(up,100,300,600);
	 DOWN=new contact(down,100,300,600);
	 SET=new contact(set,100,300);
	 BACK=new contact(back,100,300);
	 NEXT=new contact(next,100,300);
	 setup_mode=0;
 	 
	 t_old=0; //t of the last blink
	 
	 lcD=LCD;
	 active=0;
 
	 
     for (int i=0;i<n_pick;i++){
         pick[i].pos=i;
		 pick_pos[i]=i;
     };

};

void Menu::print_menu(){
	 lcD->clear();
	 
	 Serial.print("header_lines: ");
	 Serial.println(header_lines);
	 Serial.print("cursor_pos: ");
	 Serial.println(cursor_pos);
	 
	 lcD->setCursor(n_cols-1,header_lines+cursor_pos);
	 switch (pick[pick_pos[cursor_pos]].mode){
	 case 1:
		 lcD->setCursor(n_cols-1,header_lines+cursor_pos);
		 lcD->write(byte(0));
	 break;
	 case 2:
		 if (pick[pick_pos[cursor_pos]].state==0){
			lcD->setCursor(n_cols-pick[pick_pos[cursor_pos]].state_string0.length(),header_lines+cursor_pos);
			lcD->print(pick[pick_pos[cursor_pos]].state_string0);
		 }
		 else{
			lcD->setCursor(n_cols-pick[pick_pos[cursor_pos]].state_string1.length(),header_lines+cursor_pos);
			lcD->print(pick[pick_pos[cursor_pos]].state_string1);
		 }
	 break;
	 case 3:
			lcD->setCursor(n_cols-pick[pick_pos[cursor_pos]].unit.length(),header_lines+cursor_pos);
			lcD->print(pick[pick_pos[cursor_pos]].unit);
			
			if(setup_mode==0){
				//Checks if the number fits withing the gap available
				//if (n_cols-pick[pick_pos[cursor_pos]].picker_name.length()-1-num_disp_length(pick[pick_pos[cursor_pos]].value,pick[pick_pos[cursor_pos]].decimals)-1-pick[pick_pos[cursor_pos]].unit.length()>=0){
				lcD->setCursor(n_cols-pick[pick_pos[cursor_pos]].unit.length()-1-num_disp_length(pick[pick_pos[cursor_pos]].value,pick[pick_pos[cursor_pos]].decimals),header_lines+cursor_pos);
				lcD->print(round_n(pick[pick_pos[cursor_pos]].value,pick[pick_pos[cursor_pos]].decimals),pick[pick_pos[cursor_pos]].decimals);
				/* }
				//if the numberr is bigger it divides by 1000 and adds a K
				else if (n_cols-pick[pick_pos[cursor_pos]].picker_name.length()-1-num_disp_length(pick[pick_pos[cursor_pos]].value,pick[pick_pos[cursor_pos]].decimals)-1-pick[pick_pos[cursor_pos]].unit.length()<0){
				} */
			}
			else{
				//Serial.println(num_disp_length(pick[pick_pos[cursor_pos]].new_value,pick[pick_pos[cursor_pos]].decimals));
				lcD->setCursor(n_cols-pick[pick_pos[cursor_pos]].unit.length()-1-num_disp_length(pick[pick_pos[cursor_pos]].new_value,pick[pick_pos[cursor_pos]].decimals),header_lines+cursor_pos);
				lcD->print(round_n(pick[pick_pos[cursor_pos]].new_value,pick[pick_pos[cursor_pos]].decimals),pick[pick_pos[cursor_pos]].decimals);
			}
	 break;
	 }
     for (int i=0;i<min(n_rows-header_lines,n_pickers);i++){
		 
		//  Serial.print("picker pos: ");
		//  Serial.println(i);
		  
		 lcD->setCursor(0,i+header_lines);
		 
		//  Serial.print("picker name: ");
		//  Serial.println(pick[pick_pos[i]].picker_name);
		//  Serial.print("ADDRESS: ");
		//  Serial.println(int(Panel));
		 
		 lcD->print(pick[pick_pos[i]].picker_name);
	 }
	 if (header_lines>0){
		lcD->setCursor(0,0);
		lcD->print(header);

	 }
	 
	 

};
void Menu::check_button(void){
		event=0;
		int up_type=UP->type();
		int down_type=DOWN->type();
		int set_type=SET->type();
		int back_type=BACK->type();
		int next_type=NEXT->type();
		/* Serial.print("UP type: ");
		Serial.println(up_type);
		Serial.print("DOWN type: ");
		Serial.println(down_type);
		Serial.print("SET type: ");
		Serial.println(set_type); */
		
		//blink routine//
		unsigned long t=millis();
		if(setup_mode==1 && abs(t-t_old)>pick[pick_pos[cursor_pos]].tblink && pick[pick_pos[cursor_pos]].mode==3){
			
			/* Serial.print("BLINK: ");
			Serial.println(pick[pick_pos[cursor_pos]].blink);
			Serial.println(abs(t-t_old)); */
			
			pick[pick_pos[cursor_pos]].blink=!pick[pick_pos[cursor_pos]].blink;
			t_old=t;
			if(pick[pick_pos[cursor_pos]].blink==1){
				//Serial.println(num_disp_length(pick[pick_pos[cursor_pos]].new_value,pick[pick_pos[cursor_pos]].decimals));
				lcD->setCursor(n_cols-pick[pick_pos[cursor_pos]].unit.length()-1-num_disp_length(pick[pick_pos[cursor_pos]].new_value,pick[pick_pos[cursor_pos]].decimals),header_lines+cursor_pos);
				lcD->print(round_n(pick[pick_pos[cursor_pos]].new_value,pick[pick_pos[cursor_pos]].decimals),pick[pick_pos[cursor_pos]].decimals);
		
			}
			if(pick[pick_pos[cursor_pos]].blink==0){
				int n_dig=n_digit(pick[pick_pos[cursor_pos]].new_value);
				String empty;
				for (int i=0;i<num_disp_length(pick[pick_pos[cursor_pos]].new_value,pick[pick_pos[cursor_pos]].decimals);i++){
					empty=empty+" ";
				}
				lcD->setCursor(n_cols-pick[pick_pos[cursor_pos]].unit.length()-1-num_disp_length(pick[pick_pos[cursor_pos]].new_value,pick[pick_pos[cursor_pos]].decimals),header_lines+cursor_pos);
				lcD->print(empty);
			}

		}
		
		//button routines//
		if (up_type==1){
			//Serial.print("setup_mode: ");
			//Serial.println(setup_mode);
			if(setup_mode==0){
				if(cursor_pos>0){					
					cursor_pos--;
				}
				else if (cursor_pos==0){
					move_down(pick_pos, n_pickers);
				}
			}
			else{ 
				pick[pick_pos[cursor_pos]].new_value=pick[pick_pos[cursor_pos]].new_value+pick[pick_pos[cursor_pos]].inc_short;
				if (pick[pick_pos[cursor_pos]].new_value>pick[pick_pos[cursor_pos]].max_value){
					pick[pick_pos[cursor_pos]].new_value=pick[pick_pos[cursor_pos]].max_value;
				}

			}
			print_menu();
		}
		if (up_type==2){
			if(setup_mode==1){
				pick[pick_pos[cursor_pos]].new_value=pick[pick_pos[cursor_pos]].new_value+pick[pick_pos[cursor_pos]].inc_long;
				if (pick[pick_pos[cursor_pos]].new_value>pick[pick_pos[cursor_pos]].max_value){
					pick[pick_pos[cursor_pos]].new_value=pick[pick_pos[cursor_pos]].max_value;
				}

			}
			print_menu();
		}		
		if (down_type==1){
			if(setup_mode==0){
				if(cursor_pos+header_lines<min(n_pickers+header_lines,n_rows)-1){
					cursor_pos++;
				}
				else if (cursor_pos+header_lines==n_rows-1){
					move_up(pick_pos, n_pickers);
				}
			}
			else{ 
				pick[pick_pos[cursor_pos]].new_value=pick[pick_pos[cursor_pos]].new_value-pick[pick_pos[cursor_pos]].inc_short;
				if (pick[pick_pos[cursor_pos]].new_value<pick[pick_pos[cursor_pos]].min_value){
					pick[pick_pos[cursor_pos]].new_value=pick[pick_pos[cursor_pos]].min_value;
				}

			}
			print_menu();
		}	
		if (down_type==2){
			if(setup_mode==1){
				pick[pick_pos[cursor_pos]].new_value=pick[pick_pos[cursor_pos]].new_value-pick[pick_pos[cursor_pos]].inc_long;
				if (pick[pick_pos[cursor_pos]].new_value<pick[pick_pos[cursor_pos]].min_value){
					pick[pick_pos[cursor_pos]].new_value=pick[pick_pos[cursor_pos]].min_value;
				}

			}
			print_menu();
		}		
		switch (navigation_mode) {
			case 4:
				if (set_type==1){
					switch(pick[pick_pos[cursor_pos]].mode){
						case 1:
								if (pick[pick_pos[cursor_pos]].child!=0){
									active=0;
									reset_value();
									
									Panel=pick[pick_pos[cursor_pos]].child;
									event=1;
									Panel->active=1;
								}
								
								/* Serial.print("child adress: ");
								Serial.println(int(pick[pick_pos[cursor_pos]].child));
								Serial.print("Panel adress: ");
								Serial.println(int(Panel)); */
								break;
						case 2:
								pick[pick_pos[cursor_pos]].state=!pick[pick_pos[cursor_pos]].state;
								event=1;
								break;
						case 3:
								break;		
					}
					Panel->print_menu();				
				}
				if (set_type==2){
					switch(pick[pick_pos[cursor_pos]].mode){
						case 1:
								break;
						case 2:
								break;
						case 3:
								setup_mode=!setup_mode;
								//Serial.println(setup_mode);	
								if (setup_mode==1){
									//new_value=pick[pick_pos[cursor_pos]].value;
								}
								else {
									pick[pick_pos[cursor_pos]].value=pick[pick_pos[cursor_pos]].new_value;
									//pick[pick_pos[cursor_pos]].new_value=0;
									if (pick[pick_pos[cursor_pos]].EEPROM_ACTIVE==1){
										EEPROM.put(pick[pick_pos[cursor_pos]].EEPROM_ADDR, pick[pick_pos[cursor_pos]].value);
									}
									pick[pick_pos[cursor_pos]].enabled=1;
									event=1;
								}
								break;		
					}			
				print_menu();				
			}
				if (back_type==1){
					switch(pick[pick_pos[cursor_pos]].mode){
						case 1:
								if (pick[pick_pos[cursor_pos]].parent!=0){
									active=0;
									reset_value();
									setup_mode=0;
									Panel=pick[pick_pos[cursor_pos]].parent;
									Panel->active=1;
									//event=1;
								}
								break;
						case 2:
								if (pick[pick_pos[cursor_pos]].parent!=0){
									active=0;
									reset_value();
									setup_mode=0;
									Panel=pick[pick_pos[cursor_pos]].parent;
									Panel->active=1;
									//event=1;
								}
								break;
						case 3:
								if (pick[pick_pos[cursor_pos]].parent!=0){
									active=0;
									reset_value();
									setup_mode=0;
									Panel=pick[pick_pos[cursor_pos]].parent;
									Panel->active=1;
									//event=1;
								}
								break;		
					}			
					Panel->print_menu();				
				}
				if (back_type==2){
					switch(pick[pick_pos[cursor_pos]].mode){
						case 1:						
								break;
						case 2:
								break;
						case 3:
								break;		
					}			
					Panel->print_menu();				
				}

			break;
			case 5:
				if (set_type==1){
					switch(pick[pick_pos[cursor_pos]].mode){
						case 1:
								break;
						case 2:
								pick[pick_pos[cursor_pos]].state=!pick[pick_pos[cursor_pos]].state;
								event=1;
								break;
						case 3:
								setup_mode=!setup_mode;
								//Serial.println(setup_mode);	
								if (setup_mode==1){
									//new_value=pick[pick_pos[cursor_pos]].value;
								}
								else {
									pick[pick_pos[cursor_pos]].value=pick[pick_pos[cursor_pos]].new_value;
									if (pick[pick_pos[cursor_pos]].EEPROM_ACTIVE==1){
										EEPROM.put(pick[pick_pos[cursor_pos]].EEPROM_ADDR, pick[pick_pos[cursor_pos]].value);
									}
									//pick[pick_pos[cursor_pos]].new_value=0;
									pick[pick_pos[cursor_pos]].enabled=1;
									event=1;
								}
								break;			
					}
					Panel->print_menu();				
				}
				if (set_type==2){
					switch(pick[pick_pos[cursor_pos]].mode){
						case 1:
								break;
						case 2:
								break;
						case 3:
								reset_value();
								event=1;
								break;		
				}			
				print_menu();				
			}
				if (next_type==1){
					switch(pick[pick_pos[cursor_pos]].mode){
						case 1:
								if (pick[pick_pos[cursor_pos]].child!=0){
									active=0;
									//reset_value();
									Panel=pick[pick_pos[cursor_pos]].child;
									event=1;
									Panel->active=1;
								}
								break;
						case 2:
								break;
						case 3:
								break;		
					}
					Panel->print_menu();				
				}
				if (next_type==2){
					switch(pick[pick_pos[cursor_pos]].mode){
						case 1:
								if (pick[pick_pos[cursor_pos]].child!=0){
									active=0;
									//reset_value();
									Panel=pick[pick_pos[cursor_pos]].child;
									event=1;
									Panel->active=1;
								}
								break;
						case 2:
								break;
						case 3:
								break;		
					}			
					Panel->print_menu();				
			}
				if (back_type==1){
					switch(pick[pick_pos[cursor_pos]].mode){
						case 1:
								if (pick[pick_pos[cursor_pos]].parent!=0){
									active=0;
									//reset_value();
									setup_mode=0;
									Panel=pick[pick_pos[cursor_pos]].parent;
									Panel->active=1;
									event=1;
								}
								break;
						case 2:
								if (pick[pick_pos[cursor_pos]].parent!=0){
									active=0;
									//reset_value();
									setup_mode=0;
									Panel=pick[pick_pos[cursor_pos]].parent;
									Panel->active=1;
									event=1;
								}
								break;
						case 3:
								if (pick[pick_pos[cursor_pos]].parent!=0){
									active=0;
									//reset_value();
									setup_mode=0;
									Panel=pick[pick_pos[cursor_pos]].parent;
									Panel->active=1;
									event=1;
								}
								break;		
					}			
					Panel->print_menu();				
				}
				if (back_type==2){
					switch(pick[pick_pos[cursor_pos]].mode){
						case 1:
								if (pick[pick_pos[cursor_pos]].parent!=0){
									active=0;
									//reset_value();
									setup_mode=0;
									Panel=pick[pick_pos[cursor_pos]].parent;
									Panel->active=1;
									event=1;
								}
								break;
						case 2:
								if (pick[pick_pos[cursor_pos]].parent!=0){
									active=0;
									//reset_value();
									setup_mode=0;
									Panel=pick[pick_pos[cursor_pos]].parent;
									Panel->active=1;
									event=1;
								}
								break;
						case 3:
								if (pick[pick_pos[cursor_pos]].parent!=0){
									active=0;
									//reset_value();
									setup_mode=0;
									Panel=pick[pick_pos[cursor_pos]].parent;
									Panel->active=1;
									event=1;
								}
								break;				
					}			
					Panel->print_menu();				
				}
			break;
		};
};
void Menu::disable_all(void){
	for (int i=0;i<n_pickers;i++){
		pick[i].enabled=0;
	}
}
void Menu::reset_value(void){
	for (int i=0;i<n_pickers;i++){
		pick[i].value=0;
	}
	
}

void init_array(int arr[],int size,int res){
    //int n = sizeof(arr) / sizeof(int);
    for(int i=0;i<size;i++){
        arr[i]=res;
    }
};
void move_up(int arr[], int size){
    int buff=arr[0];
    for (int i=0;i<size-1;i++){
        arr[i]=arr[i+1] ;
    }
    arr[size-1]=buff;
}
void move_down(int arr[], int size){
    int buff=arr[size-1];
    for (int i=size-1;i>0;i--){
        arr[i]=arr[i-1] ;
    }
    arr[0]=buff;
}
int n_digit(int num){
	int n=0;
	if (num==0) {
		n=1;
	}
	
	else if (num>0){
		while (num>0)
		{
			num=num/10;
			n++;
		}
	}
	else if (num<0){
		while (num<0)
		{
			num=num/10;
			n++;
		}
		n+1;
	}
    return n;
}
int n_digit(float num){
	//Serial.println("n_digit:");
	int n=0;
	long numm=long(trunc(num));
	//Serial.println(numm);
	if (numm==0) {
		n=1;
	}
	
	else if (numm>0){
		while (numm>0)
		{
			numm=numm/10;
			n++;
		}
	}
	else if (numm<0){
		while (numm<0)
		{
			numm=numm/10;
			n++;
		}
		n+1;
	}
    return n;
	//Serial.println("/n_digit");
}
float  round_n (float val,int dig){
	//Serial.println("round_n:");
	float r_val=trunc(round(val*pow(10.0,dig)))/pow(10.0,dig);
	//Serial.println(r_val);
	return r_val;
	//Serial.println("/round_n:");
}
int  num_disp_length (float val,int dig){
	//Serial.println("num_disp_length:");
	long n=n_digit(val)+dig;
	if (dig>0){n++;};
	if (val<0) {n++;};
	//Serial.println(n);
	return n;
	//Serial.println("/num_disp_length:");
}
float range_loop(float x, float _max_x,float _min_x,int decimals){
	  if(x>=_max_x){
		  return round_n(_min_x+(x-_max_x)-(_max_x-_min_x)*trunc((x-_max_x)/(_max_x-_min_x)),decimals);      
	  }
	  else if( x<_min_x){
	  return round_n(_max_x-(_min_x-x)-(_max_x-_min_x)*trunc((x-_min_x)/(_max_x-_min_x)),decimals);
	  }
	  else { return round_n(x,decimals);}
}

// predefined global entities 

Menu *Panel;