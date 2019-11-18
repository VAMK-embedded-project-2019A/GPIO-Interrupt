#include <iostream>
#include <cstdio> 
#include <cstring>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>


#include "GPIO.h"

/****************************************************************
 * Constructor
 ****************************************************************/
GPIO::GPIO(int pin){
	_gpioPin = pin;
}

GPIO::GPIO(int pin,pinDirection dir){
	_gpioPin = pin;
	_dir = dir;
	GPIO::setDir(_dir);
	fd = GPIO::fdOpen();
}

/****************************************************************
 * pinExport
 ****************************************************************/
void GPIO::pinExport()
{
	int fd, len;
	char buf[MAX_BUF];
 
	fd = open(SYSFS_GPIO_DIR "/export", O_WRONLY);
	if (fd < 0) {
		perror("gpio/export");
		//return fd;
	}
 
	len = snprintf(buf, sizeof(buf), "%d", _gpioPin);
	write(fd, buf, len);
	close(fd);
}

/****************************************************************
 * setDir
 * to set direction of a GPIO pin
 ****************************************************************/
void GPIO::setDir(pinDirection dir){
	int fd; // file descriptor
	char buf[MAX_BUF];

	snprintf(buf, sizeof(buf), SYSFS_GPIO_DIR  "/gpio%d/direction", _gpioPin);

	fd = open(buf, O_WRONLY);
	if (fd < 0) {
		perror("gpio/direction");
		//return fd;
	}

	if (dir == OUTPUT)
		write(fd, "out", 4);
	else
		write(fd, "in", 3);
	close(fd);
}

/****************************************************************
 * setValue
 * to set Value of a GPIO pin
 ****************************************************************/
void GPIO::setValue(pinValue val){
	int fd;
	char buf[MAX_BUF];

	snprintf(buf, sizeof(buf), SYSFS_GPIO_DIR "/gpio%d/value", _gpioPin);

	fd = open(buf, O_WRONLY);
	if (fd < 0) {
		perror("gpio/set-value");
		//return fd;
	}

	if (val == LOW)
		write(fd, "0", 2);
	else
		write(fd, "1", 2);

	close(fd);
}

/****************************************************************
 * setEdge
 ****************************************************************/
void GPIO::setEdge(char *edge)
{
	int fd;
	char buf[MAX_BUF];

	snprintf(buf, sizeof(buf), SYSFS_GPIO_DIR "/gpio%d/edge", _gpioPin);

	fd = open(buf, O_WRONLY);
	if (fd < 0) {
		perror("gpio/set-edge");
		//return fd;
	}

	write(fd, edge, strlen(edge) + 1);
	close(fd);
}

/****************************************************************
 * gpio_fd_open
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
 * gpio_fd_close
 ****************************************************************/

int GPIO::fdClose()
{
	return close(fd);
}
