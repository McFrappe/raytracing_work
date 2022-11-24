RESOLUTION=500
OMP=0

# Additional flags to use when compiling, e.g.
# make CCFLAGS=-DOUTPUT PROG=exercise2
CCFLAGS=-DOMP


serial_compile:
	c++ main.cpp $(CCFLAGS) -o main.out  -ljpeg
omp_compile:
	c++ -fopenmp -Wall main.cpp $(CCFLAGS) -o main.out -ljpeg 

omp_to_img: omp_compile
	./main.out $(RESOLUTION)

serial_to_img: serial_compile
	./main.out $(RESOLUTION)

preview:
	imgcat image.jpeg

clean:
	rm -rf *.out
	rm image.*
