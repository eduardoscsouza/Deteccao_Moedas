all:
	g++ border.cpp -o border.out -I /usr/include/opencv `pkg-config opencv --libs` -std=gnu++11
clean:
	rm *~ border.out