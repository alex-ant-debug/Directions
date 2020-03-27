#include <QCoreApplication>
#include <cmath>
#include <cstring>
#include <vector>
#include "direction.cpp"
#include "parserCSV.cpp"

using namespace std;

int getLinesFromFile(string *arraysString, string file)
{
    int i = 0;
    ifstream in(file);
    if (in.is_open())
    {
        while (getline(in, *(arraysString+i)))
        {
           i++;
        }
    }
    in.close();
    return i;
}

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    try{
        string file = {"E:\\Qt project\\All Different Directions\\alldifferentdirections\\table.csv"};
        ParserCSV parserCSV(file);
        parserCSV.parseFile();
        parserCSV.printParserData();

        cout<<endl;

        file = {"E:\\Qt project\\All Different Directions\\alldifferentdirections\\sample.in"};
        Parser parser(file);
        parser.parseFile();

        vector<vector<string>> parsedData = parser.getParsedData();
        parser.printParserData();
        cout<<parserCSV.rowSeparator<<endl;
        if(!Direction::validateData(parsedData))
        {
            cout<<"Error data file"<<endl;
            return a.exec();
        }

        Direction::runTasks(parsedData);

    }

    catch(int error)
    {
        switch (error) {
        case 97: {cout<<"Error: wrong file format"<<endl; break;}
        case 98: {cout<<"Error: file is empty "<<error<<endl; break;}
        case 99: {cout<<"Error "<<error<<endl; break;}
        case 100:{cout<<"False string in file"<<endl;break;}
        default:
            break;
        }

    }

    return a.exec();
}
