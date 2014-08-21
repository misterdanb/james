main: main.cpp
	g++ -O2 -ggdb -std=c++11 -ljames `pkg-config --cflags gtk+-3.0` -o james-gtk3 main.cpp `pkg-config --libs gtk+-3.0`

clean:
	rm -f *.o *~ gtk3-james
