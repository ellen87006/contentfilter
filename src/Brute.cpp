#include <iostream>
#include <fstream>
#include <sstream>
#include <list>
#include <cstring>
#include <vector>
#include <time.h>
using namespace std;
#define Buildfilter 1
#define Removepro 2

void FreeKeyWordList(list<char *> &list_input)
{
    for (list<char *>::iterator it_key = list_input.begin(); it_key != list_input.end(); ++it_key)
    {
        delete *it_key;
    }
}

void Log2File(const char *outputpath, int dir, int exectime)
{
    ofstream logfile;
    if (dir == Removepro)
    {
        logfile.open(outputpath, ios::app);
        logfile << "ProfanitiesRemove Time:" << exectime << " ms" << endl;
    }
    else
    {
        logfile.open(outputpath, ios::app);
        logfile << "BuiildFilter Time:" << exectime << " ms" << endl;
    }
    logfile.close();
}
//build filter
void SliceFilterlst(const char *path, const char *outputpath, char *input)
{
   // cout << "Please Wait....." << endl;
    clock_t start_buildtree = clock();
    char *delim = new char[1];
    delim[0] = '\n';
    ifstream FilterFileStream;
    stringstream RowFilter;
    FilterFileStream.open(path, ios::in);
    RowFilter << FilterFileStream.rdbuf();
    FilterFileStream.close();
    // read filter file complete
    char *Filter = new char[RowFilter.str().length() + 1];
    std::memset(Filter, 0, sizeof(char) * RowFilter.str().length());
    memcpy(Filter, RowFilter.str().c_str(), RowFilter.str().length());
    char output[512];
    std::memset(output, 0, sizeof(char) * 512);
    int i = 0;
    while (i < strlen(input))
    {
        int hit = 0;
        char *FilterSegment = strtok(Filter, delim);
        while(FilterSegment)
        {
            
            if(i+strlen(FilterSegment)< strlen(input)){
            char tmp[(strlen(FilterSegment))];
            memcpy(tmp, input+i, sizeof(char) * (strlen(FilterSegment)));
            if (strcmp(tmp, FilterSegment) == 0)
            {  
                hit = strlen(FilterSegment);
            }
            
            }
          
            FilterSegment = strtok(NULL, delim);
        }
        if (hit == 0)
        {
            i++;
        }
        else
        {
             i += hit;
           
        }
    }
    clock_t end_buildtree = clock();
    int exectime = end_buildtree - start_buildtree;
    Log2File(outputpath, Buildfilter, exectime);
}

void AddFilterlst(const char *path, char *input)
{
    ofstream Modifyfilestream;
    std::memset(input, 0, sizeof(char) * 512);
    cin.getline(input, 512);
    Modifyfilestream.open(path, ios::app);
    input[strlen(input)] = '\n';
    Modifyfilestream.write(input, strlen(input));
    Modifyfilestream.close();
}

int main(int argc, char *argv[])
{
    list<char *> list_keyword;
    ifstream Testcase;
    Testcase.open(argv[2], ios::in);
    stringstream line;
    char input[512];
    std::memset(input, 0, sizeof(char) * 512);
    while (1)
    {
        Testcase.getline(input, sizeof(input));
        if (strcmp(input, "#terminate") == 0)
        {
            break;
        }
        else if (strcmp(input, "#add") == 0)
        {
            Testcase.getline(input, sizeof(input));
            while (1)
            {
                if (strcmp(input, "#complete") == 0)
                {
                    break;
                }
                else if (strncmp(input, "#", 1))
                    AddFilterlst(argv[1], input);
            }
        }
        else if (strncmp(input, "#", 1) == 0)
        {
            cout << "type\"#terminate\" to leave add mode!!" << endl;
            cout << "type\"#add\" to leave add mode!!" << endl;
        }
        else
        {
            SliceFilterlst(argv[1], argv[3], input);
        }
        std::memset(input, 0, sizeof(char) * 512);
    }
    Testcase.close();
    return 0;
}