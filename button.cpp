#include "button.h"
#include "buttonpoll.h"

#include <cstdio>	// std::perror()

#include <fcntl.h>	// open()
#include <poll.h>	// poll()
#include <fstream>	// std::fstream

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
	std::fstream file;
	
	file.open(path);
	if(!file.is_open()){
		std::perror(path.c_str());
		return false;
	}

	auto data = std::to_string(_gpio_pin);
	file << data;
	file.close();
	return true;
}

bool Button::setDirection()
{
	std::string path = SYSFS_GPIO_DIR + "/gpio" + std::to_string(_gpio_pin) + "/direction";
	std::fstream file;
	
	file.open(path);
	if(!file.is_open()){
		std::perror(path.c_str());
		return false;
	}
	
	std::string data = "in";
	file << data;
	file.close();
	return true;
}

bool Button::setEdge(int edge)
{
	std::string path = SYSFS_GPIO_DIR + "/gpio" + std::to_string(_gpio_pin) + "/edge";
	std::fstream file;
	
	file.open(path);
	if(!file.is_open()){
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
			file.close();
			return false;
	}
	file << data;
	file.close();
	return true;
}