#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include <fcntl.h>
#include <unistd.h>
#include <pthread.h>
#include <time.h>
#include <string.h>
#include <sys/time.h>
#include <signal.h>
#include <gps.h>
#include "ts_ipc_p1.h"
#include "ts_ipc_p1_cmd.h"
#include "cellular_task.h"
#include "ts_vlt_app_0022.h" 
#include "app_task.h"
#include "app_0022_init.h"
#include "ipc1_mpu_mod_init.h"
#include "geo_fence_task.h"
#define SERIAL_PORT "/dev/ttyUSB0"  // Change this to your device's port
#define BAUDRATE B230400          // Change this to your desired baud rate


int fd;
extern SApp0022Handle OApp0022Handle;
extern SIPC_P1_Handle OIPC_P1_Hdle;
static uint8_t IPCP1_Mstr_UartRxBuffer[2048];
struct gps_data_t OGps_data;
static void ipc1_master_serial_monitoring_task( void );
static void delayMilliseconds(unsigned int milliseconds) {
    usleep(milliseconds * 1000);
}
void RandomArray(size_t length, uint8_t *array);
uint32_t RandomInt(uint32_t min, uint32_t max);

void timer_callback(int signo)
{   
   
  app_0022_1sec_tick_isr(&OApp0022Handle);

}


int main() {
	int ret = 0;


  pthread_t cellular_task;
  pthread_t app_task;
  pthread_t geo_fence_task;

  if ((ret = gps_open("localhost", "2947", &OGps_data)) == -1) {
  fprintf(stderr, "gps_open() failed: %s\n", gps_errstr(ret));
  exit(EXIT_FAILURE);
  }

  // Enable JSON output
  gps_stream(&OGps_data, WATCH_ENABLE | WATCH_JSON, NULL);


  ret = pthread_create( &cellular_task, NULL, cellular_task_func,(void *)"Thread 1");
	if(ret)
	{
	fprintf(stderr,"Error - pthread_create() return code: %d\n\r",ret);
	 exit(EXIT_FAILURE);
	}

  ret = pthread_create( &app_task, NULL, app_task_func,(void *)"Thread 2");
	if(ret)
	{
	fprintf(stderr,"Error - pthread_create() app_thread return code: %d\n\r",ret);
	 exit(EXIT_FAILURE);
	}

  ret = pthread_create( &geo_fence_task, NULL,geo_fence_task_func,(void *)"Thread 3");
	if(ret)
	{
	fprintf(stderr,"Error - pthread_create() app_thread return code: %d\n\r",ret);
	 exit(EXIT_FAILURE);
	}

 
  fd = open(SERIAL_PORT, O_RDWR | O_NOCTTY | O_NDELAY);
  if (fd == -1) {
   // perror("open_port: Unable to open serial port");
   fprintf(stderr," serial port open error\n\r");
    return 1;
 
  }

 // Set the baud rate and other parameters
  struct termios options;
  tcgetattr(fd, &options);
  cfsetispeed(&options, BAUDRATE);
  cfsetospeed(&options, BAUDRATE);

  // Set parity, stop bits, data size, and flow control
  options.c_cflag &= ~PARENB; // Clear parity bit
  options.c_cflag &= ~CSTOPB; // Clear stop field
  options.c_cflag &= ~CSIZE; // Clear data size bits
  options.c_cflag |= CS8; // 8 bits per byte
//   options.c_cflag |= ~CRTSCTS; // Disable hardware flow control
  options.c_cflag |= CREAD | CLOCAL; // Enable read & ignore control lines

  // Set local flags
  options.c_lflag &= ~ICANON; // Disable canonical mode
  options.c_lflag &= ~ECHO; // Disable echo
  options.c_lflag &= ~ECHOE; // Disable erasure
  options.c_lflag &= ~ECHONL; // Disable new-line echo
  options.c_lflag &= ~ISIG; // Disable INTR, QUIT, and SUSP signals

  // Set input flags
  options.c_iflag &= ~(IXON | IXOFF | IXANY); // Disable software flow control
  options.c_iflag &= ~(IGNBRK | BRKINT | PARMRK | ISTRIP | INLCR | IGNCR | ICRNL); // Disable special handling of received bytes

  // Set output flags
  options.c_oflag &= ~OPOST; // Disable output post-processing
  options.c_oflag &= ~ONLCR; // Prevent conversion of newline to CR/LF

  tcsetattr(fd, TCSANOW, &options);

	ipc1_mpu_mod_init();


	srand((unsigned int)time(NULL));

  app_0022_mod_init();



    struct itimerval timer;
  timer.it_interval.tv_sec = 1;
  timer.it_interval.tv_usec = 0;
  timer.it_value = timer.it_interval;
    // Set the timer callback function
  signal(SIGALRM, timer_callback);

  // Start the timer
  ret = setitimer(ITIMER_REAL, &timer, NULL);
 
	while(1){
		ipc1_master_serial_monitoring_task();
	}
  
   
    
  while(1);
	pthread_join( cellular_task, NULL);

	exit(EXIT_SUCCESS);
  return 0;
}



static void ipc1_master_serial_monitoring_task( void )
{
	EIPC_P1_Errors err_code;
	uint8_t rd_cnt = 0;
	uint32_t event = 1;
	char print_data[1024];

	//printf("ipc1_master_serial_monitoring_task() started\r\n");

	for( ;; )
	{
		memset(IPCP1_Mstr_UartRxBuffer,0,sizeof(IPCP1_Mstr_UartRxBuffer));
		rd_cnt = read(fd,IPCP1_Mstr_UartRxBuffer,sizeof(IPCP1_Mstr_UartRxBuffer));
		if(rd_cnt > 0 && rd_cnt!=255)
		{	
      for(int i=0; i<rd_cnt; i++)
          {
          sprintf(&print_data[i*2], "%02X", IPCP1_Mstr_UartRxBuffer[i]);
          }
     // printf("%s",print_data);
      err_code = ipc_p1_frame_handler(&OIPC_P1_Hdle,IPCP1_Mstr_UartRxBuffer,rd_cnt);
      if(err_code != EIPC_P1_ERR_NO_ERROR)
      {	
       // printf("\r\nipc_p1_frame_handler() = %u", err_code);
      }
		}
		rd_cnt = 0;
		fflush( stdout );
	}
}



void RandomArray(size_t length, uint8_t *array) {
    // Generate random uint8_t values and store them in the array
    for (size_t i = 0; i < length; i++) {
        array[i] = (uint8_t)(rand() % 256); // Ensure the value is between 0 and 255
    }
}

uint32_t RandomInt(uint32_t min, uint32_t max) {

    // Generate a random value in the specified range
    return (rand() % (max - min + 1) )+ min;

    
}