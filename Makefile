RESOLUTION=500
OMP=0

# Additional flags to use when compiling, e.g.
# make CCFLAGS=-DOUTPUT PROG=exercise2
CCFLAGS=-DOMP

all: serial omp omp_to_image serial_to_img preview

serial:
	/opt/homebrew/opt/gcc/bin/c++-12 -L/opt/homebrew/opt/libjpeg/lib -ljpeg main.cpp $(CCFLAGS) -o main.out
omp:
	/opt/homebrew/opt/gcc/bin/c++-12 -fopenmp -L/opt/homebrew/opt/libjpeg/lib -ljpeg -Wall main.cpp $(CCFLAGS) -o main.out

omp_to_img: omp
	./main.out $(RESOLUTION)

serial_to_img: serial
	./main.out $(RESOLUTION)

preview:
	imgcat image.jpeg

clean:
	rm -rf *.out
	rm image.*
