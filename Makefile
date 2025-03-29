debug.out: *.cpp
	g++ main.cpp ./uart_class.cpp -Wall -Og -ggdb -o debug.out
