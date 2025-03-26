debug.out: *.cpp
	g++ main.cpp ./uart_class.cpp -Wall -ggdb -o debug.out
