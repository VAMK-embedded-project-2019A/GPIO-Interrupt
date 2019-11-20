#include <iostream>
#include <cstdio>
#include <cstring>
#include <poll.h>
#include <stdlib.h>
#include <unistd.h> //for lseek
#include "GPIO.h"

int main()
{
	/* struct pollfd fdset[2];     // array of file descriptor
	int nfds = 2;               //number of interrupt
	int button1_fd,button2_fd, timeout, rc, len;
	int buf[1];
	int button1_pin = 25;
	int button2_pin = 22;
	int led_pin = 20;
	
	int count1 = 0;
	int count2 = 0;
	timeout = 3000;

	
	GPIO button1(button1_pin, INPUT);
	GPIO button2(button2_pin, INPUT);
	button1.setEdge("rising");
	button2.setEdge("rising");
	
	GPIO led(led_pin, OUTPUT);
	led.setValue(HIGH);

	
    std::cout << "test" << std::endl;
	
	while (1) {
		memset((void*)fdset, 0, sizeof(fdset)); //clear the fdset block of mermory

		fdset[0].fd = button1.fd;
		fdset[0].events = POLLPRI;
		
		fdset[1].fd = button2.fd;
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
				button1.is_pressed = true;
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
				button2.is_pressed = true;
				led.setValue(LOW);
				}
			}
		}

		fflush(stdout);
	}
	
	button1.fdClose();
	button2.fdClose(); */




	int button1 = 25;

	GPIO play;
	play.init(button1, INPUT, RISING);
	printf("%d\n", play.fd);
	
	ButtonPoll buttonpoll;
	buttonpoll.add(&play);

	//printf("%d\n", (buttonpoll.gpio_list(1))->fd);

	return 0;
}
