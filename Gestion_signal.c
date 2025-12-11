#include <sys/types.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>


#define MAX_COMMANDE_LENGTH 128
#define PROMPT_SIZE 64
#define MAX_ARGS 2 

int main (){
    char buffer[MAX_COMMANDE_LENGTH];
    char *prompt =" "; 
    ssize_t nbytes;

    const char *welcom_message = "$./enseash \n Bienvenue dans le Shell ENSEA. \n Pour quitter, taper 'exit'. \n";
    const char *out1_message = "Bye, see you soon \n Fermeture d'enseash.\n";
    const char *out2_message = "Bye, bye ... Ctr+D killed me :( \n Fermeture d'enseash.\n";


    write(STDOUT_FILENO, welcom_message, strnlen(welcom_message, 128));

    while (1){
        
        write(STDOUT_FILENO, "enseash [", 10);
        write(STDOUT_FILENO, prompt, strnlen(prompt, 128));
        //write(STDOUT_FILENO,"ici",4);
        write(STDOUT_FILENO, "] % ", 4); 

        nbytes = read(STDIN_FILENO, buffer, MAX_COMMANDE_LENGTH); // Reading the command

        //Execution of a command
        buffer[nbytes - 1] = '\0'; 

        pid_t pid;
        int status;

        pid = fork();
        if (pid < 0) {
            perror("enseash: fork error");
            
        } else if (pid == 0) {
            // child process
            
            execlp(buffer, buffer, (char *) NULL); // the output of this function is an int 

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

            pid_t fpid; 
    
            while ((fpid = wait(&status))!= -1){
                write(STDOUT_FILENO,"ici",4);
                if (WIFEXITED(status)){
                    prompt = "exit : " + WEXITSTATUS(status);
                } else if (WIFSIGNALED(status)){
                    prompt = "sign : " + WTERMSIG (status);
                }
        }
        }

        //End of the Shell

        if (nbytes == 0) {
            write(STDOUT_FILENO, out2_message, strnlen(out2_message, 128));
            break;
        }

        if (strncmp(buffer, "exit", 4) == 0) {
            write(STDOUT_FILENO, out1_message, strnlen(out1_message, 128));
            break;
        }

        // Return and signals




    }

    return 0;
}