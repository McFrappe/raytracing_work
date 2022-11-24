RESOLUTION=500
OMP=0

# Additional flags to use when compiling, e.g.
# make CCFLAGS=-DOUTPUT PROG=exercise2
CCFLAGS=-DOMP

all: pipe_to_img preview

build:
ifeq ($(OMP),1)
	/opt/homebrew/opt/gcc/bin/c++-12 -fopenmp -Wall main.cpp $(CCFLAGS) -o main.out
else
	g++ -std=c++11 main.cpp -o main.out
endif

pipe_to_img: build
	./main.out $(RESOLUTION) > image.ppm
	convert image.ppm image.jpeg
	rm image.ppm

preview:
	imgcat image.jpeg

clean:
	rm -rf *.out
	rm image.*
