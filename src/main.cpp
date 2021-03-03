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
//9:LIGTH
//10:PWM ESC motor control
//11:HOLD
//12:FEED
//13: board led
//(A0)14:TEMP SENSOR
//(A1)15:COOLANT
//(A2)16:
//(A3)17:
//(A4)18:SDA DISPLAY
//(A5)19:SCL DISPLAY
//(A6)20:
//(A7)21:
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//LIBRARIES

#include <Arduino.h>
#include <LiquidCrystal_I2C.h>
#include <Wire.h>
#include <PWM_READ.h>
#include <LCD_MENU.h>
#include <PID.h>


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
#define LIGHT 9
#define T_SENS 14
#define COOLANT 15

//LCD #############################################################
#ifdef LCD_ACTIVE
	#define max_char 20
	#define max_row 4
	LiquidCrystal_I2C lcd(0x27, max_char, max_row);
	uint8_t next[8] = {0x0, 0x0, 0x4, 0x2, 0x1f, 0x2, 0x4, 0x0};
	uint8_t lght[8] = {0b00100, 0b10101, 0b01110, 0b11111, 0b01110, 0b10101, 0b00100, 0b00000};
#endif // !LCD_ACTIVE

// PID #############################################################
int maxPWM=4000;
int minPWM=1900;
PID PID1(0.002, 2 ,0, 1800, minPWM, maxPWM, 100);

// //MENU#############################################################
#ifdef LCD_ACTIVE
	Menu M1(3,1,&lcd,UP,DOWN,SET,BACK,NEXT);
	Menu M21(3,1,&lcd,UP,DOWN,SET,BACK,NEXT);
	// Menu M22(4,1,&lcd,UP,DOWN,SET,BACK,NEXT);
	Menu M211(1,1,&lcd,UP,DOWN,SET,BACK,NEXT);
	// Menu M212(1,1,&lcd,UP,DOWN,SET,BACK,NEXT);
	// Menu M213(1,1,&lcd,UP,DOWN,SET,BACK,NEXT);
	
#endif
//Variables#############################################################

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

int power_mode=0; //0: no power mode 1:PID 2:EXT PWM 3:%Power

