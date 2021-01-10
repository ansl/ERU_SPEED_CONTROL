
#include <Arduino.h>

///clase para la lectura de pwm de cualquier rango basado en el interrupt INT1///

class PWM_READ
{	public:
		PWM_READ(long rng);
		long duty();
		unsigned long test();
		static void PWM_ISR();

	private:
		long _range;

};