all:
	g++ *.cpp -o Game -lsfml-graphics -lsfml-window -lsfml-system
clean:
	rm -rf *o Game