/*
 * Created by vr61v on 04.05.2023.
*/
#include "parser.h"
#include <utility>

// Constants
const char* SEPARATORS = " !@#$%^&*()[]{}|/?<>,.+=\n\t\r\\\0";

// exception class
parserException::parserException(char* massage){ this->massage = massage; }
const char* parserException::what() const noexcept{ return this->massage; }

// all parsers
bool isDigit(const std::string& number){
    return number.find_first_not_of("0123456789-") == std::string::npos;
}
adjacent parserTxtAdjacentMatrix(const char* filePath){
    std::ifstream file(filePath);

    if(!file.is_open()){ throw parserException("File not found"); }

    int length; std::string line;
    getline(file, line);
    if(line.data() == nullptr){ throw parserException("File is empty"); }
    if(!isDigit(line)){ throw parserException("Invalid value: Number of vertices is not a digit"); }
    length = std::atoi(line.c_str());

    adjacent adjacentMatrix;
    adjacentMatrix.matrix.resize(length, std::vector<int>(length, 0));

    int it = 0; std::string names;
    getline(file, names);
    char* namesLine = std::strtok(names.data(), SEPARATORS);
    while(namesLine != nullptr){
        adjacentMatrix.names.emplace_back(namesLine);
        namesLine = strtok(nullptr, SEPARATORS);
    }
    free(namesLine);
    if(adjacentMatrix.names.size() != length){
        throw parserException("Invalid value: The number of vertices does not match the specified");
    }

    int x = 0;
    while(!file.eof()){
        int y = 0; std::string values;
        getline(file, values);
        char* valuesLine = std::strtok(values.data(), SEPARATORS);
        while(valuesLine != nullptr){
            if(!isDigit(valuesLine)){ throw parserException("Invalid value: Value is not a digit"); }
            adjacentMatrix.matrix[x][y++] = std::atoi(valuesLine);
            valuesLine = strtok(nullptr, SEPARATORS);
        }
        if(y > length){
            throw parserException("Invalid value: Number of values greater than specified");
        }
        ++x;
    }

    file.close();
    return adjacentMatrix;
}
