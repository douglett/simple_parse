SRC = main.cpp
HEAD = func.hpp stmt.hpp inputfile.hpp
OUT = main.out

$(OUT): $(SRC) $(HEAD)
	g++ -Wall -std=c++17 -o $(OUT) $(SRC)

run: $(OUT)
	./$(OUT)