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

Parser(string file, char symbol = ' ')
{
    this->fileName = file;
    this->rowSeparator = symbol;
}

vector<string> splitRow(char *rowPtr)
{
    vector<string> rowData;
    char *pch = strtok(rowPtr, &rowSeparator);//" "

    while (pch != NULL)                         // пока есть лексемы
      {
          rowData.push_back(pch);
          pch = strtok(NULL, &rowSeparator);//" "
      }
    return rowData;
}

unsigned int countNumberOfDelimiters(string lineTable, char separator)
{//найти стандартную функцию подсчета разделителей в строке
    unsigned int quantity = 0;
    size_t found = 0;

    found = lineTable.find_first_of(separator,found);

    while(found != string::npos)
    {
        quantity++;
        found = lineTable.find_first_of(separator,found+1);
    }

    return quantity;
}

//automaticSeparatorDetection(void)
//int separators[3] = {',', '.', ' '};

bool detectIsTable(char separator)
{
    ifstream fileTable(this->fileName);
    string lineTable;
    bool isTable = true;

    unsigned int initialValue = 0;

    if(!getline(fileTable, lineTable))
    {
        throw 98;
    }

    initialValue = countNumberOfDelimiters(lineTable, separator);

    if(initialValue == 0)
    {
        return false;
    }

    while (getline(fileTable, lineTable))
    {
        int quantity = countNumberOfDelimiters(lineTable, separator);
        if(quantity != initialValue)
        {
            isTable = false;
            break;
        }
    }

    return isTable;
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
char rowSeparator;
vector<vector<string>> parsedData;

};
