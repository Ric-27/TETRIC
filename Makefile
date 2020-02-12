all: compile
	./TETRIC.out
compile: clean
	g++ -Wall -O3 *.cpp -o TETRIC.out -lsfml-graphics -lsfml-window -lsfml-system -lsfml-network -lpthread
clean:
	rm -rf *o TETRIC 