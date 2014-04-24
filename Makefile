CC= g++
CFLAGS=-I. -I/usr/local/cuda/include -L/usr/local/cuda/lib -lcuda -lcudart
DEPS = Point.h Primitive.h Vector.h Triangle.h Scene.h Ray.h Material.h Image.h Plane.h Light.h Spehere.h cudaCode. h
OBJ = cudaCode.o Point.o Primitive.o Vector.o  Triangle.o Scene.o Ray.o Material.o Image.o Light.o Plane.o Sphere.o  main.o 

main: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

cudaCode.o: cudaCode.cu
	nvcc -c cudaCode.cu -o cudaCode.o 


