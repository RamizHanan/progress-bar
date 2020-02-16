
#include <fstream>
#include <iostream>

using namespace std;


typedef struct {
    long * CurrentStatus; // long which represents the current status of the computation being tracked
    long InitialValue; //  starting value for the computation
    long TerminationValue; //  value at which the computation is complete
} PROGRESS_STATUS;

void progress_monitor(PROGRESS_STATUS*);
long word_count(std::string fileName); //spawns progress bar thread 

int main(int argc, const char *argv[])
{
    std::string filePath = argv[1];

    if (checkArgs(argv))
    {
        word_count(filePath);
    }
    
    return 0;
}


bool checkArgs(const char *argv[])
{
    if (argv[1] == NULL)
        {
            cerr << "Missing arg: Please enter absolute path of file to be checked" << endl;
            return false;
        }

    return true;
}