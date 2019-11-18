#include <iostream>
#include <cstdio> //stdio.h
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>


#include "GPIO.h"

GPIO::GPIO(int pin){
	_gpioPin = pin;
}

/****************************************************************
 * gpio_set_dir
 ****************************************************************/
int GPIO::setDir(pinDirection dir){
	int fd; // file descriptor
	char buf[MAX_BUF];

	snprintf(buf, sizeof(buf), SYSFS_GPIO_DIR  "/gpio%d/direction", _gpioPin);

	fd = open(buf, O_WRONLY);
	if (fd < 0) {
		perror("gpio/direction");
		return fd;
	}

	if (dir == OUTPUT)
		write(fd, "out", 4);
	else
		write(fd, "in", 3);
	//printf("%d", fd);
	close(fd);
	return 0;
}

int GPIO::setValue(pinValue val){
	int fd;
	char buf[MAX_BUF];

	snprintf(buf, sizeof(buf), SYSFS_GPIO_DIR "/gpio%d/value", _gpioPin);

	fd = open(buf, O_WRONLY);
	if (fd < 0) {
		perror("gpio/set-value");
		return fd;
	}

	if (val == LOW)
		write(fd, "0", 2);
	else
		write(fd, "1", 2);

	close(fd);
	return 0;
}
int GPIO::getValue(){
	int fd;
	char buf[MAX_BUF];
	char ch;
	int value;

	snprintf(buf, sizeof(buf), SYSFS_GPIO_DIR "/gpio%d/value", _gpioPin);

	fd = open(buf, O_RDONLY);
	if (fd < 0) {
		perror("gpio/get-value");
		return fd;
	}

	read(fd, &ch, 1);

	if (ch != '0') {
		value = 1;
	} else {
		value = 0;
	}

	close(fd);
	return value;
}

/****************************************************************
 * gpio_export
 ****************************************************************/
int GPIO::pinExport()
{
	int fd, len;
	char buf[MAX_BUF];
 
	fd = open(SYSFS_GPIO_DIR "/export", O_WRONLY);
	if (fd < 0) {
		perror("gpio/export");
		return fd;
	}
 
	len = snprintf(buf, sizeof(buf), "%d", _gpioPin);
	write(fd, buf, len);
	close(fd);
 
	return 0;
}

/****************************************************************
 * gpio_set_edge
 ****************************************************************/
int GPIO::setEdge(char *edge)
{
	int fd;
	char buf[MAX_BUF];

	snprintf(buf, sizeof(buf), SYSFS_GPIO_DIR "/gpio%d/edge", _gpioPin);

	fd = open(buf, O_WRONLY);
	if (fd < 0) {
		perror("gpio/set-edge");
		return fd;
	}

	write(fd, edge, strlen(edge) + 1);
	close(fd);
	return 0;
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

int GPIO::fdClose(int fd)
{
	return close(fd);
}
