
#include <fstream>
#include <iostream>
#include <pthread.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
using namespace std;

#define MAXSYM 40
const char *separators = " \t";

typedef struct {
    long *CurrentStatus; // long which represents the current status of the computation being tracked
    long InitialValue; //  starting value for the computation
    long TerminationValue; //  value at which the computation is complete
} PROGRESS_STATUS;

void *progress_monitor(void* arg);
long word_count(std::string fileName); //spawns progress bar thread 
bool checkArgs(const char *argv[]);
long get_file_size(std::string fileName); //byte size

int main(int argc, const char *argv[])
{
    std::string filePath = argv[1];
    long num_words;

    if (checkArgs(argv))
    {
        num_words = word_count(filePath);
    }

    cout << "There are " << num_words << " words in " << argv[1] << ".\n";
    
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

    PROGRESS_STATUS* pstat = new PROGRESS_STATUS;

    pstat->InitialValue = 0;
    pstat->TerminationValue = get_file_size(fileName);
    cout << "File size in bytes is " << get_file_size(fileName) << endl;
    pstat->CurrentStatus = new long(0);

    pthread_t progress;
    pthread_create(&progress, NULL, progress_monitor, (void *)pstat);

    long w_count = 0;
    long c_count = 0;
    //Read in valid word file
    std::ifstream wordFile(fileName);
    char c = NULL;
    char curr = NULL;
    char prev = NULL;
    while (wordFile.get(c)){
        curr = c;
        c_count++;
        *(pstat->CurrentStatus) += c_count;

        if(!isspace(curr))
            if(isspace(prev) || prev == NULL){ //NULL for first char in a stream
                w_count++;
            }

        prev = curr;
        
    }        // loop getting single characters



    pthread_join(progress, NULL);
    delete pstat->CurrentStatus;
    delete pstat;
    wordFile.close();   

    return w_count;
}

long get_file_size(std::string fileName){
    struct stat buff;
    int size = stat(fileName.c_str(), &buff);
    return size == 0 ? buff.st_size : -1;
}

void * progress_monitor(void* pstat){
    PROGRESS_STATUS* progstat = (PROGRESS_STATUS *) pstat;
    //every time the char count changes then print new progress
    //print the progress bar
    //cout << "In the thread." << endl;

    long term_val = progstat->TerminationValue;
    long init_val = progstat->InitialValue;

    while (*(progstat->CurrentStatus) <= term_val) {
        long progress = (*(progstat->CurrentStatus) - init_val);
        long total = term_val - init_val;
        int num_symbols = ((progress / total) * MAXSYM);
        cout << "Progress: " << progress << ", Total: " << total << ", Num_symbols: " << num_symbols << endl;
        int i;
        //string loadbar = "";
        for (i = 1; i <= num_symbols; i++) {
            if (i % 10 == 0) 
                //loadbar += "+";
                cout << "+";
            else
                //loadbar += "-";
                cout << "-";
        }
        cout << flush;
        if (num_symbols == MAXSYM) {
            cout << "\nOut of the thread." << endl;
            pthread_exit(NULL);
        }
    }
}