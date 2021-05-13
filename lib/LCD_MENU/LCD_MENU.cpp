#include <LCD_MENU.h>
#include <Arduino.h>
#include <math.h>
#include <LiquidCrystal_I2C.h>
#include <MENU_DEF.h>
#ifndef EEPROM_h
#include <EEPROM.h>
#endif

contact::contact(uint8_t pin,uint16_t dbounce,uint16_t short_pulse,uint16_t long_pulse){
	_t_pulse_start=0;
	_t_long_end=0;
  	_pin=pin;
  	_dbounce=dbounce;
	_short_pulse=short_pulse;
	_long_pulse=long_pulse;
	_init_pulse=0;
}
contact::contact(uint8_t pin,uint16_t dbounce,uint16_t short_pulse){
	_t_pulse_start=0;
	_t_long_end=0;
  	_pin=pin;
  	_dbounce=dbounce;
	_short_pulse=short_pulse;
	_long_pulse=100000;
	_init_pulse=0;
}
uint8_t contact::type(){
	unsigned long t = millis();
	uint8_t type=0;
		// Serial.print("dbounce: ");
		// Serial.println(_dbounce);
		// Serial.print("short_pulse: ");
		// Serial.println(_short_pulse,DEC);
	
	if(digitalRead(_pin)==0 && _init_pulse==0){ //when a flank down is detetedit starts measuring how long is the contact pressed
		_init_pulse=1;
		_t_pulse_start=t;
    }
	else if	(digitalRead(_pin)==1 && _init_pulse==1 && abs(t-_t_pulse_start)>_dbounce && abs(t-_t_pulse_start)<=_short_pulse && abs(t-_t_long_end)>_short_pulse ){ //cond1: if the contactor is released before the debounce the value is not released until the debounce has passed and it is considered short. If it is released after de debound but not passing the limit of the short it is ocnsidered short
		_init_pulse=0;
		// Serial.print("short: ");
		// Serial.println(1);
		type=1;
		//return 1;
		
	}
	else if	(digitalRead(_pin)==1 && _init_pulse==1 && abs(t-_t_pulse_start)>_short_pulse && abs(t-_t_pulse_start)<=_long_pulse ){ //cond2: if the contactor is released before the debounce it does not return a value until the debounce has passed
		_init_pulse=0;
		// Serial.print("long: ");
		// Serial.println(2);
		_t_long_end=t;
		type=2;
		//return 2;
		
	}
	else if (_init_pulse==1 && abs(t-_t_pulse_start)>_long_pulse){//cond2: if the contactor it is pushed for more time than the short push the long push it finishes and restarts
		_init_pulse=0;
		_t_long_end=t;
		// Serial.print("long_repeticion: ");
		// Serial.println(2);
		type=2;
		//return 2;
		
	}
   
	return type;
};
Picker::Picker(){
    //   picker_name=F("");
      pos=0;//initialied later when the menu constructor is called
    //   mode=1;// default type of picker =1 ; 1:next menu ->  2:on/off can be customized with the status_string 3:input variable value + unit(customizable)
      state=0;
    //   state_string0=F("OFF");
    //   state_string1=F("ON");
      value=0;
	  new_value=0; //New value to add to picker.value
	//   decimals=0; // number of decimas to be displayed
    //   unit=F("UNK");
	//   min_value=-10000;
	//   max_value=10000;
	//   inc_short=1;
	//   inc_long=10;
      blink=0;
	//   tblink=300;
	  enabled=0;
      child=0;
	  parent=0;
	  EEPROM_ACTIVE=0;
	//   EEPROM_ADDR=0;
	  ref=0;

      //Menu Sub_Menu(1);
};
void Picker::get_val_EEPROM(){
	if(EEPROM_ACTIVE==1){
		EEPROM.get((uint8_t)pgm_read_byte(&M_Picker_EEPROM_ADDR_table[ref]),value);
	}
}

