#include <QCoreApplication>
#include <cmath>
#include <cstring>
#include <vector>
#include "direction.cpp"
#include "parser.cpp"

using namespace std;

int getLinesFromFile(string *arraysString, string file)
{
    int i = 0;
    ifstream in(file);      // окрываем файл для чтения
    if (in.is_open())
    {
        while (getline(in, *(arraysString+i)))
        {
           i++;
        }
    }
    in.close();             // закрываем файл
    return i;               //колл. строк
}

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    try{
        string file = {"E:\\Qt project\\All Different Directions\\alldifferentdirections\\table.csv"};//sample.in
        Parser parser(file, ';');
        //bool ss = parser.detectIsTable(',');//
        //cout<<ss;
        cout<<parser.automaticSeparatorDetection();//isTable();

       /* parser.parseFile();
        vector<vector<string>> parsedData = parser.getParsedData();
        if(!Direction::validateData(parsedData))
        {
            cout<<"Error data file"<<endl;
            return a.exec();
        }

        Direction::runTasks(parsedData);*/

    }

    catch(int error)
    {
        switch (error) {
        case 98: {cout<<"Error: file is empty "<<error<<endl; break;}
        case 99: {cout<<"Error "<<error<<endl; break;}
        case 100:{cout<<"False string in file"<<endl;break;}
        default:
            break;
        }

    }

    return a.exec();
}
