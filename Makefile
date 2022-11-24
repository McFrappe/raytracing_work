RESOLUTION=500
OMP=0

# Additional flags to use when compiling, e.g.
# make CCFLAGS=-DOUTPUT PROG=exercise2
CCFLAGS=-DOMP

all: serial omp omp_to_image serial_to_img preview

serial:
	c++-12 -ljpeg main.cpp $(CCFLAGS) -o main.out
omp:
	c++-12 -fopenmp -ljpeg main.cpp $(CCFLAGS) -o main.out

omp_to_img: omp
	./main.out $(RESOLUTION)

serial_to_img: serial
	./main.out $(RESOLUTION)

preview:
	imgcat image.jpeg

clean:
	rm -rf *.out
	rm image.*