Menu::Menu(uint8_t n_pick, uint8_t n_header_lines, LiquidCrystal_I2C *LCD,uint8_t up, uint8_t down, uint8_t set,uint8_t back){
     
	 menu_ref=0;
	 navigation_mode=4;//4 buttons navigation mode
	 event=0;
	 menu_flag=1;
	 header_lines=n_header_lines;
    //  header=F("");
	 
	 n_rows=LCD->_rows;//total number of rows of the display  default 2
	 n_cols=LCD->_cols;//total number of columns of the display  default 16

     pick=new Picker[n_pick];
	 pick_pos=new uint8_t[n_pick];	
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
 
	 
     for (uint8_t i=0;i<n_pick;i++){
         pick[i].pos=i;
		 pick_pos[i]=i;
     };

};
Menu::Menu(uint8_t n_pick, uint8_t n_header_lines, LiquidCrystal_I2C *LCD,uint8_t up, uint8_t down, uint8_t set,uint8_t back,uint8_t next){
    
	 menu_ref=0;
	 navigation_mode=5;//5 buttons navigation mode
	 event=0;
	 menu_flag=1;
	 
	 header_lines=n_header_lines;
    //  header=F("");
	 
	 n_rows=LCD->_rows;//total number of rows of the display  default 2
	 n_cols=LCD->_cols;//total number of columns of the display  default 16

     pick=new Picker[n_pick];
	 pick_pos=new uint8_t[n_pick];	
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
 
	 
     for (uint8_t i=0;i<n_pick;i++){
         pick[i].pos=i;
		 pick_pos[i]=i;
     };

};

void Menu::print_menu(){
	strcpy_P(MENU_Header, (char*)pgm_read_word(&(M_Header_table[menu_ref])));
	PICKER_mode=(uint8_t)pgm_read_byte(&M_Picker_mode_table[pick[pick_pos[cursor_pos]].ref]);
	// strcpy_P(PICKER_name, (char*)pgm_read_word(&(M_Picker_name_table[pick[pick_pos[cursor_pos]].ref]))); /moved later to print all the pickers
	strcpy_P(PICKER_state_string0, (char*)pgm_read_word(&(M_Picker_state_string0_table[pick[pick_pos[cursor_pos]].ref])));
	strcpy_P(PICKER_state_string1, (char*)pgm_read_word(&(M_Picker_state_string1_table[pick[pick_pos[cursor_pos]].ref])));
	PICKER_decimals=(uint8_t)pgm_read_byte(&M_Picker_decimals_table[pick[pick_pos[cursor_pos]].ref]);
	strcpy_P(PICKER_unit, (char*)pgm_read_word(&(M_Picker_unit_table[pick[pick_pos[cursor_pos]].ref])));
	PICKER_min_value=pgm_read_float(&M_Picker_min_value_table[pick[pick_pos[cursor_pos]].ref]);
	PICKER_max_value=pgm_read_float(&M_Picker_max_value_table[pick[pick_pos[cursor_pos]].ref]);
	PICKER_inc_short=pgm_read_float(&M_Picker_inc_short_table[pick[pick_pos[cursor_pos]].ref]);
	PICKER_inc_long=pgm_read_float(&M_Picker_inc_long_table[pick[pick_pos[cursor_pos]].ref]);
	PICKER_tblink=(uint16_t)pgm_read_word(&M_Picker_tblink_table[pick[pick_pos[cursor_pos]].ref]);
	PICKER_EEPROM_ADDR=(uint8_t)pgm_read_byte(&M_Picker_EEPROM_ADDR_table[pick[pick_pos[cursor_pos]].ref]);

// Serial.println("MENU SETTINGS:");
// Serial.println(PICKER_mode);
// Serial.println(PICKER_name);
// Serial.println(PICKER_state_string0);
// Serial.println(PICKER_state_string1);
// Serial.println(PICKER_decimals);
// Serial.println(PICKER_unit);
// Serial.println(PICKER_min_value);
// Serial.println(PICKER_max_value);
// Serial.println(PICKER_inc_short);
// Serial.println(PICKER_inc_long);
// Serial.println(PICKER_tblink);
// Serial.println(PICKER_EEPROM_ADDR);

	 lcD->clear();
	 lcD->setCursor(n_cols-1,header_lines+cursor_pos);
	 switch (PICKER_mode){
	 case 1:
		 lcD->setCursor(n_cols-1,header_lines+cursor_pos);
		 lcD->write(0);
	 break;
	 case 2:
		 if (pick[pick_pos[cursor_pos]].state==0){
			// Serial.println(PICKER_state_string0);
			lcD->setCursor(n_cols-strlen(PICKER_state_string0),header_lines+cursor_pos);
			lcD->print(PICKER_state_string0);
		 }
		 else{
			// Serial.println(PICKER_state_string1);
			lcD->setCursor(n_cols-strlen(PICKER_state_string1),header_lines+cursor_pos);
			lcD->print(PICKER_state_string1);
		 }
	 break;
	 case 3:
			lcD->setCursor(n_cols-strlen(PICKER_unit),header_lines+cursor_pos);
			lcD->print(PICKER_unit);
			
			if(setup_mode==0){
				//Checks if the number fits withing the gap available
				//if (n_cols-pick[pick_pos[cursor_pos]].picker_name.length()-1-num_disp_length(pick[pick_pos[cursor_pos]].value,PICKER_decimals)-1-strlen(PICKER_unit)>=0){
				lcD->setCursor(n_cols-strlen(PICKER_unit)-1-num_disp_length(pick[pick_pos[cursor_pos]].value,PICKER_decimals),header_lines+cursor_pos);
				lcD->print(round_n(pick[pick_pos[cursor_pos]].value,PICKER_decimals),PICKER_decimals);
				/* }
				//if the numberr is bigger it divides by 1000 and adds a K
				else if (n_cols-pick[pick_pos[cursor_pos]].picker_name.length()-1-num_disp_length(pick[pick_pos[cursor_pos]].value,PICKER_decimals)-1-strlen(PICKER_unit)<0){
				} */
			}
			else{
				//Serial.println(num_disp_length(pick[pick_pos[cursor_pos]].new_value,PICKER_decimals));
				lcD->setCursor(n_cols-strlen(PICKER_unit)-1-num_disp_length(pick[pick_pos[cursor_pos]].new_value,PICKER_decimals),header_lines+cursor_pos);
				lcD->print(round_n(pick[pick_pos[cursor_pos]].new_value,PICKER_decimals),PICKER_decimals);
			}
	 break;
	 }

     for (int i=0;i<min(n_rows-header_lines,n_pickers);i++){
		 strcpy_P(PICKER_name, (char*)pgm_read_word(&(M_Picker_name_table[pick[pick_pos[i]].ref])));
		// Serial.println(PICKER_name);
		
		 lcD->setCursor(0,i+header_lines);
		 lcD->print(PICKER_name);
	 }
	 if (header_lines>0){
		lcD->setCursor(0,0);
		lcD->print(MENU_Header);

	 }

};

