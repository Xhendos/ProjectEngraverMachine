Imageprocessor.o: ./src/Processing/Imageprocessor.cpp ./src/Processing/Imageprocessor.hpp
	g++ -c ./src/Processing/Imageprocessor.cpp 

Gpio.o: ./src/Gpio/Gpio.cpp ./src/Gpio/Gpio.hpp
	g++ -c ./src/Gpio/Gpio.cpp

Camera.o: ./src/Capture/Camera.cpp ./src/Capture/Camera.hpp
	g++ -c ./src/Capture/Camera.cpp

main.o: ./src/main.cpp
	g++ -c ./src/main.cpp

program: Imageprocessor.o Gpio.o Camera.o main.o
	g++ -o program Imageprocessor.o Gpio.o Camera.o main.o -I /home/xhendos/Downloads/SFML-2.4.2/include -lsfml-graphics
