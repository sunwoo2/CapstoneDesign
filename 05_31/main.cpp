/*
 *  RPLIDAR
 *  Ultra Simple Data Grabber Demo App
 *
 *  Copyright (c) 2009 - 2014 RoboPeak Team
 *  http://www.robopeak.com
 *  Copyright (c) 2014 - 2019 Shanghai Slamtec Co., Ltd.
 *  http://www.slamtec.com
 *
 */
/*
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <pigpio.h>

#include "rplidar.h" //RPLIDAR standard sdk, all-in-one header

#ifndef _countof
#define _countof(_Array) (int)(sizeof(_Array) / sizeof(_Array[0]))
#endif

#ifdef _WIN32
#include <Windows.h>
#define delay(x)   ::Sleep(x)
#else
#include <unistd.h>
static inline void delay(_word_size_t ms){
    while (ms>=1000){
        usleep(1000*1000);
        ms-=1000;
    };
    if (ms!=0)
        usleep(ms*1000);
}
#endif

#define ESC 		27
#define SERVO 		18


// Range : 25000
// Max!!
// left  	: 1200~1540 
// straight : 1540
// right 	: 1540~1880
// go		: 
// stop	 	: 1200
#define LEFT		1200     
#define STRAIGHT	1520 
#define RIGHT		1750    

#define GO			1370
#define STOP		1200 


using namespace rp::standalone::rplidar;

bool checkRPLIDARHealth(RPlidarDriver * drv)
{
    u_result     op_result;
    rplidar_response_device_health_t healthinfo;


    op_result = drv->getHealth(healthinfo);
    if (IS_OK(op_result)) { // the macro IS_OK is the preperred way to judge whether the operation is succeed.
        printf("RPLidar health status : %d\n", healthinfo.status);
        if (healthinfo.status == RPLIDAR_STATUS_ERROR) {
            fprintf(stderr, "Error, rplidar internal error detected. Please reboot the device to retry.\n");
            // enable the following code if you want rplidar to be reboot by software
            // drv->reset();
            return false;
        } else {
            return true;
        }

    } else {
        fprintf(stderr, "Error, cannot retrieve the lidar health code: %x\n", op_result);
        return false;
    }
}

#include <signal.h>
bool ctrl_c_pressed;
void ctrlc(int)
{
    ctrl_c_pressed = true;
}

int main(int argc, const char * argv[]) {
	clock_t start, end;

/****** Initialize motors ********/
    int Hz_ESC, Hz_SERVO, Range_ESC, Range_SERVO;
	unsigned int setpoint = 1540;
	//unsigned int val;

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




	// Time for ready
    delay(1000);

	// ESC Initializing
    if( !gpioPWM(ESC, STOP) ) { 
		printf("\nESC Initialize! \n");
    }
	// SERVO Initializing
    if( !gpioPWM(SERVO, STRAIGHT) ) {
 		printf("SERVO Initialize! \n\n");
	}


	// Start
	// 1330 하면 안감
    if( !gpioPWM(ESC, 1600) ) printf("Start\n"); 
    delay(1000);
    if( !gpioPWM(ESC, 1550) ) printf("Start\n"); 

