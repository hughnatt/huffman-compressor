SRC=../src
HEAD=../include

OPTIONS = -g -Wall

exec: test_analyse test_huffman test_mtf


test_analyse: test_analyse.o analyse.o
	gcc -o $@ test_analyse.o analyse.o

test_analyse.o: test_analyse.c
	gcc -c $< -I $(HEAD)/ $(OPTIONS)

analyse.o: $(SRC)/analyse.c
	gcc -c $< -I $(HEAD)/ $(OPTIONS)


test_huffman: test_huffman.o huffman.o
	gcc -o $@ test_huffman.o huffman.o

test_huffman.o: test_huffman.c
	gcc -c $< -I $(HEAD)/ $(OPTIONS)

huffman.o: $(SRC)/huffman.c
	gcc -c $< -I $(HEAD)/ $(OPTIONS)


test_mtf: test_mtf.o mtf.o
	gcc -o $@ test_mtf.o mtf.o

test_mtf.o: test_mtf.c
	gcc -c $< -I $(HEAD)/ $(OPTIONS)

mtf.o: $(SRC)/mtf.c
	gcc -c $< -I $(HEAD)/ $(OPTIONS)



clean:
	rm -rf *.o test_analyse test_huffman test_mtf