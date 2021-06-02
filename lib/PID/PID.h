#include <Arduino.h>

class PID
{ 	public:
		PID(float Kp, float Kd, float Ki, int OUT_INIT, int RNG_OUT_MIN=0, int RNG_OUT_MAX=100, int ft=0);//ft stands for a parameter used to set the threadshold for the evaluation of the PID
		//void Kp(float n);
		//void Kd(float n);
		//void Ki(float n);
		void SetRangeOut(int min, int max);
		void Param(float p, float i, float d);
		long Evaluate(long IN,long TARGET);//esta seria equivalente a RPM
  	private:
  		float _Kp;
		float _Kd;
		float _Ki;
    	long _RNG_OUT_MIN;
   		long _RNG_OUT_MAX;
    	int _ft;

    	long _IN;
    	long _IN_old;
    	long _OUT;
    	long _OUT_old;

    	float _delta_h;
    	double _int_err_h;
    	long _dt;
		long _ot;
};

