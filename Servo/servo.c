#include <stdio.h>
#include <pigpio.h>

#define SERVO 18

int main(int argc, char *argv[])
{
   double start;
   int tmp;

   if (gpioInitialise() < 0)
   {
      fprintf(stderr, "pigpio initialisation failed\n");
      return 1;
   }

   /* Set GPIO modes */
   gpioSetMode(SERVO, PI_OUTPUT);

   /* Start 1500 us servo pulses on GPIO 1 */
   //gpioServo(SERVO, 1500);

   start = time_time();

//   while ((time_time() - start) < 20.0)
   while(1)
   {
   	  /* Start 2.5% dutycycle PWM on GPIO 1 */
	  /* Left */
	  tmp = gpioPWM(SERVO, 100);
      printf("1 : %d \n", tmp); /* 6.375/255 = 2.5% */

      time_sleep(3);

   	  /* Start 7.5% dutycycle PWM on GPIO 1 */
	  /* Right */
	  tmp = gpioPWM(SERVO, 0);
      printf("2 : %d \n", tmp); /* 19.125/255 = 7.5% */

      time_sleep(3);

   	  /* Start 4.5% dutycycle PWM on GPIO 1 */
	  /* Straight */
      tmp = gpioPWM(SERVO, 255); /* 11.475/255 = 4.5% */
	  printf("3 : %d \n", tmp);

      time_sleep(3);
   }

   /* Stop DMA, release resources */
   gpioTerminate();

   return 0;
}
