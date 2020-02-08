#include "parser.h"
#include <cstring>
#include <vector>
#include <fstream>

using namespace std;

class Parser
{
public:

Parser()
{}

Parser(string file)
{
    this->fileName = file;
}

vector<string> splitRow(char *rowPtr)
{
    vector<string> rowData;
    char *pch = strtok(rowPtr, " ");

    while (pch != NULL)                         // пока есть лексемы
      {
          rowData.push_back(pch);
          pch = strtok(NULL, " ");
      }
    return rowData;
}


void parseFile(void)
{
    string line;
    // окрываем файл для чтения
    ifstream in(this->fileName);
    if (in.is_open())
    {
        while (getline(in, line))
        {
            vector<string> splitedRow;
            //преобразование string в char
            char *cstr = &line[0u];
            splitedRow = splitRow(cstr);
            this->parsedData.push_back(splitedRow);
            line.clear();
        }
        // закрываем файл
        in.close();
    }
    else
    {
        throw 99;//"File not found";
    }
}

vector<vector<string>> getParsedData(void)
{
   return  parsedData;
}

protected:
string fileName;
vector<vector<string>> parsedData;

};
