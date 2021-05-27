
#include <Arduino.h>
#include <pins_arduino.h>
#include <PWM_READ.h>

#define PWM_IN_FREQ 32*65536UL/F_CPU  /1960//1.96 kHz
//extern volatile unsigned long timer0_overflow_count;




        volatile uint32_t      peak_width         =      0;
        volatile uint16_t      tmr_overf          =      0;

// ISR(INT1_vect){
//     PWM_READ::PWM_ISR();
// }

// ISR(TIMER1_OVF_vect) {
//    tmr_overf++;  
//  }

PWM_READ::PWM_READ(){
    //using Timer 1 
         //inicializo timer1/normal mode
             TCCR1A=0;					//Inicializo los control registers del timer1 tanto el A como el B
             TCCR1B=0;
             TCCR1C=0;
             TIMSK1=0;
            // TCCR1A |=(1 << COM1B1);		//defino la salida en el OCR1B en toogle mode #pin10
            // TCCR1A |=(1 << WGM10);		//WGMxx Lo define como fast pwm con el TOP en OCR1A /la comparaci?n en el OC1B
            // TCCR1A |=(1 << WGM11);
             TCCR1B |=(1 << ICNC1);     //input noise canceler-> It causes a delay of 4 clock cycles as it considers a change only if it persists for at least 4 successive system clocks.
             TIMSK1 |=(1 << ICIE1);     //Input Capture Interrupt Enable
             TCCR1B |=(1 << ICES1);     //Select edge detection for input capture function.0 = Capture on the falling edge1 = Capture on rising edge
             TCCR1B |=(1 << CS10);	  	//Seteo el prescaling en no prescaling
             //TCCR1B |=(1 << CS11);		//Seteo el prescaling en no prescaling

             //TIMSK1 |= (1<<TOIE1); // Overflow Interrupt Enable 
             TIFR1 |=(1<<ICF1);

}
uint32_t PWM_READ::duty(){
        // return 10*lroundf((_range/10)*abs(CYCLE_END-DUTY_START)/CYCLE_END);
           return peak_width;

}
float PWM_READ::freq(){
        // return 10*lroundf((_range/10)*abs(CYCLE_END-DUTY_START)/CYCLE_END);
        return PWM_IN_FREQ;
}
// unsigned long PWM_READ::test(){
//         return CYCLE_END;

// }
void PWM_READ::PWM_ISR(){
    uint32_t curr_v = ICR1;
    static uint32_t last_v;

    curr_v  = curr_v + tmr_overf * 65536UL; 

    if(((TCCR1B>>ICES1)&1)==0 && curr_v>last_v){
      peak_width=curr_v-last_v;
      tmr_overf=0;
    }
    TCCR1B ^= (1<<ICES1);  // invert flange detection

    last_v=curr_v;
    
}

ISR(TIMER1_CAPT_vect){
    PWM_READ::PWM_ISR();
}
// void PWM_READ::PWM_ISR(){ //INT1 input
                
//                 //  uint8_t oldSREG = SREG;

//                 if (((EICRA>>2) & 0x01)==0){
//                     t_abs_old=t_abs;
//                     //DETECTA CAIDA lee el final del ciclo, resetea el timer y cambia la deteccion a rising edge/ calcula los periodos
//                     t_abs= micros();//(timer0_overflow_count*256+TCNT0); //timer0_overflow_count contabiliza cuantas veces se llega la overflow *256(tamaño del timer)
//                     CYCLE_END=t_abs-t_abs_old;
//                     EICRA |= (1 << ISC10);//cambia la deteccion de flanco a rising edge
//                 }
//                 else if (((EICRA>>2) & 0x01)==1){ 
 
//                     //DETECTA SUBIA lee el duty y cambia la deteccion a falling edge
//                     DUTY_START=micros()-t_abs;//(timer0_overflow_count*256+TCNT0)-t_abs;//lee dode se produce la
//                     EICRA ^= (1 << ISC10); //cambia la deteccion de flanco a falling edge

