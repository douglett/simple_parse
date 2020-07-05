SRC = main.cpp
HEAD = inputfile.hpp globals.hpp \
		func.hpp stmt.hpp expr.hpp
OUT = main.out

$(OUT): $(SRC) $(HEAD)
	g++ -Wall -std=c++17 -o $(OUT) $(SRC)

run: $(OUT)
	./$(OUT)