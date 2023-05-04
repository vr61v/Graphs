/*
 * Created by vr61v on 04.05.2023.
*/
#include "parser.h"

const char* SEPARATORS = " ,.\n\t\r/\\\0";

adjacent parserTxtAdjacentMatrix(const char* filePath){
    std::ifstream file(filePath);

    int length = 0; std::string line;
    getline(file, line);
    length = std::atoi(line.c_str());

    adjacent adjacentMatrix;
    adjacentMatrix.names.resize(length);
    adjacentMatrix.matrix.resize(length, std::vector<int>(length, 0));

    int it = 0; std::string names;
    getline(file, names);
    char* namesLine = std::strtok(names.data(), SEPARATORS);
    while(namesLine != nullptr){
        adjacentMatrix.names[it++] = namesLine;
        namesLine = strtok(nullptr, SEPARATORS);
    }
    free(namesLine);

    int x = 0;
    while(!file.eof()){
        int y = 0; std::string values;
        getline(file, values);
        char* valuesLine = std::strtok(values.data(), SEPARATORS);
        while(valuesLine != nullptr){
            adjacentMatrix.matrix[x][y++] = std::atoi(valuesLine);
            valuesLine = strtok(nullptr, SEPARATORS);
        }
        ++x;
    }

    file.close();
    return adjacentMatrix;
}