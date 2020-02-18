
#include <fstream>
#include <iostream>
#include <pthread.h>
using namespace std;

const char *separators = " \t";

typedef struct {
    long * CurrentStatus; // long which represents the current status of the computation being tracked
    long InitialValue; //  starting value for the computation
    long TerminationValue; //  value at which the computation is complete
} PROGRESS_STATUS;

void *progress_monitor(void* arg);
long word_count(std::string fileName); //spawns progress bar thread 
bool checkArgs(const char *argv[]);
long file_size(std::string fileName); //byte size

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

long word_count(std::string fileName)
{ //spawns progress bar thread 

    long fsize = file_size(fileName);
    PROGRESS_STATUS* pstat = new PROGRESS_STATUS;

    pstat->InitialValue = 0;
    pstat->TerminationValue = fsize;
    pstat->CurrentStatus = 0;

    pthread_t progress;
    pthread_create(&progress, NULL, progress_monitor, (void *)pstat);

    long w_count = 0;
    long c_count = 0;
    //Read in valid word file
    std::ifstream wordFile(fileName);
    char c;
    char curr = NULL;
    char prev = NULL;
    while (wordFile.get(c)){
        curr = c;
        c_count++;

        if(!isspace(curr))
            if(isspace(prev) || prev == NULL){ //NULL for first char in a stream
                w_count++;
                pstat->CurrentStatus+= c_count;
            }

        prev = curr;
        
    }        // loop getting single characters


    wordFile.close();   

    pthread_join(progress, NULL);

    return w_count;
}

long file_size(std::string fileName){
    long count = 0;

    //Read in valid word file
    std::ifstream wordFile(fileName);

    char c;
    
    while (wordFile.get(c))   // loop getting single characters
        count++;

    wordFile.close();   

    return count;
}

void * progress_monitor(void* pstat){
    PROGRESS_STATUS* progstat = (PROGRESS_STATUS *) pstat;
    //if word count changes then print new progress
    //print the progress bar


}