void Menu::check_button(void){
		event=0;
		uint8_t up_type=UP->type();
		uint8_t down_type=DOWN->type();
		uint8_t set_type=SET->type();
		uint8_t back_type=BACK->type();
		uint8_t next_type=NEXT->type();

		// Serial.println((up_type+down_type+set_type+back_type+next_type));
		if ((up_type+down_type+set_type+back_type+next_type)>0){
			if (menu_flag==0){
				up_type=0;
				down_type=0;
				set_type=0;
				back_type=0;
				event=1;
			}
			t_last_button_input=millis();
			menu_flag=1;

		}
		
		if((millis()-t_last_button_input)<screen_saver_delay){
			menu_flag=1;
		}
		else{
			menu_flag=0;
		}

		// Serial.print("UP type: ");
		// Serial.println(up_type);
		// Serial.print("DOWN type: ");
		// Serial.println(down_type);
		// Serial.print("SET type: ");
		// Serial.println(set_type);
		
		//print menu

		//blink routine//
		unsigned long t=millis();
		if(setup_mode==1 && menu_flag==1 && abs(t-t_old)>PICKER_tblink && PICKER_mode==3){
			
			/* Serial.print("BLINK: ");
			Serial.println(pick[pick_pos[cursor_pos]].blink);
			Serial.println(abs(t-t_old)); */
			
			pick[pick_pos[cursor_pos]].blink=!pick[pick_pos[cursor_pos]].blink;
			t_old=t;
			if(pick[pick_pos[cursor_pos]].blink==1){
				//Serial.println(num_disp_length(pick[pick_pos[cursor_pos]].new_value,PICKER_decimals));
				lcD->setCursor(n_cols-strlen(PICKER_unit)-1-num_disp_length(pick[pick_pos[cursor_pos]].new_value,PICKER_decimals),header_lines+cursor_pos);
				lcD->print(round_n(pick[pick_pos[cursor_pos]].new_value,PICKER_decimals),PICKER_decimals);
		
			}
			if(pick[pick_pos[cursor_pos]].blink==0){
				int n_dig=n_digit(pick[pick_pos[cursor_pos]].new_value);
				String empty;
				for (int i=0;i<num_disp_length(pick[pick_pos[cursor_pos]].new_value,PICKER_decimals);i++){
					empty=empty+" ";
				}
				lcD->setCursor(n_cols-strlen(PICKER_unit)-1-num_disp_length(pick[pick_pos[cursor_pos]].new_value,PICKER_decimals),header_lines+cursor_pos);
				lcD->print(empty);
			}

		}
		
		//button routines//
		if (up_type==1){
			if(setup_mode==0){
				if(cursor_pos>0){	
// Serial.println("up");		
					cursor_pos--;
				}
				else if (cursor_pos==0){
// Serial.println("menu down");		
					move_down(pick_pos, n_pickers);
				}
			}
			else{ 
				// Serial.println(pick[pick_pos[cursor_pos]].new_value);
				// Serial.println(PICKER_inc_short);
				// Serial.println(pick[pick_pos[cursor_pos]].new_value+PICKER_inc_short);

				pick[pick_pos[cursor_pos]].new_value=pick[pick_pos[cursor_pos]].new_value+PICKER_inc_short;
				if (pick[pick_pos[cursor_pos]].new_value>PICKER_max_value){
					pick[pick_pos[cursor_pos]].new_value=PICKER_max_value;
				}

			}
			// Serial.print("cursor_pos:");
			// Serial.println(cursor_pos);
			// Serial.println(pick_pos[cursor_pos]);
			// Serial.println(pick[pick_pos[cursor_pos]].ref);
			// print_menu();
			Panel->event=1;
		}
		if (up_type==2){
			if(setup_mode==1){
				pick[pick_pos[cursor_pos]].new_value=pick[pick_pos[cursor_pos]].new_value+PICKER_inc_long;
				if (pick[pick_pos[cursor_pos]].new_value>PICKER_max_value){
					pick[pick_pos[cursor_pos]].new_value=PICKER_max_value;
				}

			}
			// print_menu();
			Panel->event=1;
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
				pick[pick_pos[cursor_pos]].new_value=pick[pick_pos[cursor_pos]].new_value-PICKER_inc_short;
				if (pick[pick_pos[cursor_pos]].new_value<PICKER_min_value){
					pick[pick_pos[cursor_pos]].new_value=PICKER_min_value;
				}

			}
			// Serial.print("cursor_pos:");
			// Serial.println(cursor_pos);
			// Serial.println(pick_pos[cursor_pos]);
			// Serial.println(pick[pick_pos[cursor_pos]].ref);
			// print_menu();
			Panel->event=1;
		}	
		if (down_type==2){
			if(setup_mode==1){
				pick[pick_pos[cursor_pos]].new_value=pick[pick_pos[cursor_pos]].new_value-PICKER_inc_long;
				if (pick[pick_pos[cursor_pos]].new_value<PICKER_min_value){
					pick[pick_pos[cursor_pos]].new_value=PICKER_min_value;
				}

			}
			// print_menu();
			Panel->event=1;
		}		
		switch (navigation_mode) {
			case 4:
				if (set_type==1){
					switch(PICKER_mode){
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
								event=1;
								break;		
					}
					// Panel->print_menu();				
				}
				if (set_type==2){
					switch(PICKER_mode){
						case 1:
								event=1;
								break;
						case 2:
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
									//pick[pick_pos[cursor_pos]].new_value=0;
									if (pick[pick_pos[cursor_pos]].EEPROM_ACTIVE==1){
										//EEPROM.put(pick[pick_pos[cursor_pos]].EEPROM_ADDR, pick[pick_pos[cursor_pos]].value);
									}
									pick[pick_pos[cursor_pos]].enabled=1;
									event=1;
								}
								break;		
					}			
				// Panel->print_menu();				
			}
				if (back_type==1){
					switch(PICKER_mode){
						case 1:
								if (pick[pick_pos[cursor_pos]].parent!=0){
									active=0;
									reset_value();
									setup_mode=0;
									Panel=pick[pick_pos[cursor_pos]].parent;
									Panel->active=1;
									event=1;
								}
								break;
						case 2:
								if (pick[pick_pos[cursor_pos]].parent!=0){
									active=0;
									reset_value();
									setup_mode=0;
									Panel=pick[pick_pos[cursor_pos]].parent;
									Panel->active=1;
									event=1;
								}
								break;
						case 3:
								if (pick[pick_pos[cursor_pos]].parent!=0){
									active=0;
									reset_value();
									setup_mode=0;
									Panel=pick[pick_pos[cursor_pos]].parent;
									Panel->active=1;
									event=1;
								}
								break;		
					}			
					// Panel->print_menu();				
				}
				if (back_type==2){
					switch(PICKER_mode){
						case 1:	
								event=1;				
								break;
						case 2:
								event=1;
								break;
						case 3:
								event=1;
								break;		
					}			
					// Panel->print_menu();				
				}
			break;
			case 5:
				if (set_type==1){
					switch(PICKER_mode){
						case 1:
								event=1;
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
										EEPROM.put(PICKER_EEPROM_ADDR, pick[pick_pos[cursor_pos]].value);
									}
									//pick[pick_pos[cursor_pos]].new_value=0;
									pick[pick_pos[cursor_pos]].enabled=1;
									event=1;
								}
								break;			
					}
					// Panel->print_menu();				
				}
				if (set_type==2){
					switch(PICKER_mode){
						case 1:
								event=1;
								break;
						case 2:
								event=1;
								break;
						case 3:
								reset_value();
								event=1;
								setup_mode=0;
								break;		
				}			
				// Panel->print_menu();				
			}
				if (next_type==1){
					switch(PICKER_mode){
						case 1:
								if (pick[pick_pos[cursor_pos]].child!=0){
									active=0;
									//reset_value();
									Panel=pick[pick_pos[cursor_pos]].child;
									Panel->event=1;
									Panel->active=1;
								}
								break;
						case 2:
								event=1;
								break;
						case 3:
								event=1;
								break;		
					}
					// Panel->print_menu();				
				}
				if (next_type==2){
					switch(PICKER_mode){
						case 1:
								if (pick[pick_pos[cursor_pos]].child!=0){
									active=0;
									//reset_value();
									Panel=pick[pick_pos[cursor_pos]].child;
									Panel->event=1;
									Panel->active=1;
								}
								break;
						case 2:
								event=1;
								break;
						case 3:
								event=1;
								break;		
					}			
					// Panel->print_menu();				
			}
				if (back_type==1){
					switch(PICKER_mode){
						case 1:
								if (pick[pick_pos[cursor_pos]].parent!=0){
									active=0;
									//reset_value();
									setup_mode=0;
									Panel=pick[pick_pos[cursor_pos]].parent;
									Panel->active=1;
									Panel->event=1;
								}
								break;
						case 2:
								if (pick[pick_pos[cursor_pos]].parent!=0){
									active=0;
									//reset_value();
									setup_mode=0;
									Panel=pick[pick_pos[cursor_pos]].parent;
									Panel->active=1;
									Panel->event=1;
								}
								break;
						case 3:
								if (pick[pick_pos[cursor_pos]].parent!=0){
									active=0;
									//reset_value();
									setup_mode=0;
									Panel=pick[pick_pos[cursor_pos]].parent;
									Panel->active=1;
									Panel->event=1;
								}
								break;		
					}			
					// Panel->print_menu();				
				}
				if (back_type==2){
					switch(PICKER_mode){
						case 1:
								if (pick[pick_pos[cursor_pos]].parent!=0){
									active=0;
									//reset_value();
									setup_mode=0;
									Panel=pick[pick_pos[cursor_pos]].parent;
									Panel->active=1;
									Panel->event=1;
								}
								break;
						case 2:
								if (pick[pick_pos[cursor_pos]].parent!=0){
									active=0;
									//reset_value();
									setup_mode=0;
									Panel=pick[pick_pos[cursor_pos]].parent;
									Panel->active=1;
									Panel->event=1;
								}
								break;
						case 3:
								if (pick[pick_pos[cursor_pos]].parent!=0){
									active=0;
									//reset_value();
									setup_mode=0;
									Panel=pick[pick_pos[cursor_pos]].parent;
									Panel->active=1;
									Panel->event=1;
								}
								break;				
					}			
					// Panel->print_menu();				
				}
			break;
		};
	// Serial.println(event);
};
void Menu::disable_all(void){
	for (uint8_t i=0;i<n_pickers;i++){
		pick[i].enabled=0;
	}
}
void Menu::reset_value(void){
	for (uint8_t i=0;i<n_pickers;i++){
		pick[i].value=0;
		pick[i].new_value=0;
	}
	
}


