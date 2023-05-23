#include <iostream>
#include <cstdlib>
#include <signal.h>
#include <cstring>
#include <algorithm>
#include <cctype>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <fstream>
#include <cerrno>
#include <vector>
#include <dirent.h>

using namespace std;

//Function Definitions
void myExecvp(char **argv);                                    //Custom implementation of execvp system call
void StrTokenizer(char *input, char **argv);                   //Tokenize the input command string into individual arguments
int GetEnv();                                                  //Get the value of an environment variable
void touch(const char *filename);                              //Create an empty file with the given filename
void rm(const char *filename);                                 //Remove the given file
void fg(int pid);                                              //Bring the input process to the foreground
void cp(const char *source, const char *destination);          //Copy the contents of source file to destination file
void cat(const char *filename);                                //Display the contents of a file
void grep(const char *pattern, const char *filename);          //Search for a pattern in file and display matching lines
void swap(const char* file1, const char* file2);               //Swap the contents of two files
void paste(const char *source, const char *destination);       //Append the contents of the source file to destination file

//Function to handle errors in system calls
void handleSystemCallError(const string& errorMessage) {     
    cerr << "ERROR: " << errorMessage << ", Error code: " << errno << endl;
}

//Function to convert a string to lowercase
string toLowercase(string str) {
    transform(str.begin(), str.end(), str.begin(), [](unsigned char c) { return tolower(c); });
    return str;
}

