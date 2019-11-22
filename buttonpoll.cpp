#include "buttonpoll.h"

#include <iostream>
#include <chrono>
#include <thread>

#include <unistd.h>	// lseek()
#include <poll.h>	// poll()

ButtonPoll::~ButtonPoll()
{
	for(auto &button : _buttons)
		close(button._fd);
}

void ButtonPoll::addButton(int pin, TriggerEdge edge)
{
	_buttons.push_back(Button(pin, static_cast<int>(edge)));
}

bool ButtonPoll::isButtonPressed()
{
	return !_pressed_queue.empty();
}

int ButtonPoll::getNextPressedPin()
{
	if(_pressed_queue.empty())
		return -1;

	int pin = _buttons.at(_pressed_queue.front())._gpio_pin;
	_pressed_queue.pop();
	return pin;
}

void ButtonPoll::start()
{
	const int BUTTON_COUNT = _buttons.size();
	std::vector<bool> first_interrupt(BUTTON_COUNT, true);
	std::chrono::milliseconds timeout{700};

	while(true)
	{
		struct pollfd *fdset = new pollfd[BUTTON_COUNT];

		for(int i=0; i<BUTTON_COUNT; i++)
		{
			fdset[i].fd = _buttons[i]._fd;
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