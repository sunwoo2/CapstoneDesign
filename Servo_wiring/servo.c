#include <stdio.h>
#include <wiringPi.h>
#include <softPwm.h>

#define SERVO 1

int main(int argc, char *argv[])
{
   double start;
   int i;

   if( wiringPiSetup() == -1 )
	   return 1;

   softPwmCreate(SERVO, 0, 200);

//   while ((time_time() - start) < 20.0)
   while(1)
   {
   	  /* Start 2.5% dutycycle PWM on GPIO 1 */
	  /* Left */
	  softPwmWrite(SERVO, 15);
      //printf("1 : %d \n", tmp); /* 6.375/255 = 2.5% */

	  for(i = 0; i < 20000; i++) {}
      //time_sleep(3);

   	  /* Start 7.5% dutycycle PWM on GPIO 1 */
	  /* Right */
	  softPwmWrite(SERVO, 24);
      //printf("2 : %d \n", tmp); /* 19.125/255 = 7.5% */

      //time_sleep(3);
	  for(i = 0; i < 20000; i++) {}

   	  /* Start 4.5% dutycycle PWM on GPIO 1 */
	  /* Straight */
      softPwmWrite(SERVO, 5); /* 11.475/255 = 4.5% */
	  //printf("3 : %d \n", tmp);

	  for(i = 0; i < 20000; i++) {}
      //time_sleep(3);
   }

   /* Stop DMA, release resources */
   //gpioTerminate();

   return 0;
}
