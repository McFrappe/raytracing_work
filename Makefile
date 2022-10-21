all: pipe_to_img preview

build:
	g++ -std=c++11 main.cpp -o main.out

pipe_to_img: build
	./main.out >> image.ppm

preview:
	imgcat image.ppm
