#include "button.h"
#include "buttonpoll.h"

#include <cstdio>	// std::perror()

#include <unistd.h>	// write(), close(), read()
#include <fcntl.h>	// open()
#include <poll.h>	// poll()

Button::Button(int pin, int edge)
{
	_gpio_pin = pin;
	
	pinExport();
	setDirection();
	setEdge(edge);
	
	std::string path = SYSFS_GPIO_DIR + "/gpio" + std::to_string(_gpio_pin) + "/value";
	_fd = open(path.c_str(), O_RDONLY | O_NONBLOCK);
	if(_fd < 0)
		std::perror(path.c_str());
}

bool Button::pinExport()
{
	std::string path = SYSFS_GPIO_DIR + "/export";
	int fd = open(path.c_str(), O_WRONLY);
	if(fd < 0)
	{
		std::perror(path.c_str());
		return false;
	}
 
	auto data = std::to_string(_gpio_pin);
	write(fd, data.c_str(), data.length());
	close(fd);
	return true;
}

bool Button::setDirection()
{
	std::string path = SYSFS_GPIO_DIR + "/gpio" + std::to_string(_gpio_pin) + "/direction";
	int fd = open(path.c_str(), O_WRONLY);
	if(fd < 0)
	{
		std::perror(path.c_str());
		return false;
	}

	std::string data = "in";
	
	// TODO: why +1 here!!!
	write(fd, data.c_str(), data.length() + 1);
	close(fd);
	return true;
}

bool Button::setEdge(int edge)
{
	std::string path = SYSFS_GPIO_DIR + "/gpio" + std::to_string(_gpio_pin) + "/edge";
	int fd = open(path.c_str(), O_WRONLY);
	if(fd < 0)
	{
		std::perror(path.c_str());
		return false;
	}

	std::string data;
	switch(edge)
	{
		case ButtonPoll::Rising:
			data = "rising";
			break; 
		case ButtonPoll::Falling:
			data = "falling";
			break;
		case ButtonPoll::Both:
			data = "both";
			break;
		default:
			close(fd);
			return false;
	}
	write(fd, data.c_str(), data.length() + 1);
	close(fd);
	return true;
}