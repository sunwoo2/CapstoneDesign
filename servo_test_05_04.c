#include <stdio.h>
#include <wiringPi.h>
#include <softPwm.h>

#define SERVO 1

int main(){
    char str;

    if( wiringPiSetup() == -1)
        return -1;

    softPwmCreate(SERVO, 0, 200);

    while(1){
        softPwmWrite(SERVO, 15);    // Adjust angle
        for(int i=0; i<10000; i++) {};   // Delay
        softPwmWrite(SERVO, 24);
        for(int i=0; i<10000; i++) {};
        softPwmWrite(SERVO, 5);
    }

    return 0;

}
