CC = g++
LIBS = -lglfw -lGL -lGLU -lglut -lm -lX11 -ldl -lpthread
CFLAGS = -g -Wall -Wno-deprecated -DGLFW_INCLUDE_NONE -I../../libs -I../../libs/glad/include
GLAD_SRC = ../../libs/glad/src/glad.c

main: main.o
	$(CC) $(CFLAGS) $< $(GLAD_SRC) -o $@ $(LIBS)

%.o: %.cpp
	$(CC) $(CFLAGS) -c $<

clean:
	-rm -f *.o
