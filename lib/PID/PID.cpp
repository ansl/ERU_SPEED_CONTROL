#include <PID.h>


PID::PID(float Kp, float Kd, float Ki, int OUT_INIT=0, int RNG_OUT_MIN=0, int RNG_OUT_MAX=100, int ft=0){
    _Kp=Kp;
    _Kd=Kd;
    _Ki=Ki;

    _RNG_OUT_MIN=RNG_OUT_MIN;
    _RNG_OUT_MAX=RNG_OUT_MAX;
    _ft=ft;


    _IN_old=0;
    _OUT=OUT_INIT;
    _OUT_old=OUT_INIT;

    _delta_h=0;
    _int_err_h=0;
    _dt=0;
    _ot=0;

}
//void PID::Kp(float n){
//    _Kp=n;
//}
//void PID::Ki(float n){
//    _Kd=n;
//}
//void PID::Kd(float n){
//    _Ki=n;
//}
void PID::SetRangeOut(int min, int max){
    _RNG_OUT_MIN=min;
    _RNG_OUT_MAX=max;
}
void PID::Param(float p, float i ,float d){
    _Kp=p;
    _Kd=i;
    _Ki=d;
}
long PID::Evaluate(long IN,long TARGET){
    if (_OUT!=_OUT_old){
        if (abs(TARGET-IN) > _ft || TARGET==0){
            _dt=millis()-_ot;     //tiempo diferencial
            _ot=millis();        //Old time          
            _int_err_h=_int_err_h+(TARGET-IN)*_dt;
            _delta_h=1*_delta_h+_Kp*(TARGET-IN)+_Kd*(IN-_IN_old)/(_OUT-_OUT_old)+_Ki*_int_err_h;
            
            if(abs(_delta_h)>=1){
                _OUT=constrain(_OUT + int(round(_delta_h)),_RNG_OUT_MIN,_RNG_OUT_MAX);
                _delta_h=_delta_h-round(_delta_h);
            }     
        }   
    }
    else{
        if (abs(TARGET-IN) > _ft || TARGET==0){
            _dt=millis()-_ot;     //tiempo diferencial
            _ot=millis();        //Old time          
            _int_err_h=_int_err_h+(TARGET-IN)*_dt;
            _delta_h=1*_delta_h+_Kp*(TARGET-IN)+_Ki*_int_err_h;
            
            if(abs(_delta_h)>=1){
                _OUT=constrain(_OUT + int(round(_delta_h)),_RNG_OUT_MIN,_RNG_OUT_MAX);
                _delta_h=_delta_h-round(_delta_h);
            }
        }                   
    }

                
    _IN_old=IN;
    _OUT_old=_OUT;
    return _OUT;
}