int main() {
    char input[1000];
    char *argv[100];
    char *username = getenv("USER");
    cout << "Welcome, " << username << "!" << endl;
    cout << "Following Commands Are Implemented: " << endl;      //Listing the implemented commands
    cout <<"exit"<< endl;
    cout <<"mkdir" << endl;
    cout <<"ls" << endl;
    cout <<"cmp" << endl;
    cout <<"touch" << endl;
    cout <<"rm" << endl;
    cout <<"cp" << endl;
    cout <<"cat" << endl;
    cout <<"grep" << endl;
    cout <<"swap" << endl;
    cout <<"paste" << endl;
    cout <<"ps" << endl;
    cout <<"kill" << endl;
    cout <<"cd" << endl;
    cout <<"top" << endl;
    cout <<"pstree" << endl;
    cout <<"who" << endl;
    cout <<"fg" << endl;
    cout <<"nice" << endl;
    cout <<"df" << endl;
    cout <<"To Check The History of the Commands Type 'history'"<<endl;
    vector<string> commandHistory;  //For storing the command history

    while (true) {
        cout << "> ";
        cin.getline(input, sizeof(input));

        if (strcmp(input, "exit") == 0) {
            break;
        } else if (strcmp(input, "history") == 0) {
            cout << "Command History:" << endl;
            for (const auto& command : commandHistory) {
                cout << command << endl;
            }
        } else if (strcmp(input, "top") == 0) {
            argv[0] = (char *)"top";
            argv[1] = nullptr;
            myExecvp(argv);
            commandHistory.push_back(input); // Add command to history
        } else if (strcmp(input, "htop") == 0) {
            argv[0] = (char *)"htop";
            argv[1] = nullptr;
            myExecvp(argv);
            commandHistory.push_back(input); // Add command to history
        } else if (strcmp(input, "pstree") == 0) {
            argv[0] = (char *)"pstree";
            argv[1] = nullptr;
            myExecvp(argv);
            commandHistory.push_back(input); // Add command to history
        } else if (strcmp(input, "who") == 0) {
            argv[0] = (char *)"who";
            argv[1] = nullptr;
            myExecvp(argv);
            commandHistory.push_back(input); // Add command to history
        } else if (strcmp(input, "fg") == 0) {
            cout << "Enter process ID to bring to the foreground: ";
            cin.getline(input, sizeof(input));
            int pid = atoi(input);
            fg(pid);
            commandHistory.push_back(input); // Add command to history
        } else if (strcmp(input, "nice") == 0) {
            argv[0] = (char *)"nice";
            argv[1] = nullptr;
            myExecvp(argv);
            commandHistory.push_back(input); // Add command to history
        } else if (strcmp(input, "df") == 0) {
            argv[0] = (char *)"df";
            argv[1] = nullptr;
            myExecvp(argv);
            commandHistory.push_back(input); // Add command to history
        } else if (strcmp(input, "mkdir") == 0) {
            cout << "Enter directory name: ";
            cin.ignore(); // Discard the newline character from the previous input
            cin.getline(input, sizeof(input));
            char* directory = input;
            if (mkdir(directory, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH) != 0) {
                handleSystemCallError("Failed to create directory");
            } else {
                cout << "Directory created successfully." << endl;
            }
            commandHistory.push_back(input); // Add command to history
        } else if (strcmp(input, "ls") == 0) {
            DIR *dir;
            struct dirent *ent;
            if ((dir = opendir(".")) != nullptr) {
                while ((ent = readdir(dir)) != nullptr) {
                    cout << ent->d_name << endl;
                }
                closedir(dir);
            } else {
                handleSystemCallError("Failed to open directory");
            }
            commandHistory.push_back(input); // Add command to history
        } else if (strcmp(input, "cmp") == 0) {
            cout << "Enter first file name: ";
            cin.getline(input, sizeof(input));
            string file1 = input;
            cout << "Enter second file name: ";
            cin.getline(input, sizeof(input));
            string file2 = input;

            FILE *file1Ptr = fopen(file1.c_str(), "rb");
            FILE *file2Ptr = fopen(file2.c_str(), "rb");

            if (file1Ptr == nullptr) {
                handleSystemCallError("Failed to open first file");
            } else if (file2Ptr == nullptr) {
                handleSystemCallError("Failed to open second file");
                fclose(file1Ptr);
            } else {
                int ch1 = getc(file1Ptr);
                int ch2 = getc(file2Ptr);
                int position = 0;
                bool filesAreEqual = true;

                while (ch1 != EOF && ch2 != EOF) {
                    position++;

                    if (ch1 != ch2) {
                        filesAreEqual = false;
                        cout << "Files differ at position " << position << endl;
                        break;
                    }

                    ch1 = getc(file1Ptr);
                    ch2 = getc(file2Ptr);
                }

                if (filesAreEqual && ch1 == EOF && ch2 == EOF) {
                    cout << "Files are identical." << endl;
                }

                fclose(file1Ptr);
                fclose(file2Ptr);
            }
            commandHistory.push_back(input); // Add command to history
        } else if (strcmp(input, "ps") == 0) {
            argv[0] = (char *)"ps";
            argv[1] = nullptr;
            myExecvp(argv);
            commandHistory.push_back(input); // Add command to history
        } else if (strcmp(input, "kill") == 0) {
            cout << "Enter process ID to kill: ";
            cin.getline(input, sizeof(input));
            int pid = atoi(input);
            if (kill(pid, SIGKILL) == 0) {
                cout << "Process killed successfully." << endl;
            } else {
                handleSystemCallError("Failed to kill process");
            }
            commandHistory.push_back(input); // Add command to history
        } else if (strcmp(input, "swap") == 0) {
            cout << "Enter first file name: ";
            cin.getline(input, sizeof(input));
            string file1 = input;
            cout << "Enter second file name: ";
            cin.getline(input, sizeof(input));
            string file2 = input;
            swap(file1.c_str(), file2.c_str());
            commandHistory.push_back(input); // Add command to history
        } else if (strncmp(input, "cd", 2) == 0) {
            char *directory = input + 3;
            if (chdir(directory) == 0) {
                cout << "Changed directory to " << directory << endl;
            } else {
                handleSystemCallError("Failed to change directory");
            }
            commandHistory.push_back(input); // Add command to history
        } else {
            StrTokenizer(input, argv);
            if (strcmp(argv[0], "touch") == 0) {
                touch(argv[1]);
                commandHistory.push_back(input); // Add command to history
            } else if (strcmp(argv[0], "rm") == 0) {
                rm(argv[1]);
                commandHistory.push_back(input); // Add command to history
            } else if (strcmp(argv[0], "cp") == 0) {
                cp(argv[1], argv[2]);
                commandHistory.push_back(input); // Add command to history
            } else if (strcmp(argv[0], "cat") == 0) {
                cat(argv[1]);
                commandHistory.push_back(input); // Add command to history
            } else if (strcmp(argv[0], "grep") == 0) {
                grep(argv[1], argv[2]);
                commandHistory.push_back(input); // Add command to history
            } else if (strcmp(argv[0], "swap") == 0) {
                swap(argv[1], argv[2]);
                commandHistory.push_back(input); // Add command to history
            } else if (strcmp(argv[0], "paste") == 0) {
                paste(argv[1], argv[2]);
                commandHistory.push_back(input); // Add command to history
            } else {
                cout << "Invalid command." << endl;
            }
        }
    }

    return 0;
}

