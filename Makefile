OS := $(shell uname)
RESOLUTION=500
OMP=0

# Additional flags to use when compiling, e.g.
# make CCFLAGS=-DOUTPUT PROG=exercise2
CCFLAGS=-DOMP

all: serial omp omp_to_image serial_to_img preview

serial:
ifneq ($(OS), Darwin)
	c++ main.cpp $(CCFLAGS) -o main.out -ljpeg
else
	c++-12 -fopenmp main.cpp $(CCFLAGS) -o main.out -ljpeg
endif

omp:
ifneq ($(OS), Darwin)
	c++ -fopenmp main.cpp $(CCFLAGS) -o main.out -ljpeg
else
	c++-12 -fopenmp main.cpp $(CCFLAGS) -o main.out -ljpeg
endif

omp_to_img: omp
	./main.out $(RESOLUTION)

serial_to_img: serial
	./main.out $(RESOLUTION)

preview:
	imgcat image.jpeg

clean:
	rm -rf *.out
	rm image.*