void setup() {
  // put your setup code here, to run once:
    Serial.begin(9600);
	
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

	TCCR1A=0;					//Inicializo los control registers del timer1 tanto el A como el B
	TCCR1B=0;
	TCCR1C=0;
	TIMSK1=0;

	TCCR1A |=(1 << COM1B1);		//defino la salida en el OCR1B en toogle mode #pin10

	TCCR1A |=(1 << WGM10);		//WGMxx Lo define como fast pwm con el TOP en OCR1A /la comparaci?n en el OC1B
	TCCR1A |=(1 << WGM11);
	TCCR1B |=(1 << WGM12);
	TCCR1B |=(1 << WGM13);

	TCCR1B |=(1 << CS11);		//Seteo el prescaling en 8

	TIMSK1 |=(1 << OCIE1B);

	//Inicializo el TOP/y el duty

	OCR1A=40000;				//40000=20ms
	OCR1B=1900;					//rango de 2000 a 4000 =>2000=1ms 4000=2ms

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
			Serial.println("init");
			lcd.clear();
			lcd.backlight();  //open the backlight
			lcd.createChar(0, next);
			lcd.setCursor(0,0);
			lcd.print("ERU Project 4.0");
			lcd.setCursor(0,1);
			lcd.print("AntS 2021");
			lcd.createChar(0,next);
			delay(2000);

			lcd.clear();

		//Define Menu Pannels
		    M1.header="ERU";
			M1.pick[0].picker_name="LIGHT";
			M1.pick[0].mode=2;
			M1.pick[1].picker_name="SPINDLE";
			M1.pick[1].mode=1;
			M1.pick[2].picker_name="CONFIG";
			M1.pick[2].mode=1;
			//M1.print_menu();
			
			M21.header="SPINDLE CONTROL";
			M21.pick[0].picker_name="PID:";
			M21.pick[0].mode=1;
			M21.pick[1].picker_name="PWM:";
			M21.pick[1].mode=1;
			M21.pick[2].picker_name="PWR%:";
			M21.pick[2].mode=1;

			M211.header="SPINDLE PID";
			M211.pick[0].picker_name="Target:";
			M211.pick[0].mode=3;
			M211.pick[0].unit="rpm";
			M211.pick[0].decimals=0;
			M211.pick[0].inc_short=50;
			M211.pick[0].inc_long=500;
			M211.pick[0].max_value=50000;
			M211.pick[0].min_value=0;
			M211.pick[0].ref=1;

			// M212.header="EXT PWM";
			// M212.pick[0].picker_name="PWM LINK:";
			// M212.pick[0].mode=2;
			// M212.pick[0].state_string0="DISCONECTED";
			// M212.pick[0].state_string1="CONNECTED";
			// M212.pick[0].ref=2;

			// M213.header="SPINDLE PWR %:";
			// M213.pick[0].mode=3;
			// M213.pick[0].unit="%";
			// M213.pick[0].decimals=0;
			// M213.pick[0].inc_short=1;
			// M213.pick[0].inc_long=10;
			// M213.pick[0].max_value=100;
			// M213.pick[0].min_value=0;
			// M213.pick[0].ref=3;
			
			// M22.header="CONFIG";
			// M22.pick[0].picker_name="P";
			// M22.pick[0].mode=3;
			// M22.pick[0].unit="";
			// M22.pick[0].decimals=0;
			// M22.pick[0].inc_long=10;
			// M22.pick[0].inc_short=1;
			// M22.pick[0].min_value=0;
			// M22.pick[0].EEPROM_ACTIVE=1;
			// M22.pick[0].EEPROM_ADDR=0;
			// EEPROM.get(M22.pick[0].EEPROM_ADDR,M22.pick[0].value);

			// M22.pick[1].picker_name="D";
			// M22.pick[1].mode=3;
			// M22.pick[1].unit="";
			// M22.pick[1].decimals=0;
			// M22.pick[1].inc_long=10;
			// M22.pick[1].inc_short=1;
			// M22.pick[1].min_value=0;
			// M22.pick[1].EEPROM_ACTIVE=1;
			// M22.pick[1].EEPROM_ADDR=4;
			// EEPROM.get(M22.pick[1].EEPROM_ADDR,M22.pick[1].value);

			// M22.pick[2].picker_name="I";
			// M22.pick[2].mode=3;
			// M22.pick[2].unit="";
			// M22.pick[2].decimals=0;
			// M22.pick[2].inc_long=10;
			// M22.pick[2].inc_short=1;
			// M22.pick[2].min_value=0;
			// M22.pick[2].EEPROM_ACTIVE=1;
			// M22.pick[2].EEPROM_ADDR=8;
			// EEPROM.get(M22.pick[2].EEPROM_ADDR,M22.pick[2].value);

			// M22.pick[3].picker_name="Temp max";
			// M22.pick[3].value=70.0;
			// M22.pick[3].mode=3;
			// M22.pick[3].unit="ºC";
			// M22.pick[3].decimals=1;
			// M22.pick[3].inc_long=10;
			// M22.pick[3].inc_short=1;
			// M22.pick[3].max_value=100;
			// M22.pick[3].min_value=0;
			// M22.pick[3].EEPROM_ACTIVE=1;
			// M22.pick[3].EEPROM_ADDR=12;
			// EEPROM.get(M22.pick[3].EEPROM_ADDR,M22.pick[3].value);

			//Define menu interconnections
			M1.pick[1].child=&M21;
			// M1.pick[2].child=&M22;
			M21.pick[0].child=&M211;
			// M21.pick[1].child=&M212;
			// M21.pick[2].child=&M213;

			M21.pick[0].parent=&M1;
			M21.pick[1].parent=&M1;
			M21.pick[2].parent=&M1;
			// M22.pick[0].parent=&M1;
			// M22.pick[1].parent=&M1;
			// M22.pick[2].parent=&M1;
			// M22.pick[3].parent=&M1;

			M211.pick[0].parent=&M21;
			// M212.pick[0].parent=&M21;
			// M213.pick[0].parent=&M21;
			
			Panel=&M1; //inicializo en panel principal
			Panel->print_menu();
	#endif
    //Inicializo el serial print
	Serial.println("ERU Project 4.0 Spindle control start...");
    delay(1000);

	#ifndef LCD_ACTIVE
		v_target=2500;
		power_mode=1;

		// Serial.print("start with target speed ");
		// Serial.print(v_target);
		// Serial.println("rmp");
	#endif

}

void loop() {
    #ifdef LCD_ACTIVE
   		Panel->check_button();
		// if (Panel->event==1){
		// 	if(Panel->pick[0].ref>0 && Panel->pick[0].enabled==1){
		// 		power_mode=Panel->pick[0].ref;
		// 	}
		// 	PID1.Param(M22.pick[0].value,M22.pick[1].value,M22.pick[2].value);
		// }

		// switch (power_mode){
		// 	case 1:
		// 		pwm_target=PID1.Evaluate(v,M211.pick[0].value);
		// 		break;
		// 	case 2:
		// 		break;
		// 	case 3:
		// 		pwm_target=int(2200*M213.pick[0].value/100)+1900; // % of throtle from 1900 -> 4100 || 1->2ms
		// 		break;
		// 	}
	#endif
	//#ifndef LCD_ACTIVE
	//pwm_target=PID1.Evaluate(v,v_target);
	// Serial.print("T:");
	// Serial.print(v_target);
	// Serial.print("rpm V:");
	// Serial.print(v);
	// Serial.print("rpm PWM:");
	// Serial.print(pwm_target);
	// Serial.println("");

	//pwm_target=int(2200*M21.pick[0].value/100)+1900; // % of throtle from 1900 -> 4100 || 1->2ms
	//#endif

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

ISR(TIMER1_COMPB_vect)//Timer usado para definir el duti cycle del PWM
	{
		OCR1B=pwm_target;
	};

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