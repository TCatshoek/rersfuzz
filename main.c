#include <stdio.h>

// /home/tom/projects/lstar/rers/TrainingSeqReachRers2019/Problem11/Problem11

#include <unistd.h>
#include <sys/wait.h>
#include <sys/prctl.h>
#include <signal.h>
#include <stdlib.h>
#include <string.h>
#include <cwalk.h>

#include "util.h"
#include "stringstorage.h"

int interact(const char*, const char*, const char*);
int process(const char*);

struct stringstorage* s;

int main(int argc, char** argv){
    char* path = "/home/tom/projects/lstar/rers/TrainingSeqReachRers2019/Problem11/Problem11";

    char** alphabet = get_alphabet(path);
    displayalphabet(alphabet);

//    s = strst_init(1024);
//
//
//    char* path = "/home/tom/projects/lstar/rers/TrainingSeqReachRers2019/Problem11/Problem11";
//    char* input = "1 2 3 4 5 6 7 8 9 10 1 2 3 4 5 6 7 8 9 10 1 2 3 4 5 6 7 8 9 10 1 2 3 4 5 6 7 8 9 10 1 2 3 4 5 6 7 8 9 10 1 2 3 4 5 6 7 8 9 10 1 2 3 4 5 6 7 8 9 10 0 ";
//
//    // Figure out binary name to set in execl
//    const char *basename;
//    size_t length;
//    cwk_path_get_basename(path, &basename, &length);
//
//    int x = 0;
//
//    for (int i = 0; i < 10000; i++){
//        printf("%d\n", i);
//        x = interact(path, basename, input);
//    }
}

int interact(const char* path, const char* basename, const char* input)
{

    pid_t pid = 0;
    int inpipefd[2];
    int outpipefd[2];
    char buf[256];
    int status;

    pipe(inpipefd);
    pipe(outpipefd);

    pid = fork();

    if (pid == 0)
    {
        // Child
        dup2(outpipefd[0], STDIN_FILENO);
        dup2(inpipefd[1], STDOUT_FILENO);
        dup2(inpipefd[1], STDERR_FILENO);

        //ask kernel to deliver SIGTERM in case the parent dies
        prctl(PR_SET_PDEATHSIG, SIGTERM);

        //replace with given process
        execl(path, basename, (char*) NULL);

        // If we reach this (shouldn't happen):
        perror("Child failed");
        exit(1);
    }
    else if (pid < 0) {
        perror("Failed");
    }
    else{
        //close unused pipe ends
        close(outpipefd[0]);
        close(inpipefd[1]);

        write(outpipefd[1], input, strlen(input));

        waitpid(pid, &status, 0);

        read(inpipefd[0], buf, 256);

        close(outpipefd[1]);
        close(inpipefd[0]);

        //printf("Received answer: \n%s\n", buf);

        return status;
    }
}

