RESOLUTION=500
OMP=0

# Additional flags to use when compiling, e.g.
# make CCFLAGS=-DOUTPUT PROG=exercise2
CCFLAGS=-DOMP

all: serial omp omp_to_image serial_to_img preview

serial:
	g++ -std=c++11 -ljpeg main.cpp -o main.out

omp:
	g++ -fopenmp -Wall -ljpeg main.cpp $(CCFLAGS) -o main.out

omp_to_img: omp
	./main.out $(RESOLUTION)

serial_to_img: serial
	./main.out $(RESOLUTION) > image.ppm
	convert image.ppm image.jpeg
	rm image.ppm

preview:
	imgcat image.jpeg

clean:
	rm -rf *.out
	rm image.*
