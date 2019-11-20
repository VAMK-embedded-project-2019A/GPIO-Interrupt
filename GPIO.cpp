#include <iostream>
#include <cstdio> 
#include <cstring>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>

#include "GPIO.h"

/****************************************************************
 * GPIO::GPIO
 ****************************************************************/
GPIO::GPIO(){
}

/****************************************************************
 * GPIO::init
 ****************************************************************/
int GPIO::init(int pin, pinDirection dir, edge edge){
	_gpioPin = pin;
	_dir = dir;
	_edge = edge;
	GPIO::pinExport();
	GPIO::setDir(_dir);
	GPIO::setEdge(_edge);
	fd = GPIO::fdOpen();
	return fd;
}

/****************************************************************
 * GPIO::pinExport
 ****************************************************************/
void GPIO::pinExport()
{
	int fd, len;
	char buf[MAX_BUF];
 
	fd = open(SYSFS_GPIO_DIR "/export", O_WRONLY);
	if (fd < 0) {
		perror("gpio/export");
	}
 
	len = snprintf(buf, sizeof(buf), "%d", _gpioPin);
	write(fd, buf, len);
	close(fd);
}

/****************************************************************
 * GPIO::setDir
 * to set direction of a GPIO pin
 ****************************************************************/
void GPIO::setDir(pinDirection direction){
	int fd; // file descriptor
	char buf[MAX_BUF];

	snprintf(buf, sizeof(buf), SYSFS_GPIO_DIR  "/gpio%d/direction", _gpioPin);

	fd = open(buf, O_WRONLY);
	if (fd < 0) {
		perror("gpio/direction");
	}

	switch(direction) {
		case OUTPUT :
			write(fd, "out", 4);
			break; 
		case INPUT :
			write(fd, "in", 3);
			break;
	}	
	close(fd);
}

/****************************************************************
 * GPIO::setValue
 * to set Value of a GPIO pin
 ****************************************************************/
void GPIO::setValue(pinValue value){
	int fd;
	char buf[MAX_BUF];

	snprintf(buf, sizeof(buf), SYSFS_GPIO_DIR "/gpio%d/value", _gpioPin);

	fd = open(buf, O_WRONLY);
	if (fd < 0) {
		perror("gpio/set-value");
	}

	switch(value) {
		case LOW :
			write(fd, "0", 2);
			break; 
		case HIGH :
			write(fd, "1", 2);
			break;
	}
	close(fd);
}

/****************************************************************
 * GPIO::setEdge
 ****************************************************************/
void GPIO::setEdge(edge edge)
{
	int fd;
	char buf[MAX_BUF];

	snprintf(buf, sizeof(buf), SYSFS_GPIO_DIR "/gpio%d/edge", _gpioPin);

	fd = open(buf, O_WRONLY);
	if (fd < 0) {
		perror("gpio/set-edge");
		//return fd;
	}

	switch(edge) {
		case RISING :
			write(fd, "rising", 7);
			break; 
		case FALLING :
			write(fd, "falling", 8);
			break;
		case BOTH :
			write(fd, "both", 5);
			break;
	}
	close(fd);
}

/****************************************************************
 * GPIO::fdOpen
 ****************************************************************/

int GPIO::fdOpen()
{
	int fd;
	char buf[MAX_BUF];

	snprintf(buf, sizeof(buf), SYSFS_GPIO_DIR "/gpio%d/value", _gpioPin);

	fd = open(buf, O_RDONLY | O_NONBLOCK );
	if (fd < 0) {
		perror("gpio/fd_open");
	}
	return fd;
}

/****************************************************************
 * GPIO::fdClose
 ****************************************************************/

int GPIO::fdClose()
{
	return close(fd);
}

/****************************************************************
 * ButtonPoll Class
 ****************************************************************/
/****************************************************************
 * ButtonPoll::ButtonPoll
 ****************************************************************/
ButtonPoll::ButtonPoll(){
}

void ButtonPoll::add(GPIO* button){
	gpio_list.push_back(button);
}

/****************************************************************
 * ButtonPoll::polling
 ****************************************************************/
void ButtonPoll::polling(){
	int size = gpio_list.size();	//number of buttons/interrupts
	int rc;
	int timeout = 1000;
	int i,len;
	int buf[1];
	int count[size];

	//memset(count, 0, size);
	for(i =0; i<size; i++){
		count[i]=0;
	}
	while(1){
		fdset = new pollfd[size];

		for(i=0; i<size; i++){
			fdset[i].fd = gpio_list[i]->fd;
			fdset[i].events = POLLPRI;
		}

		rc = poll(fdset, size, timeout);
		if (rc < 0) {
			std::cout << std::endl << "poll()failed!" << std::endl;
		}

		if (rc == 0) {
			std::cout << ".";
		}

		if(rc>0){
			for(i=0; i<size; i++){
				if (fdset[i].revents & POLLPRI) {
					lseek(fdset[i].fd, 0, SEEK_SET);
					len = read(fdset[i].fd, &buf, 1);
					
					if(count[i] == 0){
						count[i]++;
						// Do Nothing for 1st interrupt
					}
					else{
						// ISR here
						std::cout << std::endl << "poll() GPIO interrupt occurred" << std::endl;
						std::cout << "read value: " << (char)buf[0] << std::endl;
						gpio_list[i]->is_pressed = true;
					}
				}
			}
		}
		fflush(stdout);
		delete fdset;
	}
}