#include <iostream>
#include <cstdio>
#include <cstring>
#include <stdlib.h>
#include <unistd.h> //for lseek
#include "GPIO.h"

int main()
{
	int button1 = 4;	//gpiopin for play button
	int button2 = 27;	//gpiopin for next button

	GPIO play, next;
	play.init(button1, INPUT, RISING);
	next.init(button2, INPUT, RISING);
	
	ButtonPoll buttonpoll;
	buttonpoll.add(&play);
	buttonpoll.add(&next);
	
	buttonpoll.polling();

	play.fdClose();
	next.fdClose();

	if(play.is_pressed){
		//do something;
		play.is_pressed = false;
	}

	
	if(next.is_pressed){
		//do something;
		next.is_pressed = false;
	}
	
	return 0;
}
