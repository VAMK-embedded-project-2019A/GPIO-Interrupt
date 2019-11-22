all: main.o buttonpoll.o button.o
	g++ -Wall $^ -o build

%.o: %.c
	g++ -Wall -c $< -o $@

clean:
	rm *.o -f