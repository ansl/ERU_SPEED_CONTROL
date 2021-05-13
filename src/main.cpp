/*
 * ERU SPEED CONTROL V4
 *
 * Created: 01/01/2021 10:37:53 PM
 * Author: ANSL
 */ 
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
// Arduino GPIO Pinout 

//0:&RX
//1:&TX
//2:Encoder magnetico input/interrupt
//3:GRBL PWM INPUT
//4:RIGHT
//5:DOWN
//6:SET
//7:UP
//8:LEFT
//9:
//10:PWM ESC motor control
//11:HOLD
//12:FEED
//13: board led
//(A0)14:LIGTH
//(A1)15:COOLANT
//(A2)16:AUX1
//(A3)17:AUX2
//(A4)18:SDA DISPLAY
//(A5)19:SCL DISPLAY
//(A6)20:
//(A7)21:TEMP_SENS
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//LIBRARIES

#include <Arduino.h>
#include <LiquidCrystal_I2C.h>
#include <Wire.h>
#include <PWM_READ.h>
#include <LCD_MENU.h>
//#include <PID.h>
#include <EEPROM.h>
#include <VescUart.h>

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//DEFINES

#define LCD_ACTIVE

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//contactors
#define NEXT 4
#define DOWN 5
#define SET 6
#define UP 7
#define BACK 8
#define HOLD 11
#define FEED 12
#define LIGHT 14
//#define T_SENS A4
#define COOLANT 15
#define T_SENS A7 //OUTPUT AUX " repurposed to be feed instaad from A3 form A7"

//LCD #############################################################
#ifdef LCD_ACTIVE
	#define max_char 20
	#define max_row 4
	LiquidCrystal_I2C lcd(0x27, max_char, max_row);
	uint8_t next[8] = {0x0, 0x0, 0x4, 0x2, 0x1f, 0x2, 0x4, 0x0};
	uint8_t lght[8] = {0b00100, 0b10101, 0b01110, 0b11111, 0b01110, 0b10101, 0b00100, 0b00000};
	uint8_t cool[8] = {B01110, B01110, B11111, B11111, B00100, B01110, B10101, B00100};
#endif // !LCD_ACTIVE

// PID #############################################################
// int maxPWM=4000;
// int minPWM=1900;
// PID PID1(0.01, 20 ,0, 1800, minPWM, maxPWM, 50);

// VESC_UART #############################################################
VescUart Vesc_UART;
// PWM input pin 3 #################################################
PWM_READ ERU_PWM(10);

// //MENU#############################################################
#ifdef LCD_ACTIVE
	Menu M1(4,1,&lcd,UP,DOWN,SET,BACK,NEXT);
		Menu M11(2,1,&lcd,UP,DOWN,SET,BACK,NEXT);
			Menu M110(1,1,&lcd,UP,DOWN,SET,BACK,NEXT);
			Menu M111(1,1,&lcd,UP,DOWN,SET,BACK,NEXT);
		Menu M12(1,1,&lcd,UP,DOWN,SET,BACK,NEXT);
	

//SCREEN SAVER#############################################################
	Screen_saver SrcSvr(&lcd);
#endif

//Global Variables#############################################################
volatile int n=0;
volatile int n0=0;
volatile int nT=0;
volatile int nP=0;
volatile int p=7;

volatile long v=0;
volatile long v_target=0;
volatile long pwm_target=0;

float T=1024 / 16000000.0;//0.000064;	//6.4e-5s
float buff=255;
float Tc=T*(buff+1);

uint8_t power_mode=2; //0: no power mode 3:PID 5:EXT PWM 7:%Power

float temp_spindle=0;

