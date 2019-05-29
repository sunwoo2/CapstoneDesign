#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <pigpio.h>

#define SERVO 18
#define ESC 27 // GPIO 2 (BCM 27) -> PWM ouput

int main(int argc, char *argv[]){
    int Hz_ESC, Hz_SERVO, Range_ESC, Range_SERVO, sel;

    if (gpioInitialise() < 0){
        fprintf(stderr, "pigpio initialisation failed\n");
        return 1;
    }

    /* Set GPIO modes */
    //gpioSetMode(ESC, PI_OUTPUT);

    // Set frequency as 40Hz
    Hz_ESC = gpioSetPWMfrequency(ESC, 40);
    printf("ESC Frequency : %d Hz \n", Hz_ESC);

    Hz_SERVO = gpioSetPWMfrequency(SERVO, 40);
    printf("SERVO Frequency : %d Hz \n", Hz_SERVO);

    // Set PWM Range
    Range_ESC = gpioSetPWMrange(ESC, 25000);
    printf("ESC Range is 0 - %d (return value)\n", Range_ESC);
    printf("ESC Range is 0 - %d (gpioGetPWMrange)\n", gpioGetPWMrange(ESC));

    Range_SERVO = gpioSetPWMrange(SERVO, 25000);
    printf("SERVO Range is 0 - %d (return value)\n", Range_SERVO);
    printf("SERVO Range is 0 - %d (gpioGetPWMrange)\n\n", gpioGetPWMrange(ESC));


    // Motion
    int left = 1200;     // 1200/25000 = 2.5%
    int straight = 1125; // 1125/25000 = 4.5%
    int right = 1875;    // 1875/25000 = 7.5%
    int forward_beef  = 1535;
    int backward_beef = 1465;
    int go = 1562;  // 1562.5/25000 = 6.25%
    int stop = 1500;    // 1500/25000 = 6%
    int back = 1437;    // 1437/25000 = 5.75%

    // turn left & stop
    if( !gpioPWM(SERVO, left) ) printf("left\n");
    if( !gpioPWM(ESC, go) ) printf("go\n");
    sleep(2);
    if( !gpioPWM(ESC, stop) ) printf("stop\n");

    // turn right & stop
    //if( !gpioPWM(SERVO, right) ) printf("right\n");
    //if( !gpioPWM(ESC, go) ) printf("go\n");
    //sleep(2);
    //if( !gpioPWM(ESC, stop) ) printf("stop\n");


    gpioTerminate();

   return 0;
}
