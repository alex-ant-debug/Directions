#include "parser.cpp"
#include <iostream>
#include <cmath>

using namespace std;

class ParserCSV : public Parser
{
    public:

    ParserCSV(string file, char symbol = 'a')
    {
        this->fileName = file;
        this->rowSeparator = symbol == 'a'
                           ? this->automaticSeparatorDetection()
                           : symbol;
    }

    char automaticSeparatorDetection(void)
    {
        bool firstTime = true;
        unsigned int separatorsCount = sizeof(separators)/sizeof(separators[0]);
        vector<bool> isTable;
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
                              else if((initialSeparatorValues[j] == 0)&&(lineSeparators[j] == 0))
                              {
                                isTable[j] = false;
                              }
                          }
                        }
                        lineSeparators.clear();
                    }
                }
            in.close();
        }
        else
        {
            throw 99;           //"File not found";
        }

        for(unsigned int i = 0; i < separatorsCount; i++)
        {
            if(isTable[i] != false)
            {
                return this->separators[i];
            }
        }
        throw 97;
    }

    vector<vector<unsigned int>> getQuotedIntervals(string lineTable, char separator)
    {
        size_t found = 0;
        vector<vector<unsigned int>> intervals;

        found = lineTable.find_first_of('"',found+1);

        bool openFlag = true;
        int serchStartPosition = found+1;
        int openPosition = found;

        while(found != string::npos)
        {
            found = lineTable.find_first_of('"', serchStartPosition);

            if(found == string::npos)
            {
                break;
            }

            if(openFlag)
            {
                if(lineTable[found+1] == '"')
                {
                    serchStartPosition = found + 2;
                }
                else if(lineTable[found+1] == separator
                        ||found == lineTable.size()-1)
                {
                    vector<unsigned int> ranges;
                    openFlag = false;
                    serchStartPosition = found+1;
                    ranges.push_back(openPosition);
                    ranges.push_back(found);
                    intervals.push_back(ranges);
                }
                else
                {
                    throw 97;
                }
            }
            else
            {
                openFlag =true;
                serchStartPosition = found+1;
                openPosition = found;
            }
        }

        return intervals;
    }

    void prepareItemString(string *item)
    {
        size_t doubleQuotes = 0;
        while(doubleQuotes != string::npos)
        {
            doubleQuotes = item->find_first_of('"', doubleQuotes);
            if(doubleQuotes != string::npos)
            {
                item->replace(item->begin() + doubleQuotes, item->begin() + doubleQuotes+2, "\"");
                doubleQuotes++;
            }
        }
    }

    vector<string> splitRow(string lineTable, char separator)
    {
        vector<vector<unsigned int>> intervals = getQuotedIntervals(lineTable, separator);

        vector<string> rowData;
        size_t foundSeparator = 0;
        unsigned int inretvalsCount = intervals.size();
        size_t startPosition = 0;
        bool isInInterval = false;

        while(foundSeparator != string::npos)
        {
            foundSeparator = lineTable.find_first_of(separator,foundSeparator+1);

            size_t endPosition = (foundSeparator == string::npos)? lineTable.size(): foundSeparator;

            isInInterval = false;
            for(unsigned int i = 0; i < inretvalsCount; i++)
            {
                if((intervals[i][0] < foundSeparator)&&(intervals[i][1] > foundSeparator))
                {
                    isInInterval = true;
                }
            }

            if(!isInInterval)
            {
                if((lineTable[startPosition] == '"')&&(lineTable[endPosition-1] == '"'))
                {
                    startPosition++;
                    endPosition--;
                }

                string pushData = lineTable.substr(startPosition, endPosition-startPosition);
                prepareItemString(&pushData);//find double quotes
                rowData.push_back(pushData);
                startPosition = foundSeparator + 1;
            }
        }

        return rowData;
    }

    unsigned int countNumberOfDelimiters(string lineTable, char separator)
    {
        size_t found = 0;
        char quotesSimbol = '"';

        found = lineTable.find_first_of(quotesSimbol,found+1);
        if(found == string::npos)
        {
            return countNumberOfDelimitersSimple(lineTable, separator);
        }
        bool openFlag = true;
        int serchStartPosition = found+1;
        int openPosition = found;

        while(found != string::npos)
        {
            found = lineTable.find_first_of(quotesSimbol, serchStartPosition);

            if(found == string::npos)
            {
                break;
            }

            if(openFlag)
            {
                if(lineTable[found+1] == quotesSimbol)
                {
                    serchStartPosition = found + 2;
                }
                else if(lineTable[found+1] == separator
                        ||found == lineTable.size()-1)
                {
                    openFlag = false;
                    lineTable.replace(openPosition, found-openPosition+1, "");
                    serchStartPosition = openPosition;
                }
                else
                {
                    return 0;
                }
            }
            else
            {
                openFlag =true;
                serchStartPosition = found+1;
                openPosition = found;
            }
        }

        if(openFlag)
        {
            return 0;
        }
        return countNumberOfDelimitersSimple(lineTable, separator);
    }

    unsigned int countNumberOfDelimitersSimple(string lineTable, char separator)
    {
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

    protected:
    const int separators[4] = {',', '.', ' ', ';'};

};

