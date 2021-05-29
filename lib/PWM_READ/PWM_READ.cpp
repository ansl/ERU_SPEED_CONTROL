
#include <Arduino.h>
#include <pins_arduino.h>
#include <PWM_READ.h>

        volatile uint16_t      peak_width         =0;
        volatile uint16_t      valley_width       =0;
        volatile uint16_t      tmr_overf          =0;

PWM_READ::PWM_READ(){
    MAX_SPINDLE=9000;
    PWM_MIN=7;
    
}
void PWM_READ::init(){
    //using Timer 1 
         //inicializo timer1/normal mode
             TCCR1A=0;					//Inicializo los control registers del timer1 tanto el A como el B
             TCCR1B=0;
             TCCR1C=0;
             TIMSK1=0;

        
             TCCR1B |=(1 << ICNC1);             //input noise canceler-> It causes a delay of 4 clock cycles as it considers a change only if it persists for at least 4 successive system clocks.
             TCCR1B |=(1 << ICES1);             //Select edge detection for input capture function.0 = Capture on the falling edge1 = Capture on rising edge
             TCCR1B |=(1 << CS10);	  	//Seteo el prescaling en no prescaling
        
             TIMSK1 |=(1 << ICIE1);             //Input Capture Interrupt Enable
             TIMSK1 |= (1<<TOIE1);              // Overflow Interrupt Enable 
             TIFR1 |=(1<<ICF1);                 // clear Flag 

}
float PWM_READ::duty(){
        // return 10*lroundf((_range/10)*abs(CYCLE_END-DUTY_START)/CYCLE_END);
           return 1.0*(peak_width)/(peak_width+valley_width);

}
uint8_t PWM_READ::duty_256(){
        // return 10*lroundf((_range/10)*abs(CYCLE_END-DUTY_START)/CYCLE_END);
         
           return round(256.0*(peak_width)/(peak_width+valley_width));

}
uint16_t PWM_READ::rpm(){
        // return 10*lroundf((_range/10)*abs(CYCLE_END-DUTY_START)/CYCLE_END);
          uint8_t duty_256=round(256.0*(peak_width)/(peak_width+valley_width));
          if (duty_256>PWM_MIN){
            return round(1.0*duty_256*MAX_SPINDLE/256);
            //return MAX_SPINDLE;
          }
          else{
            return 0;
          }
}
float PWM_READ::freq(){
        // return 10*lroundf((_range/10)*abs(CYCLE_END-DUTY_START)/CYCLE_END);
        return F_CPU/(peak_width+valley_width);
}

void PWM_READ::PWM_ISR(){
    uint32_t curr_v = ICR1;
    static uint32_t last_v;

    curr_v  = curr_v + tmr_overf * 65536UL; 

    if(((TCCR1B>>ICES1)&1)==0 && curr_v>last_v){
      peak_width=curr_v-last_v;

    }
    if(((TCCR1B>>ICES1)&1)==1 && curr_v>last_v){
      valley_width=curr_v-last_v;
      // Freq=F_CPU/(peak_width+valley_width);
      // Duty=1.0*peak_width/(peak_width+valley_width);
      // Duty_256=round(255.0*peak_width/(peak_width+valley_width));
    }
        
    TCCR1B ^= (1<<ICES1);  // invert flange detection

    last_v=curr_v;
    
}

ISR(TIMER1_CAPT_vect){
    PWM_READ::PWM_ISR();
}

ISR(TIMER1_OVF_vect) {
   tmr_overf++;  
 }