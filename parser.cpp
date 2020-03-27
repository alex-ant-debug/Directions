#include "parser.h"
#include <iostream>
#include <cstring>
#include <vector>
#include <fstream>

using namespace std;

class Parser
{
    public:

    Parser()
    {}

    Parser(string file, char symbol = ' ')
    {
        this->fileName = file;
        this->rowSeparator = symbol;
    }

    virtual vector<string> splitRow(string line, char separator)
    {
        char *rowPtr = &line[0u];
        vector<string> rowData;
        char *pch = strtok(rowPtr, &separator);

        while (pch != NULL)
          {
              rowData.push_back(pch);
              pch = strtok(NULL, &separator);
          }
        return rowData;
    }

    void parseFile(void)
    {
        string line;

        ifstream in(this->fileName);
        if (in.is_open())
        {
            while (getline(in, line))
            {
                vector<string> splitedRow;
                splitedRow = splitRow(line, this->rowSeparator);
                this->parsedData.push_back(splitedRow);
                line.clear();
            }

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

    void printParserData(void)
    {
        for(unsigned int i = 0; i< parsedData.size(); i++, cout<<endl)
            for(unsigned int j = 0; j< parsedData[i].size(); j++)
            {
                cout<<parsedData[i][j]<<"\t";
            }
    }

    char rowSeparator;

    protected:
    string fileName;
    vector<vector<string>> parsedData;

};
