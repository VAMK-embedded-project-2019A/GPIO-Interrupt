#include <iostream>
#include <cstdio>	// std::perror()
#include <cstdlib>
#include <algorithm>
#include <chrono>
#include <thread>

#include <errno.h>
#include <unistd.h>	// lseek()
#include <fcntl.h>	// read(), write(), open()
#include <poll.h>	// poll()

#include "GPIO.h"

GPIO::GPIO(int pin, int edge)
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

bool GPIO::pinExport()
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

bool GPIO::setDirection()
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

bool GPIO::setEdge(int edge)
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

ButtonPoll::~ButtonPoll()
{
	std::for_each(_gpio_vec.begin(), _gpio_vec.end(), [](GPIO &gpio)
	{
		close(gpio._fd);
	});
}

void ButtonPoll::addButton(int pin, TriggerEdge edge)
{
	_gpio_vec.push_back(GPIO(pin, static_cast<int>(edge)));
}

bool ButtonPoll::isButtonPressed()
{
	return !_pressed_queue.empty();
}

int ButtonPoll::getNextPressedPin()
{
	if(_pressed_queue.empty())
		return -1;

	int pin = _gpio_vec.at(_pressed_queue.front())._gpio_pin;
	_pressed_queue.pop();
	return pin;
}

void ButtonPoll::start()
{
	const int BUTTON_COUNT = _gpio_vec.size();
	std::vector<bool> first_interrupt(BUTTON_COUNT, true);
	std::chrono::milliseconds timeout{700};

	while(true)
	{
		struct pollfd *fdset = new pollfd[BUTTON_COUNT];

		for(int i=0; i<BUTTON_COUNT; i++)
		{
			fdset[i].fd = _gpio_vec[i]._fd;
			fdset[i].events = POLLPRI;
		}

		// blocking until at least one fd ready, or timeout
		int ready_count = poll(fdset, BUTTON_COUNT, timeout.count());
		if(ready_count < 0)
		{
			std::cout << std::endl << "poll() failed!" << std::endl;
			goto NEXT_POLL;
		}
		if(ready_count == 0)
		{
			std::cout << ".";
			goto NEXT_POLL;
		}

		for(int i=0; i<BUTTON_COUNT; i++)
		{
			if(fdset[i].revents & POLLPRI)
			{
				lseek(fdset[i].fd, 0, SEEK_SET);

				const int BUFFER_SIZE{1};
				int buf[BUFFER_SIZE];
				read(fdset[i].fd, &buf, BUFFER_SIZE);

				// do nothing for 1st interrupt
				if(first_interrupt.at(i))
				{
					first_interrupt[i] = false;
					continue;
				}

				std::cout << std::endl << "Button " << i << "pressed" << std::endl;
				_pressed_queue.push(i);
			}
		}

	NEXT_POLL:
		std::cout.flush(); // print all cout buffer
		delete[] fdset;
		std::this_thread::sleep_for(std::chrono::seconds(1) - timeout);
	}
}