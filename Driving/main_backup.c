#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <pigpio.h>

#define SERVO 18
#define ESC 27 // GPIO 2 (BCM 27) -> PWM ouput

void* servo();
void* esc();

int main(int argc, char *argv[]){
   
    pthread_t t_servo,t_esc;
    int result;

    result = pthread_create(&t_servo, NULL, servo, NULL);
    if(result != 0){
        perror("Create servo thread failed!");
        exit(1);
    }
    result = pthread_create(&t_esc, NULL, esc, NULL);
    if(result != 0){
        perror("Create esc thread failed!");
        exit(1);
    }

    result = pthread_join(t_servo, NULL);
    if(result != 0){
        perror("Join servo thread failed!");
        exit(1);
    }
    result = pthread_join(t_esc, NULL);
    if(result != 0){
        perror("Join esc thread failed!");
        exit(1);
    }

   return 0;
}

void* servo(){
   // Range : 0~255 (default)
   double left = 6.375; // 2.5%
   double straight = 11.475;    // 4.5%
   double right = 19.125;   // 7.5%

   if (gpioInitialise() < 0)
   {
      fprintf(stderr, "pigpio initialisation failed\n");
      return 1;
   }

   // Set GPIO modes 
   gpioSetMode(SERVO, PI_OUTPUT);

   // Start 1500 us servo pulses on GPIO 1
   //gpioServo(SERVO, 1500);

    while(1){

        if( !gpioPWM(SERVO, left) ){
            printf("left\n");
        }
        sleep(2);

        if( !gpioPWM(SERVO, straight) ){
            printf("straight\n");
        }
        sleep(2);

        if( !gpioPWM(SERVO, right) ){
            printf("right\n");
        }
        sleep(2);

   }

    gpioTerminate();

    pthread_exit(0);
}

void* esc(){
    int Hz, range;
    double pwm;

    if (gpioInitialise() < 0){
        fprintf(stderr, "pigpio initialisation failed\n");
        return 1;
    }

    /* Set GPIO modes */
    gpioSetMode(ESC, PI_OUTPUT);

    // Set frequency as 40Hz
    Hz = gpioSetPWMfrequency(ESC, 40);
    printf("Frequency is %d Hz \n", Hz);

    // Set PWM Range
    range = gpioSetPWMrange(ESC, 25000);
    printf("Set Range is 0 - %d  \n", range);
    //printf("Set range is 0 - %d \n", gpioGetPWMrange(ESC));

    // Data
    // forward  : 6.25% (1562.5)
    // stop     : 6%    (1500)
    // backward : 5.75% (1437.5)
    int forward_beef  = 1535;
    int backward_beef = 1465;
    int stop          = 1500;


    // Motion
    while(1){

        if( !gpioPWM(ESC, stop) ) printf("tmp = %d \n", stop);
        sleep(1);

        // forward
        if( !gpioPWM(ESC, forward_beef) ) printf("tmp = %d \n", forward_beef);
        sleep(1);
        pwm = 1550;
        if( !gpioPWM(ESC, pwm) ) printf("tmp = %d \n", pwm);
        sleep(2);

        // stop
        if( !gpioPWM(ESC, stop) ) printf("tmp = %d \n", stop);
        sleep(2);

        // backward
        if( !gpioPWM(ESC, backward_beef) ) printf("tmp = %d \n", backward_beef);
        sleep(1);
        pwm = 1450;
        if( !gpioPWM(ESC, pwm) ) printf("tmp = %d \n", pwm);
        sleep(2);

        // stop
        if( !gpioPWM(ESC, stop) ) printf("tmp = %d \n", stop);
        sleep(2);

    }


    /* Stop DMA, release resources */
    gpioTerminate();

    pthread_exit(0);
}
