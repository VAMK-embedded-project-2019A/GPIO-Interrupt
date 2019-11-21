#ifndef BUTTON_H
#define BUTTON_H

#include <string>

class Button
{
	public:
		friend class ButtonPoll;
		Button() = delete;

	private:
		Button(int pin, int edge);
		bool pinExport();
		bool setDirection();
		bool setEdge(int edge);
		
		const std::string SYSFS_GPIO_DIR{"/sys/class/gpio"};
		int _gpio_pin;
		int _fd;
};

#endif // BUTTON_H