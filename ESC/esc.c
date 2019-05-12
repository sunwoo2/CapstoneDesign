#include <stdio.h>
#include <pigpio.h>

#define ESC 27 // GPIO 2 (BCM 27) --> PWM output

int main(int argc, char *argv[])
{
   double start;
   int Hz, range;
   int tmp = 1500;

   if (gpioInitialise() < 0)
   {
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
   printf("Range is 0 - %d  \n", range);

   printf("Set range is 0 - %d \n", gpioGetPWMrange(ESC));



   start = time_time();
      if( !gpioPWM(ESC, tmp) ) { // 1437.5/25000 = 5.75% 
	  	printf("tmp = %d \n", tmp);
	  }
      time_sleep(3);
	  tmp = 1450;
      if( !gpioPWM(ESC, tmp) ) { // 1437.5/25000 = 5.75% 
	  	printf("tmp = %d \n", tmp);
	  }
      time_sleep(5);
	  tmp = 1500;
      if( !gpioPWM(ESC, tmp) ) { // 1437.5/25000 = 5.75% 
	  	printf("tmp = %d \n", tmp);
	  }
      time_sleep(5);
	  tmp = 1550;
      if( !gpioPWM(ESC, tmp) ) { // 1437.5/25000 = 5.75% 
	  	printf("tmp = %d \n", tmp);
	  }
/*
   // 전진 경적 소리 : 1535, 전진 시작 : 1540
   for(int i=0; i<50; i++){
      if( !gpioPWM(ESC, tmp) ) { // 1437.5/25000 = 5.75% 
	  	printf("tmp = %d \n", tmp);
	  }
      time_sleep(0.5);
	  tmp--;
   }
   // 후진 경적 소리 : 1465
   for(int i=0; i<100; i++){
      if( !gpioPWM(ESC, tmp) ) { // 1437.5/25000 = 5.75% 
	  	printf("tmp = %d \n", tmp);
	  }
      time_sleep(0.5);
	  tmp++;
   }
   for(int i=0; i<50; i++){
      if( !gpioPWM(ESC, tmp) ) { // 1437.5/25000 = 5.75% 
	  	printf("tmp = %d \n", tmp);
	  }
      time_sleep(0.5);
	  tmp--;
   }
*/
   // while ((time_time() - start) < 10.0)
   while(1)
   {

   	  // Start 5.75% dutycycle PWM on GPIO 2 
	  // Backward
	   /*
      if( !gpioPWM(ESC, tmp) ) { // 1437.5/25000 = 5.75% 
	  	printf("tmp = %d \n", tmp);
	  }
      time_sleep(0.5);

	  tmp--;
	 */ 
/*
   	  // Start 6.25% dutycycle PWM on GPIO 2 
	  // Drive 
      if( !gpioPWM(ESC, 1555) ) { // 1562.5/25000 = 6.25% 
		printf("PWM 6.05% \n");
	  }

      time_sleep(2);


   	  // Start 6% dutycycle PWM on GPIO 2 
	  // Stop 
      if( !gpioPWM(ESC, 1500) ) { // 1500/25000 = 6% 
	  	printf("PWM 6% \n");
	  }
      time_sleep(3);


   	  // Start 5.75% dutycycle PWM on GPIO 2 
	  // Backward 
      if( !gpioPWM(ESC, 1435) ) { // 1437.5/25000 = 5.75% 
	  	printf("PWM 5.75% \n");
	  }
      time_sleep(2);


   	  // Start 6% dutycycle PWM on GPIO 2 
	  // Stop 
      if( !gpioPWM(ESC, 1500) ) { // 1500/25000 = 6% 
	  	printf("PWM 6% \n");
	  }
      time_sleep(3);
*/	  
   }

   /* Stop DMA, release resources */
   gpioTerminate();

   return 0;
}