/***********************/


    const char * opt_com_path = NULL;
    _u32         baudrateArray[2] = {115200, 256000};
    _u32         opt_com_baudrate = 0;
    u_result     op_result;

    bool useArgcBaudrate = false;

    printf("Ultra simple LIDAR data grabber for RPLIDAR.\n"
           "Version: "RPLIDAR_SDK_VERSION"\n");

    // read serial port from the command line...
    if (argc>1) opt_com_path = argv[1]; // or set to a fixed value: e.g. "com3" 

    // read baud rate from the command line if specified...
    if (argc>2)
    {
        opt_com_baudrate = strtoul(argv[2], NULL, 10);
        useArgcBaudrate = true;
    }

    if (!opt_com_path) {
#ifdef _WIN32
        // use default com port
        opt_com_path = "\\\\.\\com3";
#elif __APPLE__
        opt_com_path = "/dev/tty.SLAB_USBtoUART";
#else
        opt_com_path = "/dev/ttyUSB0";
#endif
    }

    // create the driver instance
	RPlidarDriver * drv = RPlidarDriver::CreateDriver(DRIVER_TYPE_SERIALPORT);
    if (!drv) {
        fprintf(stderr, "insufficent memory, exit\n");
        exit(-2);
    }
    
    rplidar_response_device_info_t devinfo;
    bool connectSuccess = false;
    // make connection...
    if(useArgcBaudrate)
    {
        if(!drv)
            drv = RPlidarDriver::CreateDriver(DRIVER_TYPE_SERIALPORT);
        if (IS_OK(drv->connect(opt_com_path, opt_com_baudrate)))
        {
            op_result = drv->getDeviceInfo(devinfo);

            if (IS_OK(op_result)) 
            {
                connectSuccess = true;
            }
            else
            {
                delete drv;
                drv = NULL;
            }
        }
    }
    else
    {
        size_t baudRateArraySize = (sizeof(baudrateArray))/ (sizeof(baudrateArray[0]));
        for(size_t i = 0; i < baudRateArraySize; ++i)
        {
            if(!drv)
                drv = RPlidarDriver::CreateDriver(DRIVER_TYPE_SERIALPORT);
            if(IS_OK(drv->connect(opt_com_path, baudrateArray[i])))
            {
                op_result = drv->getDeviceInfo(devinfo);

                if (IS_OK(op_result)) 
                {
                    connectSuccess = true;
                    break;
                }
                else
                {
                    delete drv;
                    drv = NULL;
                }
            }
        }
    }
    if (!connectSuccess) {
        
        fprintf(stderr, "Error, cannot bind to the specified serial port %s.\n"
            , opt_com_path);
        goto on_finished;
    }

    // print out the device serial number, firmware and hardware version number..
    printf("RPLIDAR S/N: ");
    for (int pos = 0; pos < 16 ;++pos) {
        printf("%02X", devinfo.serialnum[pos]);
    }

    printf("\n"
            "Firmware Ver: %d.%02d\n"
            "Hardware Rev: %d\n"
            , devinfo.firmware_version>>8
            , devinfo.firmware_version & 0xFF
            , (int)devinfo.hardware_version);



    // check health...
    if (!checkRPLIDARHealth(drv)) {
        goto on_finished;
    }

    signal(SIGINT, ctrlc);
    
    drv->startMotor();
    // start scan...
    drv->startScan(0,1);


    // fetech result and print it out...
    while (1) {

        rplidar_response_measurement_node_t nodes[8192];
        size_t   count = _countof(nodes);
        op_result = drv->grabScanData(nodes, count);
        double cur_angle, dist_from_lidar, exist_from_lidar, cur_dist45, cur_dist315, 
               pre_dist45, pre_dist315;
        // -15도~15도. dist45=1.8m기준으로 약 1m거리 정도.
        // stop을 위해
        double dist15, dist345, dist0, pre_dist15, pre_dist345, pre_dist0;

        if (IS_OK(op_result)) {
            drv->ascendScanData(nodes, count);


            // Set data from lidar
            for (int pos = 0; pos < (int)count ; ++pos) {

					cur_angle = (nodes[pos].angle_q6_checkbit >> RPLIDAR_RESP_MEASUREMENT_SYNCBIT)/64.0f;
					dist_from_lidar = nodes[pos].distance_q2/4.0f;  // 단위 : mm
                    exist_from_lidar = nodes[pos].sync_quality;     // data가 존재하면 양수. 맞나?

    				if( cur_angle > 44.5 && cur_angle < 45.5 ) {
                		if( exist_from_lidar ) {

							//cur_dist45 = (pre_dist45 + dist_from_lidar)/2;
                            cur_dist45 = dist_from_lidar;
                        	//printf("45도 - theta: %03.2f Dist: %08.2f Q: %d \n", cur_dist45, dist_from_lidar, exist_from_lidar);
                            pre_dist45 = cur_dist45;

						} else {
                            //printf("45도 Q 제대로 안나옴\n");
							cur_dist45 = pre_dist45;
						}
                    } else if( cur_angle > 314.5 && cur_angle < 315.5 ) {
                		if( exist_from_lidar ) {

							//cur_dist315 = (pre_dist315 + dist_from_lidar)/2;
                            cur_dist315 = dist_from_lidar;
                        	//printf("315도 - theta: %03.2f Dist: %08.2f Q: %d \n", cur_dist315, dist_from_lidar, exist_from_lidar);
                            pre_dist315 = cur_dist315;

						} else {
                            //printf("315도 Q 제대로 안나옴\n");
							cur_dist315 = pre_dist315;
						}
                    } else if( cur_angle > 344.5 && cur_angle < 345.5 ) {
                		if( exist_from_lidar ){
                            dist345 = dist_from_lidar;
                            pre_dist345 = dist345;
                        }else 
                            dist345 = pre_dist345; 
                    } else if( cur_angle > 359.5 && cur_angle < 0.5 ) {
                		if( exist_from_lidar ){
                            dist0 = dist_from_lidar;
                            pre_dist0 = dist0;
                        }else 
                            dist0 = pre_dist0; 
                    } else if( cur_angle > 14.5 && cur_angle < 15.5 ) {
                		if( exist_from_lidar ){
                            dist15 = dist_from_lidar;
                            pre_dist15 = dist15;
                        }else 
                            dist15 = pre_dist15; 
					}
            }// for문

        }

        if (ctrl_c_pressed){ 
            break;
        }

        //** 제어 algorithm **//
		double diff = cur_dist45 - cur_dist315;
        double Kp1 = 0.3;     // 비례제어 상수 B값. 거리차 10cm 이상 날때
        double Kp2 = 0.2;      // 비례제어 상수 B값. 거리차 10cm 이하 일때
        int limit_diff = 300;   // 300mm = 30cm
        int stop_diff  = 300;
		printf("Difference of distance (Left - Right): %08.2f \n", diff);

        //** stop (30cm 이하면 stop)

/*
        if( dist0 < stop_diff ){
            gpioPWM(ESC, STOP);
			printf("0도 장애물 stop\n");
		}
        if( dist15 < stop_diff ){
            gpioPWM(ESC, STOP);
			printf("15도 장애물 stop\n");
		}
        if( dist345 < stop_diff ){
            gpioPWM(ESC, STOP);
			printf("-15도 장애물 stop\n");
		}
*/


		if( fabs(diff) >= limit_diff ) {   

            // setpoint = 1540
			setpoint += (int)(diff*Kp1);   
			//printf("거리차 10cm이상 diff : %08.2f \n", diff);

			if( setpoint <= LEFT ) { setpoint = LEFT; }
			if( setpoint >= RIGHT ) { setpoint = RIGHT; }

    		if( !gpioPWM(SERVO, setpoint) )
				(setpoint < STRAIGHT) ? printf("Left %d \n", setpoint) : printf("Right %d \n", setpoint);

		} else {
			//printf("Same direction! \n");
			//printf("거리차 10cm이하 diff : %08.2f \n", diff);

			setpoint += (STRAIGHT - setpoint)*Kp2;

    		if( !gpioPWM(SERVO, setpoint) ) printf("Straight! \n");
		}

    }


    drv->stop();
    drv->stopMotor();
	return 0;
    // done!
on_finished:
    RPlidarDriver::DisposeDriver(drv);
    drv = NULL;
    return 0;
}
