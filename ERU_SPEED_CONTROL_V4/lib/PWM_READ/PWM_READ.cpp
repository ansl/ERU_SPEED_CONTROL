
#include <Arduino.h>
#include <pins_arduino.h>
#include <PWM_READ.h>

extern volatile unsigned long timer0_overflow_count;
volatile unsigned long t_abs=0;
volatile unsigned long t_abs_old=0;
volatile unsigned long CYCLE_END=0;
volatile unsigned long DUTY_START=0;

ISR(INT1_vect){
    PWM_READ::PWM_ISR();
}

PWM_READ::PWM_READ(long rng){

    EIMSK |= (1 << INT1);       // Enable external interrupt INT1 #pin3
    EICRA |= (1 << ISC11);      // Trigger INT1 on falling edge
    _range=rng;
}
long PWM_READ::duty(){
        return 10*lroundf((_range/10)*abs(CYCLE_END-DUTY_START)/CYCLE_END);
}
unsigned long PWM_READ::test(){
        return DUTY_START;

}
void PWM_READ::PWM_ISR(){
                uint8_t oldSREG = SREG;

                if (((EICRA>>2) & 0x01)==0){
                    t_abs_old=t_abs;
                    //DETECTA CAIDA lee el final del ciclo, resetea el timer y cambia la deteccion a rising edge/ calcula los periodos
                    t_abs=(timer0_overflow_count*256+TCNT0); //timer0_overflow_count contabiliza cuantas veces se llega la overflow *256(tamaño del timer)
                    CYCLE_END=t_abs-t_abs_old;
                    EICRA |= (1 << ISC10);//cambia la deteccion de flanco a rising edge
                }
                else if ((EICRA>>2) & 0x01==1){ 
 
                    //DETECTA SUBIA lee el duty y cambia la deteccion a falling edge
                    DUTY_START=(timer0_overflow_count*256+TCNT0)-t_abs;//lee dode se produce la
                    EICRA ^= (1 << ISC10); //cambia la deteccion de flanco a falling edge

                }
                SREG = oldSREG;
            
}