//                 }
//                 //  SREG = oldSREG;
            
// }






/*
  Using Input Capture to measure frequency & duty cycle 
  on Arduino Mega2560.

  Reads a PWM signal on pin 49, prints the result over Serial Monitor.
  Commands to activate printout - "d".
  Range 1.00 Hz <---> 86.00 kHz
  
  Released into the public domain.

  Created by Anatoly Kuzmenko, April 2018
  k_anatoly@hotmail.com
*/

//            String       in_String         =     "";        
//            boolean      end_input         =  false;  
//            uint8_t      adres_reg         =      0;         
//            uint8_t      debug_osm         =      0;


//   volatile int32_t      period_hi         =      0;
//   volatile int32_t      period_lo         =      0;

//   volatile uint8_t      tmr_overf         =      0;
//   volatile uint8_t      f_capture         =      0; // flag

//            float        duty_cycl         =    0.0;
//            float        freq_cntr         =    0.0;

// void setup()
// {
//   Serial.begin(115200);
//   in_String.reserve(200);

//   init_tmr4();
// }

// void loop()
// {
//   int32_t  tempr = 0;
//   char *   pEnd;
  
//   if(debug_osm) {
//     if(f_capture) {
            
//       uint32_t summ = period_hi + period_lo;
      
//       duty_cycl = period_hi;
//       duty_cycl *= 100.0;  // %
//       duty_cycl /= summ;
//       freq_cntr = 16000000.0 /summ;

//       Serial.print(F("\n\tFreq: "));
//       Serial.print(freq_cntr, 6);      
//       Serial.print(F("\tduty cycle: "));
//       Serial.print(duty_cycl, 4);      
//       f_capture = 0;
//       }  
//     delay(500);
//     }

//   serialEvent(); 

//   if( end_input) {
//     char cmd = in_String[0];
//     in_String[0] = '+';
    
//     if( cmd == 'd' ) {
//       debug_osm = 1 - debug_osm;
//       if(debug_osm) Serial.print(F("\nDebug aktiv."));
//       else          Serial.print(F("\nDebug de-aktiv."));
//       }
                        
//     in_String = "";
//     end_input= false;
//   }
// }

// void serialEvent() {
//   while (Serial.available()) {
//     char inChar = (char)Serial.read();
//     in_String += inChar;
//     if (inChar == '\n') {
//       end_input= true;
//     }
//   }
// }





// void init_tmr4(void)
// {  
//   TCCR4A = 0;
//   TCCR4B = 0;

//   TCCR4B |= (1<< CS40); // set prescaler to 16 MHz

//   TCCR4B |= (1<<ICNC4); // input noise canceler on
//   TCCR4B |= (1<<ICES4); // input capture edge select (lo-hi) 

//   TIMSK4 |= (1<<TOIE4); // Overflow Interrupt Enable 
//   TIMSK4 |= (1<<ICIE4); // InCaptureInterrupt Enable   
// }

// ISR(TIMER4_OVF_vect) {
//   tmr_overf++;  
// }

// ISR(TIMER4_CAPT_vect) {
//   static uint16_t last_v = 0;
//          uint16_t curr_v = ICR4;
//          uint32_t accuml = 0;

//   accuml  = curr_v + tmr_overf *65536UL;  
//   accuml -= last_v;    
//   last_v  = curr_v;
//   tmr_overf = 0;        

//   if(TCCR4B &  (1<<ICES4)) {  // lo-hi
//     TCCR4B &= ~(1<<ICES4);    // input capture edge select (hi-lo) next 
//     period_lo = accuml;
//     f_capture = 1;       
//     }
//   else {  // hi-lo
//     TCCR4B |= (1<<ICES4);    // input capture edge select (lo-hi) next 
//     period_hi = accuml;
//     }
// }