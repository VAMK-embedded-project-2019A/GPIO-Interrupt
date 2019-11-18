
all: main.o GPIO.o
	g++ main.o GPIO.o -o build
	rm *.o -f
	
main: main.cpp
	g++ main.cpp -c

GPIO: GPIO.o
	g++ GPIO.cpp -c

clean:
	rm *.o -f