Screen_saver::Screen_saver(LiquidCrystal_I2C *LCD){
		t_refresh=500;
		t_last=0;
		lcD=LCD;
}
void Screen_saver::print(uint8_t power_mode,long rpm_t,long rpm_a,long Vin,long Cin,long Tmft,long Cm,long Tm,bool light_state,bool cool_state){
// SrcSvr.print(power_mode,M110.pick[0].value,Vesc_UART.data.rpm,Vesc_UART.data.inpVoltage,Vesc_UART.data.avgInputCurrent,Vesc_UART.data.tempFET,100.0,Vesc_UART.data.avgMotorCurrent,Vesc_UART.data.tempMotor,M1.pick[1].state,M1.pick[2].state);
						
	if((millis()-t_last)>=t_refresh){
		t_last=millis();

		if (light_state==1){
			lcD->setCursor(lcD->_cols-1,0);
			lcD->write(1);
		}
		else{
			lcD->setCursor(lcD->_cols-1,0);
			lcD->print(F(" "));
		}
		if (cool_state==1){
			lcD->setCursor(lcD->_cols-2,0);
			lcD->write(2);
		}
		else{
			lcD->setCursor(lcD->_cols-2,0);
			lcD->print(F(" "));
		}
		switch (power_mode){
		 case 2:
		 	lcD->setCursor(0,0);
		    lcD->print(F("MANUAL           "));//print  type of control
			lcD->setCursor(0,1);
			sprintf(SS_buff,"T:%4lirpm  A:%4lirpm",rpm_t,rpm_a);//tst);
						 //Serial.println(SS_buff);
			lcD->print(SS_buff);
			lcD->setCursor(0,2);
			// sprintf(SS_buff,"Vi:%2iV Ci:%2iA Tf:%2iC",(long)Vin,(long)Cin,(long)Tmft);
			sprintf(SS_buff,"I:%02i.%1iV/%02i.%1iA  %2i.%0iC",(int)Vin/10,(int)Vin%10,(int)Cin/100,(int)(Cin%100)/10,(int)Tmft/10,(int)Tmft%10);
			lcD->print(SS_buff);
			lcD->setCursor(0,3);
			sprintf(SS_buff,"M:%02i.%1iV/%02i.%1iA  %2i.%0iC",(int)Vin/10,(int)Vin%10,(int)Cm/100,(int)(Cm%100)/10,(int)abs(Tm/10),(int)abs(Tm%10));
						 //Serial.println(SS_buff);
			lcD->print(SS_buff);
			break;
		case 4:
			lcD->setCursor(0,0);
		    lcD->print(F("EXT.CTRL          "));//print  type of control
			lcD->setCursor(0,1);
			sprintf(SS_buff,"T:%4lirpm  A:%4lirpm",rpm_t,rpm_a);//tst);
						 //Serial.println(SS_buff);
			lcD->print(SS_buff);
			lcD->setCursor(0,2);
			sprintf(SS_buff,"Vi:%2liV Ci:%2liA Tf:%2liC",Vin,Cin,Tmft);
						 //Serial.println(SS_buff);
			lcD->print(SS_buff);
			lcD->setCursor(0,3);
			sprintf(SS_buff,"Cm:%2liA Tm:%2liC",Cm,Tm);
						 //Serial.println(SS_buff);
			lcD->print(SS_buff);
			break;

		}
	}
}


