SRC=../src
HEAD=../include

test_analyse: test_analyse.o analyse.o
	gcc -o $@ test_analyse.o analyse.o

test_analyse.o: test_analyse.c
	gcc -c $< -I $(HEAD)/ -g

analyse.o: $(SRC)/analyse.c
	gcc -c $< -I $(HEAD)/ -g

clean:
	rm -rf *.o test_analyse