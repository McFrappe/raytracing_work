all: pipe_to_img preview

build:
	g++ -std=c++11 main.cpp -o main.out

pipe_to_img: build
	./main.out > image.ppm
	convert image.ppm image.jpeg
	rm image.ppm

preview:
	imgcat image.jpeg

clean:
	rm -rf *.out