void init_array(int arr[],int size,int res){
    //int n = sizeof(arr) / sizeof(int);
    for(uint8_t i=0;i<size;i++){
        arr[i]=res;
    }
};
void init_array(uint8_t arr[],uint8_t size,uint8_t res){
    //int n = sizeof(arr) / sizeof(int);
    for(uint8_t i=0;i<size;i++){
        arr[i]=res;
    }
};
void move_up(uint8_t arr[], uint8_t size){
    int buff=arr[0];
    for (uint8_t i=0;i<size-1;i++){
        arr[i]=arr[i+1] ;
    }
    arr[size-1]=buff;
}
void move_down(uint8_t arr[], uint8_t size){
    uint8_t buff=arr[size-1];
    for (uint8_t i=size-1;i>0;i--){
        arr[i]=arr[i-1] ;
    }
    arr[0]=buff;
}
int n_digit(float num){
	//Serial.println("n_digit:");
	uint8_t n=0;
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
float  round_n (float val,uint8_t dig){
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
int  num_disp_length (float val,uint8_t dig){
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
float range_loop(float x, float _max_x,float _min_x,uint8_t decimals){
	  if(x>=_max_x){
		  return round_n(_min_x+(x-_max_x)-(_max_x-_min_x)*trunc((x-_max_x)/(_max_x-_min_x)),decimals);      
	  }
	  else if( x<_min_x){
	  return round_n(_max_x-(_min_x-x)-(_max_x-_min_x)*trunc((x-_min_x)/(_max_x-_min_x)),decimals);
	  }
	  else { return round_n(x,decimals);}
}


Menu *Panel;