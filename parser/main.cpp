#include <iostream>
#include "parser.h"

int main() {
    adjacent matrix;
    try{
        matrix = parserTxtAdjacentMatrix("/Users/vr61v/Desktop/Graphs/Graphs/parser/test2.txt");
    }catch(const parserException& exc){
        std::cout << exc.what();
    }

    return 0;
}
