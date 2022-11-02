RESOLUTION=500

all: pipe_to_img preview

build:
	g++ -std=c++11 main.cpp -o main.out

build_mac:
	/opt/homebrew/opt/gcc/bin/c++-12 -fopenmp -Wall main.cpp -o main.out

pipe_to_img: build_mac
	./main.out $(RESOLUTION) > image.ppm
	convert image.ppm image.jpeg
	rm image.ppm

preview:
	imgcat image.jpeg

clean:
	rm -rf *.out
