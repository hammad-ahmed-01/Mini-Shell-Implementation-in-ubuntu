#include <iostream>
#include <string.h>
#include <unistd.h>
#include <cstdlib>
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>

#define TOKENSIZE 100

using namespace std;

void StrTokenizer(char *line, char **argv);
void myExecvp(char **argv);
int GetEnv();

int main()
{
    char input[250];
    char *argv[TOKENSIZE];

    while (true)
    {
        cout << "cwushell-> ";
        cin.getline(input, 250);
        StrTokenizer(input, argv);

        if (strcmp(argv[0], "exit") == 0)
        {
            if (argv[1] != NULL)
                exit(atoi(argv[1]));
            else
                break;
        }
        else if (strcmp(argv[0], "mkdir") == 0)
        {
            if (argv[1] != NULL)
                mkdir(argv[1], 0777);
        }
        else if (strcmp(argv[0], "ls") == 0)
        {
            system("ls");
        }
        else if (strcmp(argv[0], "cmp") == 0)
        {
            myExecvp(argv);
        }
    }

    return 0;
}

void myExecvp(char **argv)
{
    pid_t pid;
    int status;
    int childStatus;
    pid = fork();
    if (pid == 0)
    {
        childStatus = execvp(*argv, argv);
        if (childStatus < 0)
        {
            cout << "ERROR: wrong input" << endl;
        }
        exit(0);
    }
    else if (pid < 0)
    {
        cout << "Something went wrong!" << endl;
    }
    else
    {
        waitpid(pid, &status, 0);
    }
}

void StrTokenizer(char *input, char **argv)
{
    char *stringTokenized;
    stringTokenized = strtok(input, " ");
    int argIndex = 0;
    while (stringTokenized != NULL)
    {
        argv[argIndex] = stringTokenized;
        stringTokenized = strtok(NULL, " ");
        argIndex++;
    }
    argv[argIndex] = NULL;
}

int GetEnv()
{
    char *path2;
    char *arr2[250];
    char *Tokenized;
    path2 = getenv("PATH");
    Tokenized = strtok(path2, ":");
    int k = 0;
    while (Tokenized != NULL)
    {
        arr2[k] = Tokenized;
        Tokenized = strtok(NULL, ":");
        k++;
    }
    arr2[k] = NULL;
    
    return 0; // Return an integer value (can be modified based on your requirements)
}