void setup() {
  // put your setup code here, to run once:
	Serial.begin(115200);
	Vesc_UART.setSerialPort(&Serial);

  //////////////////////////////////////////////////////////////////////////////////////
	//SET GPIO
	pinMode(13, OUTPUT);		//pin de los leds
	digitalWrite(13,HIGH);		//pin de los leds
	
	//PWM ESC
	pinMode(10,OUTPUT);// salida pwm

	//ENCODER magnetico
	pinMode(2, INPUT);			//Encoder magn?tico
	digitalWrite(2, HIGH);
	
	//GRBL PWM INPUT
	pinMode(3,INPUT);// salida pwm
	digitalWrite(3, HIGH);
	//CONTACTORS
	pinMode(UP, INPUT_PULLUP);
	pinMode(DOWN, INPUT_PULLUP);
	pinMode(NEXT, INPUT_PULLUP);
	pinMode(BACK, INPUT_PULLUP);
	pinMode(SET, INPUT_PULLUP);
	pinMode(HOLD, INPUT_PULLUP);
	pinMode(FEED, INPUT_PULLUP);
	
	//SERVICES
	pinMode(LIGHT, OUTPUT);
	pinMode(COOLANT, OUTPUT);
	pinMode(T_SENS, INPUT);
	
	//////////////////////////////////////////////////////////////////////////////////////
	//INTERRUPTS
	cli();
	//inicializo timer1/pwm
	// TCCR1A=0;					//Inicializo los control registers del timer1 tanto el A como el B
	// TCCR1B=0;
	// TCCR1C=0;
	// TIMSK1=0;
	// TCCR1A |=(1 << COM1B1);		//defino la salida en el OCR1B en toogle mode #pin10
	// TCCR1A |=(1 << WGM10);		//WGMxx Lo define como fast pwm con el TOP en OCR1A /la comparaci?n en el OC1B
	// TCCR1A |=(1 << WGM11);
	// TCCR1B |=(1 << WGM12);
	// TCCR1B |=(1 << WGM13);
	// TCCR1B |=(1 << CS11);		//Seteo el prescaling en 8
	// TIMSK1 |=(1 << OCIE1B);
	// //Inicializo el TOP/y el duty
	// OCR1A=40000;				//40000=20ms
	// OCR1B=1900;					//rango de 2000 a 4000 =>2000=1ms 4000=2ms

	//////////////////////////////////////////////////////////////////////////////////////
	//inicializo timer2/overflow

	TCCR2A=0;					//Inicializo los control registers del timer2
	TCCR2B=0;
	TIMSK2=0;

	TCCR2B |=(1 << CS20);		//Seteo el prescaling en 1024
	TCCR2B |=(1 << CS21);		//Seteo el prescaling en 1024
	TCCR2B |=(1 << CS22);		//Seteo el prescaling en 1024
	TIMSK2 |=(1 << TOIE2);		//Seteo como se dispara el interrupt(overflow)
	// //////////////////////////////////////////////////////////////////////////////////////
	// //ENABLE EXTERNAL INTERRUPT

	//MAGNETIC ENCODER
	EIMSK |= (1 << INT0);		// Enable external interrupt INT0 #pin2
	EICRA |= (1 << ISC01);		// Trigger INT0 on falling edge

	// //GRBL PWM INPUT
	// //PWM_READ GRBL_PWM(10000);

	sei();						// Enable global interrupts
	
	#ifdef LCD_ACTIVE
		//LCD MENU definitions    
			lcd.init();  //initialize the lcd
			// Serial.println(F("init"));
			lcd.clear();
			lcd.backlight();  //open the backlight

			lcd.setCursor(0,0);
			lcd.print(F("ERU Project 5.0"));
			lcd.setCursor(0,1);
			lcd.print(F("AntS 2021"));
			lcd.createChar(0,next);
			lcd.createChar(1,lght);
			lcd.createChar(2,cool);
			delay(500);

			lcd.clear();
			v=0;
			pwm_target=1800;
			delay(1000);
			//Serial.println(F("INITIALIZING....................."));
		//Define Menu Pannel refferences
		M1.menu_ref=0;
		M11.menu_ref=1;
		M110.menu_ref=2;
		M111.menu_ref=3;
		M12.menu_ref=4;
		//ERU_MAIN
			// SPINDLE
			M1.pick[0].ref=0;
				// SPINDLE CONTROL
				M11.pick[0].ref=1;
					M110.pick[0].ref=2;
					M110.pick[0].value=0;
				M11.pick[1].ref=3;
					M111.pick[0].ref=4;
					M111.pick[0].value=0;
			// LIGHT
			M1.pick[1].ref=5;
			// COOLANT
			M1.pick[2].ref=6;
			// CONFIG
			M1.pick[3].ref=7;
				// TMAX
				M12.pick[0].ref=8;
				M12.pick[0].EEPROM_ACTIVE=1;
				M12.pick[0].get_val_EEPROM();


		//Define menu interconnections
			M1.pick[0].child=&M11;
				M11.pick[0].child=&M110;
				M11.pick[1].child=&M111;
			M1.pick[3].child=&M12;

			M11.pick[0].parent=&M1;
				M110.pick[0].parent=&M11;
			M11.pick[1].parent=&M1;
				M111.pick[0].parent=&M11;
			
			M12.pick[0].parent=&M1;


			Panel=&M1; //inicializo en panel principal
			Panel->print_menu();

			

// Serial.println(pwm_target);

	#endif
    //Inicializo el serial print
	// Serial.println(F("ERU Project 4.0 Spindle control start..."));

	#ifndef LCD_ACTIVE
		pwm_target=1800;
		v_target=2500;
		power_mode=1;
    	//delay(10000);
		// Serial.print("start with target speed ");
		// Serial.print(v_target);
		// Serial.println("rmp");
	#endif

}

