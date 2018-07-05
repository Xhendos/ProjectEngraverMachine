Imageprocessor.o: ./src/Processing/Imageprocessor.cpp ./src/Processing/Imageprocessor.hpp
	g++ -c ./src/Processing/Imageprocessor.cpp -lsfml-graphics

Gpio.o: ./src/Gpio/Gpio.cpp ./src/Gpio/Gpio.hpp
	g++ -c ./src/Gpio/Gpio.cpp

Camera.o: ./src/Capture/Camera.cpp ./src/Capture/Camera.hpp
	g++ -c ./src/Capture/Camera.cpp

main.o: ./src/main.cpp
	g++ -c ./src/main.cpp

Plotter.o: ./src/Plotter/plotter.cpp ./src/Plotter/plotter.hpp
	g++ -c ./src/Plotter/plotter.cpp

program: Imageprocessor.o Gpio.o Camera.o main.o Plotter.o
	g++ -o program Imageprocessor.o Gpio.o Camera.o Plotter.o main.o -lsfml-graphics
