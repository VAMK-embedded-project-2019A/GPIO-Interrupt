#ifndef GPIO_H
#define GPIO_H

#include <vector>
#include <string>
#include <queue>

class GPIO
{
	public:
		friend class ButtonPoll;

		GPIO() = delete;

	private:
		GPIO(int pin, int edge);
		bool pinExport();
		bool setDirection();
		bool setEdge(int edge);
		
		const std::string SYSFS_GPIO_DIR{"/sys/class/gpio"};
		int _gpio_pin;
		int _fd;
};

class ButtonPoll
{
	public:
		ButtonPoll() = default;
		~ButtonPoll();
		enum TriggerEdge : int { Rising, Falling, Both };

		void addButton(int pin, TriggerEdge edge);
		void start();
		bool isButtonPressed();
		int getNextPressedPin();

	private:
		std::vector<GPIO> _gpio_vec;
		std::queue<int> _pressed_queue;
};

#endif // GPIO_H