#ifndef BUTTONPOLL_H
#define BUTTONPOLL_H

#include <vector>
#include <queue>

#include "button.h"

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
		std::vector<Button> _buttons;
		std::queue<int> _pressed_queue;
};

#endif // BUTTONPOLL_H