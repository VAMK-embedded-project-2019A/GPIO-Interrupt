#include "buttonpoll.h"

int main()
{
	int pin_1 = 4;	// gpiopin for play button
	int pin_2 = 27;	// gpiopin for next button
	
	ButtonPoll button_poll;
	button_poll.addButton(pin_1, ButtonPoll::TriggerEdge::Rising);
	button_poll.addButton(pin_2, ButtonPoll::TriggerEdge::Rising);
	
	button_poll.start();
	
	return 0;
}
