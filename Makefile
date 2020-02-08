all:
	g++ -Wall -O3 *.cpp -o Game -lsfml-graphics -lsfml-window -lsfml-system -lsfml-network -lpthread
clean:
	rm -rf *o Game 