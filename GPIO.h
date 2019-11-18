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
	bool is_pressed = false;
	int fd;

public:
	GPIO(int pin);
	GPIO(int pin,pinDirection dir);
	
	void pinExport();
	void setDir(pinDirection dir);
	void setValue(pinValue val);
	void setEdge(char *edge);
	int fdOpen();
	int fdClose();

private:
	int _gpioPin;
	pinDirection _dir;
	
};