void loop() {
	
    #ifdef LCD_ACTIVE
		Panel->check_button();
		temp_spindle = (analogRead(T_SENS)*5.0/1024 - 0.5) * 100;
		// Serial.println(temp_spindle);
		digitalWrite(LIGHT, M1.pick[1].state);
		digitalWrite(COOLANT, M1.pick[2].state);
		switch (power_mode){
			case 2:
				if (Panel->menu_flag==1 && Panel->event==1){//PRINT MENU 
						Panel->print_menu();
						uint8_t _ref=Panel->pick[Panel->pick_pos[Panel->cursor_pos]].ref; //check the picker ref to change the power_mode
						if (_ref==2 || _ref==4){
							power_mode=_ref;
						}
				}else if (Panel->menu_flag==0)//PRINT SCREEN SAVER
				{
						//uint8_t power_mode,float rpm_t,float rpm_a,float Vin,float Cin,float Tmft,float Pow,float Cm,float Tm,bool light_state,bool cool_state
						Vesc_UART.getVescValues();
						delay(100);
						SrcSvr.print(power_mode,M110.pick[0].value,Vesc_UART.data.rpm,Vesc_UART.data.inpVoltage,Vesc_UART.data.avgInputCurrent,Vesc_UART.data.tempFET,Vesc_UART.data.avgMotorCurrent,Vesc_UART.data.tempMotor,M1.pick[1].state,M1.pick[2].state);
				}		
				Vesc_UART.setRPM(M110.pick[0].value);
				break;
			case 4:
				if (Panel->menu_flag==1 && Panel->event==1){//PRINT MENU 
						Panel->print_menu();
						uint8_t _ref=Panel->pick[Panel->pick_pos[Panel->cursor_pos]].ref; //check the picker ref to change the power_mode
						if (_ref==2 || _ref==4){
							power_mode=_ref;
						}
				}else if (Panel->menu_flag==0)//PRINT SCREEN SAVER
				{
						//SrcSvr.print(power_mode,temp_spindle,M12.pick[3].value,v,M111.pick[0].value,M1.pick[0].state,M1.pick[3].state);
				}
				//Vesc_UART.setRPM(XXXX);

			break;
			}

	#endif
	#ifndef LCD_ACTIVE
		pwm_target=PID1.Evaluate(v,v_target);
		Serial.print("T:");
		Serial.print(v_target);
		Serial.print("rpm V:");
		Serial.print(v);
		Serial.print("rpm PWM:");
		Serial.print(pwm_target);
		Serial.println("");

		//pwm_target=int(2200*v_target/100)+1900; // % of throtle from 1900 -> 4100 || 1->2ms
	#endif

}



//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////	
//ISR
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


ISR(INT0_vect) //interrupt que detecta cada pulso del encoder magnetico
	{
		uint8_t oldSREG = SREG;
		
		digitalWrite(13,!digitalRead(13));	// Toggle LED on pin 13
		n=n+1;
		
		SREG = oldSREG;
	}


ISR(TIMER2_OVF_vect) //TImer por overflow usado para calcular en cada periodo de tiempo definido por el OVF el numero de pulsos dado por el encoder magnetico
	{
		if (nT<=10){
			nT++;
			nP=nP+n;
			n=0;
		}
		else{
			v=60*nP/(nT*Tc*p);
			nT=0;
			nP=0;
			n=0;
		}
	};

// ISR(TIMER1_COMPB_vect)//Timer usado para definir el duti cycle del PWM
// 	{
// 		OCR1B=pwm_target;
// 	};