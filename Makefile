build:
	g++ utils.cpp hist.cpp mask.cpp main.cpp -o main.out -I /usr/include/opencv `pkg-config opencv --libs` -std=gnu++11 -O3 -lm

test:
	g++ utils.cpp hist.cpp mask.cpp main.cpp -o main.out -I /usr/include/opencv `pkg-config opencv --libs` -std=gnu++11 -Wall -Wextra -O3 -lm
	./main.out < input

debug:
	g++ utils.cpp hist.cpp mask.cpp main.cpp -o main.out -I /usr/include/opencv `pkg-config opencv --libs` -std=gnu++11 -Wall -Wextra -O3 -lm -g
	valgrind --track-origins=yes ./main.out < input

run:
	./main.out < input

clean:
	rm *.out