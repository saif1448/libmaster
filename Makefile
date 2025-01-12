# all: Main.cpp Frame.cpp Frame.h
# 	g++ -o runme Main.cpp Frame.cpp Video.cpp

# clean:
# 	rm -f runme

# all: runme

# runme: Main.o VideoN.o
# 	g++ -o runme Main.o VideoN.o

# Main.o: Main.cpp VideoN.h
# 	g++ -c Main.cpp

# VideoN.o: VideoN.cpp VideoN.h
# 	g++ -c VideoN.cpp

# clean:
# 	rm -f runme Main.o VideoN.o


# Makefile

# Target executable
all: runme

# Link object files to create the executable
runme: Main.o Video.o
	g++ -o runme Main.o Video.o

# Compile Main.cpp into Main.o
Main.o: Main.cpp Video.h
	g++ -c Main.cpp

# Compile Video.cpp into Video.o
Video.o: Video.cpp Video.h
	g++ -c Video.cpp

# Clean up build files
clean:
	rm -f runme Main.o Video.o