
#ifndef ARDUINO_H
	#include <Arduino.h>
#endif
///clase para la lectura de pwm de cualquier rango basado en el interrupt INT1///

class PWM_READ
{	public:
		PWM_READ(void);
		void init();
		float duty();
		uint8_t duty_256();
		uint16_t rpm();
		float freq();
		unsigned long test();
		static void PWM_ISR();
		uint16_t MAX_SPINDLE;
		uint8_t PWM_MIN;

	private:


};