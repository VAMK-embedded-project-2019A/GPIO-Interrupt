#define SYSFS_GPIO_DIR "/sys/class/gpio"
#define MAX_BUF 64

enum pinDirection{
	INPUT = 0,
	OUTPUT = 1
};

enum pinValue{
	LOW = 0,
	HIGH = 1
};

class GPIO{

	
public:
	GPIO(int pin);
	int setDir(pinDirection dir);
	int setValue(pinValue val);
	int getValue();
	int setEdge(char *edge);
	int fdOpen();
	int fdClose(int fd);
	int pinExport();


private:
	int _gpioPin;
};