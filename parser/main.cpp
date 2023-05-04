#include <iostream>
#include "parser.h"

int main() {
    edges matrix;
    try{
        matrix = parserTxtIncidentMatrix("/Users/vr61v/Desktop/Graphs/Graphs/parser/test.txt");
    }catch(const parserException& exc){
        std::cout << exc.what();
    }

    return 0;
}
