/*
 * Created by vr61v on 04.05.2023.
*/

#ifndef PARSER_PARSER_H
#define PARSER_PARSER_H
#include "iostream"
#include "fstream"
#include <cstring>
#include "map"

/*
 * name - list of vertex names
 * matrix - matrix of values
 */
struct adjacent{
    std::vector<std::string> names;
    std::vector<std::vector<int>> matrix;
};

/*
 * name - map of vertex names and indices
 * list - matrix of values
 * edges - struct with start, end vertices and weight
 */
struct edgesList{
    struct edge{
        std::string start, end;
        int weight;
    };
    std::map<std::string, int> names;
    std::vector<edge> list;
};

/*
 * exception class for parser
 */
class parserException : public std::exception{
private:
    char* massage;
public:
    explicit parserException(char* massage);
    const char * what() const noexcept override;
};

/*
 * function take file.txt path and return adjacent with names & matrix
 */
bool isDigit(const std::string& number);
adjacent parserTxtAdjacentMatrix(const char* filePath);

#endif //PARSER_PARSER_H
