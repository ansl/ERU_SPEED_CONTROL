
#include <LCD_MENU.h>




Menu M1(2,0,&lcd,UP,DOWN,SET,BACK,NEXT);
Menu M20(1,1,&lcd,UP,DOWN,SET,BACK,NEXT);

//Define Menu Pannels
    M1.pick[0].picker_name="TORNO";
    M1.pick[0].mode=1;
    M1.pick[1].picker_name="PLATO";
    M1.pick[1].mode=1;
    M1.pick[2].picker_name="ERU CON";
    M1.pick[2].mode=1;
    //M1.print_menu();
    
    M20.pick[0].picker_name="SPEED:";
    M20.pick[0].mode=3;
    M20.pick[0].unit="RPM";
    M20.pick[0].decimals=0;
    M20.pick[0].inc_short=10;
    M20.pick[0].inc_long=100;
    M20.pick[0].max_value=150;
    M20.pick[0].min_value=0;
    M20.header="TORNO";
    
    Serial.begin(9600);     //Inicializo el serial print
    Panel=&M1; //inicializo en panel principal
    Panel->print_menu();