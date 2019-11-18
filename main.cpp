#include <iostream>
#include <cstdio>
#include <cstring>
#include <poll.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>


#include "GPIO.h"

int main()
{
	struct pollfd fdset[2];     // array of file descriptor
	int nfds = 2;               //number of interrupt
	int button1_fd,button2_fd, timeout, rc, len;
	int buf[1];
	int button1_pin = 25;
	int button2_pin = 22;
	int led_pin = 20;
	
	int count1 = 0;
	int count2 = 0;
	timeout = 3000;


    GPIO button1(button1_pin);
    GPIO button2(button2_pin);
    GPIO led(led_pin);

	button1.pinExport();
    button1.setDir(INPUT);
    button1.setEdge("rising");
	button1_fd = button1.fdOpen();
	
	button2.pinExport();
	button2.setDir(INPUT);
    button2.setEdge("rising");
	button2_fd = button2.fdOpen();
	
	led.setDir(OUTPUT);
	led.setValue(HIGH);

	
    std::cout << "test" << std::endl;
	
	while (1) {
		memset((void*)fdset, 0, sizeof(fdset)); //clear the fdset block of mermory

		fdset[0].fd = button1_fd;
		fdset[0].events = POLLPRI;
		
		fdset[1].fd = button2_fd;
		fdset[1].events = POLLPRI;

		rc = poll(fdset, nfds, timeout);

		if (rc < 0) {
			std::cout << std::endl << "poll()failed!" << std::endl;
			return -1;
		}

		if (rc == 0) {
			std::cout << ".";
			led.setValue(HIGH);
		}

		if(rc>0){
			if (fdset[0].revents & POLLPRI) {
				lseek(fdset[0].fd, 0, SEEK_SET);
				len = read(fdset[0].fd, &buf, 1);
				if(count1 == 0){
					count1++;
					// Do Nothing for 1st interrupt
				}
				else{
					// ISR here
				std::cout << std::endl << "poll() GPIO " << button1_pin << " interrupt occurred" << std::endl;
				std::cout << "read value: " << (char)buf[0] << std::endl;
				led.setValue(LOW);
				}
			}
			
		

			if (fdset[1].revents & POLLPRI) {
				lseek(fdset[1].fd, 0, SEEK_SET);
				len = read(fdset[1].fd, &buf, 1);
				if(count2 == 0){
					count2++;
					// Do Nothing for 1st interrupt
				}
				else{
					// ISR here
				std::cout << std::endl << "poll() GPIO " << button2_pin << " interrupt occurred" << std::endl;
				std::cout << "read value: " << (char)buf[0] << std::endl;
				led.setValue(LOW);
				}
			}
		}

		fflush(stdout);
	}
	button1.fdClose(button1_fd);
	button1.fdClose(button2_fd); 
	return 0;
}
