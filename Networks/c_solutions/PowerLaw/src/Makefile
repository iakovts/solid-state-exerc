OUT=PL_TSOUROS

build:
	@g++ -g -std=c++17 -fsanitize=address -fopenmp  -Wall -O2 -o $(OUT) PowerLaw.cpp

run: build
	@for gamma in -3 -2.5 -2; do \
		./$(OUT) -g $$gamma; \
		done

clean:
	@rm -f PowerLaw*.txt
	@rm -f $(OUT)
