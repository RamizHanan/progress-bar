/*
Created by Mark May and Ramiz Hanan
SDSU CS-570 Assignment 3 Multitheading
*/

#include <fstream>
#include <iostream>
#include <pthread.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#define MAXSYM 40.0

using namespace std;

void *progress_monitor(void *arg);
long word_count(std::string fileName); //spawns progress bar thread
bool checkArgs(const char *argv[]);
long get_file_size(std::string fileName); //byte size

typedef struct
{
    long *CurrentStatus;   // long which represents the current status of the computation being tracked
    long InitialValue;     //  starting value for the computation
    long TerminationValue; //  value at which the computation is complete
} PROGRESS_STATUS;

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
{
    long w_count = 0;
    long c_count = 0;
    char c = '\0';
    char curr = '\0';
    char prev = '\0';
    std::ifstream wordFile(fileName);
    PROGRESS_STATUS *pstat = new PROGRESS_STATUS;
    pthread_t progress;

    pstat->InitialValue = 0;
    pstat->TerminationValue = get_file_size(fileName);
    pstat->CurrentStatus = new long(0);

    pthread_create(&progress, NULL, progress_monitor, (void *)pstat);

    while (wordFile.get(c))
    {
        curr = c;
        c_count++;
        (*pstat->CurrentStatus)++;

        if (!isspace(curr))
            if (isspace(prev) || prev == '\0') //NULL for first char in a stream
            { 
                w_count++;
            }

        prev = curr;

    } // loop getting single characters

    pthread_join(progress, NULL);
    delete pstat->CurrentStatus;
    delete pstat;
    wordFile.close();

    return w_count;
}

long get_file_size(std::string fileName)
{
    struct stat buff;
    int size = stat(fileName.c_str(), &buff);
    return size == 0 ? buff.st_size : -1;
}

void *progress_monitor(void *pstat)
{
    string loadbar;
    double prog_val = 0.0;
    double total = 0.0;
    int num_symbols = 0;
    int i;
    PROGRESS_STATUS *progstat = (PROGRESS_STATUS *)pstat;

    double term_val = (double)progstat->TerminationValue;
    double init_val = (double)progstat->InitialValue;


    while (prog_val <= term_val)
    {
        prog_val = (double)*progstat->CurrentStatus;
        total = (prog_val / term_val);
        num_symbols = (int)((total) * MAXSYM);
        loadbar.clear();
        for (i = 1; i <= num_symbols; i++)
        {
            if (i % 10 == 0)
                loadbar += "+";
            else
                loadbar += "-";
        }

        cout << '\r' << loadbar << flush;

        if (num_symbols == (int)MAXSYM)
        {
            cout << endl;
            pthread_exit(NULL);
        }
    }
}
