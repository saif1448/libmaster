all: Main.cpp Frame.cpp Frame.h
	g++ -o runme Main.cpp Frame.cpp Video.cpp

clean:
	rm -f runme

