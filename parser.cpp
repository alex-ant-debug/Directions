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

vector<string> splitRow(char *rowPtr, char separator)
{
    vector<string> rowData;
    char *pch = strtok(rowPtr, &separator);//" "

    while (pch != NULL)                         // пока есть лексемы
      {
          rowData.push_back(pch);
          pch = strtok(NULL, &separator);//" "
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

char automaticSeparatorDetection(void)
{
    bool firstTime = true;
    unsigned int separatorsCount = sizeof(separators)/sizeof(separators[0]);
    vector<bool> isTable; // это атрибут класса статический
    vector<unsigned int> initialSeparatorValues;
    string line;

    for(unsigned int i = 0; i < separatorsCount; i++)
    {
        isTable.push_back(true);
    }

    ifstream in(this->fileName);
    if (in.is_open())
    {
            while (getline(in, line))
            {
                vector<unsigned int> lineSeparators;
                for(unsigned int i = 0; i < separatorsCount; i++)
                {
                    if(isTable[i] != false)
                    {
                        lineSeparators.push_back(countNumberOfDelimiters(line, this->separators[i]));
                    }
                    else
                    {
                        lineSeparators.push_back(false);
                    }
                }
                line.clear();

                if(firstTime == true)
                {
                    initialSeparatorValues = lineSeparators;
                    lineSeparators.clear();
                    firstTime = false;
                }
                else
                {
                    for(unsigned int j = 0; j < separatorsCount; j++)
                    {
                      if(isTable[j] != false)
                      {
                          if(initialSeparatorValues[j] != lineSeparators[j])
                          {
                            isTable[j] = false;
                          }
                          if((initialSeparatorValues[j] == 0)&&(lineSeparators[j] == 0))
                          {
                            isTable[j] = false;
                          }
                      }
                    }
                    lineSeparators.clear();
                }
            }
        // закрываем файл
        in.close();
    }
    else
    {
        throw 99;//"File not found";
    }

    for(unsigned int i = 0; i < separatorsCount; i++)
    {
        if(isTable[i] != false)
        {
            return this->separators[i];
        }
    }
    return 0;
}

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
        unsigned int quantity = countNumberOfDelimiters(lineTable, separator);
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
            splitedRow = splitRow(cstr, this->rowSeparator);
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

char rowSeparator;

protected:
string fileName;
vector<vector<string>> parsedData;
const int separators[4] = {',', '.', ' ', ';'};

};
