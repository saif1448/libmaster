# CXX = g++
# CXXFLAGS = -std=c++11 -Wall -Wextra -O2
# LDFLAGS = -L. -lFilmMaster2000

# SRCS = Video.cpp main.cpp
# OBJS = $(SRCS:.cpp=.o)

# all: libFilmMaster2000.a runme

# libFilmMaster2000.a: Video.o
# 	ar rcs libFilmMaster2000.a Video.o

# Video.o: Video.cpp Video.h
# 	$(CXX) $(CXXFLAGS) -c Video.cpp -o Video.o

# main.o: main.cpp Video.h
# 	$(CXX) $(CXXFLAGS) -c main.cpp -o main.o

# runme: main.o libFilmMaster2000.a
# 	$(CXX) $(CXXFLAGS) main.o -o runme $(LDFLAGS)

# test: runme
# 	./runme input.bin output.bin -S reverse
# 	./runme input.bin output.bin -S swap_channel 1,2
# 	./runme input.bin output.bin -S clip_channel 1 10,200
# 	./runme input.bin output.bin -S scale_channel 1 1.5

# clean:
# 	rm -f *.o libFilmMaster2000.a runme


CXX = g++
CXXFLAGS = -std=c++11 -Wall -Wextra -O2
LDFLAGS = -L. -lFilmMaster2000

# Add OpenMP flag for multi-threaded functions
OPENMP_FLAG = -fopenmp

SRCS = Video.cpp main.cpp
OBJS = $(SRCS:.cpp=.o)

all: libFilmMaster2000.a runme

libFilmMaster2000.a: Video.o
	ar rcs libFilmMaster2000.a Video.o

# Compile Video.cpp with OpenMP support
Video.o: Video.cpp Video.h
	$(CXX) $(CXXFLAGS) $(OPENMP_FLAG) -c Video.cpp -o Video.o

main.o: main.cpp Video.h
	$(CXX) $(CXXFLAGS) -c main.cpp -o main.o

# Link with OpenMP support
runme: main.o libFilmMaster2000.a
	$(CXX) $(CXXFLAGS) $(OPENMP_FLAG) main.o -o runme $(LDFLAGS)

test: runme
	./runme input.bin output.bin -S reverse
	./runme input.bin output.bin -S swap_channel 1,2
	./runme input.bin output.bin -S clip_channel 1 10,200
	./runme input.bin output.bin -S scale_channel 1 1.5

clean:
	rm -f *.o libFilmMaster2000.a runme