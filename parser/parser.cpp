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

    int lengthV = 0; std::string line;
    getline(file, line);
    if(line.data() == nullptr){ throw parserException("File is empty"); }
    if(!isDigit(line)){ throw parserException("Invalid value: Number of vertices is not a digit"); }
    lengthV = std::atoi(line.c_str());

    adjacent adjacentMatrix;
    adjacentMatrix.matrix.resize(lengthV, std::vector<int>(lengthV, 0));

    std::string names;
    getline(file, names);
    char* namesLine = std::strtok(names.data(), SEPARATORS);
    while(namesLine != nullptr){
        adjacentMatrix.names.emplace_back(namesLine);
        namesLine = strtok(nullptr, SEPARATORS);
    }
    free(namesLine);
    if(adjacentMatrix.names.size() != lengthV){
        throw parserException("Invalid value: The number of vertices does not match the specified");
    }

    int x = 0;
    while(!file.eof()){
        int y = 0; std::string values; getline(file, values);
        char* valuesLine = std::strtok(values.data(), SEPARATORS);
        while(valuesLine != nullptr){
            if(!isDigit(valuesLine)){ throw parserException("Invalid value: Value is not a digit"); }
            adjacentMatrix.matrix[x][y++] = std::atoi(valuesLine);
            valuesLine = strtok(nullptr, SEPARATORS);
        }
        if(y > lengthV){
            throw parserException("Invalid value: Number of values greater than specified");
        }
        ++x;
    }

    file.close();
    return adjacentMatrix;
}
edgesIncident parserTxtIncidentMatrix(const char* filePath){
    std::ifstream file(filePath);
    if(!file.is_open()){ throw parserException("File not found"); }

    int lengthV = 0, lengthE = 0;
    std::string line; getline(file, line);
    if(line.data() == nullptr){ throw parserException("File is empty"); }
    else{
        char* firstLine = std::strtok(line.data(), SEPARATORS);

        if(!isDigit(firstLine)){ throw parserException("Invalid value: Number of vertices is not a digit"); }
        lengthV = std::atoi(firstLine);
        firstLine = strtok(nullptr, SEPARATORS);

        if(firstLine == nullptr){ throw parserException("Invalid value: You must specify the number of vertices and edges"); }
        if(!isDigit(firstLine)){ throw parserException("Invalid value: Number of edges is not a digit"); }
        lengthE = std::atoi(firstLine);
    }

    edgesIncident edgesList;
    std::string edges; getline(file, edges);
    std::vector<std::string> edgesName;
    char* edgesLine = std::strtok(edges.data(), SEPARATORS);
    while(edgesLine != nullptr){
        if(!edgesList.list.contains(edgesLine)){
            edgesList.list[edgesLine];
            edgesName.emplace_back(edgesLine);
        }
        else{ throw parserException("Invalid value: there are edges with the same name"); }
        edgesLine = strtok(nullptr, SEPARATORS);
    }
    if(edgesName.size() > lengthE){ throw parserException("Invalid value: The number of edges does not match the given one"); }

    int x = 0;
    while(!file.eof()){
        int y = 0; std::string values; getline(file, values);
        char* valuesLine = std::strtok(values.data(), SEPARATORS);
        if(!valuesLine){
            throw parserException("Invalid value: The number of vertices does not match the specified");
        }
        edgesList.names.emplace_back(valuesLine);
        valuesLine = strtok(nullptr, SEPARATORS);

        while(valuesLine != nullptr){
            if(!isDigit(valuesLine)){ throw parserException("Invalid value: Value is not a digit"); }
            int number = std::atoi(valuesLine);
            if(number != 0){
                auto iterator = edgesList.list.find(edgesName[y]);
                if(iterator->second.start == -1){
                    iterator->second.start = x;
                    iterator->second.weight = number;
                }else if(iterator->second.end == -1){
                    iterator->second.end = x;
                    if(iterator->second.weight > 0 && number > 0 && iterator->second.weight != number ||
                            iterator->second.weight > 0 && number < 0 && iterator->second.weight != number * -1 ||
                            iterator->second.weight < 0 && number > 0 && iterator->second.weight * -1 != number){
                        throw parserException("Invalid value: The values in the columns must be equal");
                    }
                    iterator->second.weight = number;
                }else{
                    throw parserException("Invalid value: An edge can only connect 2 vertices");
                }
            }
            ++y;
            if(y > lengthE){
                throw parserException("Invalid value: The number of edges does not match the specified");
            }
            valuesLine = strtok(nullptr, SEPARATORS);
        }
        ++x;
        if(x == lengthV){
            break;
        }
    }

    file.close();
    return edgesList;
}
edges parserTxtEdgesList(const char* filePath){
    std::ifstream file(filePath);
    if(!file.is_open()){ throw parserException("File not found"); }

    int lengthE = 0; std::string line;
    getline(file, line);
    if(line.data() == nullptr){ throw parserException("File is empty"); }
    if(!isDigit(line)){ throw parserException("Invalid value: Number of vertices is not a digit"); }
    lengthE = std::atoi(line.c_str());

    edges edgesList;

    std::string names; int it = 0;
    getline(file, names);
    char* namesLine = std::strtok(names.data(), SEPARATORS);
    while(namesLine != nullptr){
        if(!edgesList.names.contains(namesLine)){ edgesList.names[namesLine] = it++; }
        else{ throw parserException("Invalid value: there are vertices with the same name"); }
        namesLine = strtok(nullptr, SEPARATORS);
    }
    free(namesLine);

    int x = 0;
    while(!file.eof()){
        if(x == lengthE){ break; }
        int y = 0; std::string values; getline(file, values);
        char* valuesLine = std::strtok(values.data(), SEPARATORS);
        std::string start, end; int weight;
        while(valuesLine != nullptr){
            if(y > 2){ throw parserException("Invalid value: Incorrect edge"); }
            if(y == 0){
                if(!edgesList.names.contains(valuesLine)){ throw parserException("Invalid value: Vertex not found"); }
                start = valuesLine;
            }
            else if(y == 1){
                if(!edgesList.names.contains(valuesLine)){ throw parserException("Invalid value: Vertex not found"); }
                end = valuesLine;
            }
            else if(y == 2){
                if(!isDigit(valuesLine)){ throw parserException("Invalid value: Value is not a digit"); }
                weight = std::atoi(valuesLine);
            }
            valuesLine = strtok(nullptr, SEPARATORS);
            ++y;
        }
        edgesList.list.push_back({start, end, weight});
        ++x;
        if(x > lengthE){ throw parserException("Invalid value: The number of edges differs from the given one"); }
    }
    if(x != lengthE){ throw parserException("Invalid value: The number of edges differs from the given one"); }

    file.close();
    return edgesList;
}