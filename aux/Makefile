OPENCV_DIR = /usr/include/opencv
OBJS       = ccl.o
CC         = g++
DEBUG      = -g
CFLAGS     = -c -I $(OPENCV_DIR)
LFLAGS     = `pkg-config opencv --libs`

all: $(OBJS)
	$(CC) $(OBJS) $(LFLAGS) -o ccl

debug: $(OBJS)
	$(CC) $(DEBUG) $(OBJS) $(LFLAGS) -o ccl

ccl.o: ccl.cc
	$(CC) $(DEBUG) $(CFLAGS) ccl.cc

clean:
	rm *.o ccl
