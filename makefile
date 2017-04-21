compile: main.cpp
	g++ -std=c++11 main.cpp -o exe -lpthread


all: compile
	./exe

clean: 
	rm exe

