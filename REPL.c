#include <sys/types.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>


#define MAX_COMMANDE_LENGTH 128
#define MAX_ARGS 2 

int main (){
    char buffer[MAX_COMMANDE_LENGTH];
    ssize_t nbytes; 

    const char *welcom_message = "$./enseash \n Bienvenue dans le Shell ENSEA. \n Pour quitter, taper 'exit'. \n";
    write(STDOUT_FILENO, welcom_message, strnlen(welcom_message, 128));

    while (1){
        write (STDOUT_FILENO, "enseash % ", 10);
        nbytes = read(STDIN_FILENO, buffer, MAX_COMMANDE_LENGTH); // Reading the command// 1. Éliminer le '\n' (saut de ligne) pour n'avoir que le nom de la commande
        buffer[nbytes - 1] = '\0'; 

        pid_t pid;
        int status;

        pid = fork();
        if (pid < 0) {
            perror("enseash: fork error");
            
        } else if (pid == 0) {
            // child process
            
            execlp(buffer, buffer, (char *) NULL);
            // printf("This line must not be printed \n");
            // printf("%s", buffer);

            // If the command doesn't find
            perror("enseash: command not found");
            exit(EXIT_FAILURE); 
            
        } else {
            // father process
            if (waitpid(pid, &status, 0) == -1) {
                perror("enseash: waitpid error");  // wait the end of the child
            }
        }
        
    }

    return 0;
}