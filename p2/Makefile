all: p2
p2: pwzip.o wunzip.o
	g++ -o pwzip pwzip.o -Wall -Werror -lpthread -O
	g++ -o wunzip wunzip.o -Wall -Werror -lpthread -O
pwzip.o: pwzip.cpp
	g++ -c pwzip.cpp 
wunzip.o: wunzip.cpp
	g++ -c wunzip.cpp
clean:
	rm -rf *.o p2