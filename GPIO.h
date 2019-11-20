#define SYSFS_GPIO_DIR "/sys/class/gpio"
#define MAX_BUF 64
#include <vector>
#include <poll.h>

enum pinDirection{
		INPUT = 0,
		OUTPUT = 1
	};

enum pinValue{
	LOW = 0,
	HIGH = 1
};

enum edge{
	RISING = 0,
	FALLING = 1,
	BOTH = 2
};

class GPIO{
	public:
		bool is_pressed = false;
		int fd;

		//Constructor
		GPIO();

		//Methods
		int init(int pin, pinDirection direction, edge edge);
		void pinExport();
		void setDir(pinDirection direction);
		void setValue(pinValue value);
		void setEdge(edge edge);
		int fdOpen();
		int fdClose();

	private:
		int _gpioPin;
		pinDirection _dir;
		edge _edge;
};

class ButtonPoll{
	public:
		std::vector <GPIO*>gpio_list;
		struct pollfd* fdset= NULL;
		
		//Constructor
		ButtonPoll();

		//Methods
		void add(GPIO* button);
		void polling();
};