void myExecvp(char **argv) {
    pid_t pid;
    int status;

    if ((pid = fork()) < 0) {
        handleSystemCallError("Fork failed");
        exit(1);
    } else if (pid == 0) {
        if (execvp(*argv, argv) < 0) {
            handleSystemCallError("Execvp failed");
            exit(1);
        }
    } else {
        while (wait(&status) != pid) {
            // Wait for child process to finish
        }
    }
}

void fg(int pid) {
  if (kill(pid, SIGCONT) == 0) {
    cout << "Process resumed in the foreground." << endl;
  } else {
    handleSystemCallError("Failed to resume process.");
  }
}
  

void StrTokenizer(char *input, char **argv) {
    while (*input != '\0') {
        while (*input == ' ' || *input == '\t' || *input == '\n')
            *input++ = '\0';
        *argv++ = input;
        while (*input != '\0' && *input != ' ' && *input != '\t' && *input != '\n')
            input++;
    }
    *argv = nullptr;
}

void touch(const char *filename) {
    ofstream file(filename);
    if (file) {
        cout << "File created: " << filename << endl;
    } else {
        handleSystemCallError("Failed to create file");
    }
}

void rm(const char *filename) {
    if (remove(filename) == 0) {
        cout << "File removed: " << filename << endl;
    } else {
        handleSystemCallError("Failed to remove file");
    }
}

void cp(const char *source, const char *destination) {
    ifstream sourceFile(source, ios::binary);
    ofstream destFile(destination, ios::binary);
    destFile << sourceFile.rdbuf();
    sourceFile.close();
    destFile.close();
    cout << "File copied: " << source << " -> " << destination << endl;
}

void cat(const char *filename) {
    ifstream file(filename);
    if (file) {
        cout << file.rdbuf();
    } else {
        handleSystemCallError("Failed to open file");
    }
}

void grep(const char *pattern, const char *filename) {
    ifstream file(filename);
    if (file) {
        string line;
        while (getline(file, line)) {
            if (line.find(pattern) != string::npos) {
                cout << line << endl;
            }
        }
    } else {
        handleSystemCallError("Failed to open file");
    }
}

void swap(const char* file1, const char* file2) {
    ifstream f1(file1, ios::binary);
    ifstream f2(file2, ios::binary);
    ofstream temp("temp", ios::binary);
    temp << f2.rdbuf();
    f2.close();
    ofstream f2out(file2, ios::binary);
    f2out << f1.rdbuf();
    f1.close();
    ofstream f1out(file1, ios::binary);
    f1out << temp.rdbuf();
    temp.close();
    cout << "Files swapped: " << file1 << " <-> " << file2 << endl;
}

void paste(const char *source, const char *destination) {
    ifstream sourceFile(source);
    ofstream destFile(destination, ios::app);
    destFile << sourceFile.rdbuf();
    sourceFile.close();
    destFile.close();
    cout << "File appended: " << source << " -> " << destination << endl;
}
