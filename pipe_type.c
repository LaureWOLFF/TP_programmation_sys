#include <sys/types.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <time.h> // adding to use clock_gettime 
#include <fcntl.h> // adding to use open()

// Constants
#define MAX_COMMANDE_LENGTH 256
#define PROMPT_SIZE 64
#define MAX_ARGS 2 

// Functions for the execution 
static void reverse_string(char *str, int length);
int int_to_string(int value, char *buffer);




int main (){
    char buffer[MAX_COMMANDE_LENGTH];
    char *argv[64]; // It's possible to have 64 parameters in one command
    char *pgrep_argv1[64];
    char *pgrep_argv2[64];
    char *input_file = NULL;
    char *output_file = NULL;
    ssize_t nbytes;
    char status_str[12]; // Buffer to stockage the converted number(ex: "127")
    char time_str[12];
    int value_to_convert;
    int status_len;
    int time_len;
    int ms = 0;
    int last_exit_status = 0; 

    struct timespec start, end; 

    const char *welcom_message = "$./enseash \n Bienvenue dans le Shell ENSEA. \n Pour quitter, taper 'exit'. \n";
    const char *out1_message = "\n Bye, see you soon \n Fermeture d'enseash.\n";
    const char *out2_message = "\n Bye, bye ... Ctr+D killed me :( \n Fermeture d'enseash.\n";


    write(STDOUT_FILENO, welcom_message, strnlen(welcom_message, 128));

    while (1){
        
        write(STDOUT_FILENO, "enseash [", 12); 

        // Determinate the type of the statut (exit or sign)
        if (last_exit_status >= 0) {
            // It is an exit (exit:N)
            write(STDOUT_FILENO, "exit:",3);
            value_to_convert = last_exit_status;
        } else {
            // It is a signal (sign:N). We use the positive value
            write(STDOUT_FILENO, "sign:", 5);
            value_to_convert = -last_exit_status;
        }

        // Conversion in string
        status_len = int_to_string(value_to_convert, status_str);
        time_len = int_to_string(ms, time_str);

        // Writing in the shell
        write(STDOUT_FILENO, status_str, status_len);

        write(STDOUT_FILENO, "|time:", 6);

        write(STDOUT_FILENO, time_str, time_len);

        write(STDOUT_FILENO, "ms] % ", 6);

        nbytes = read(STDIN_FILENO, buffer, MAX_COMMANDE_LENGTH); // Reading the command

        //Execution of a command with several parameters
        buffer[strcspn(buffer, "\n")] = '\0';

        // stdin and sdtout
        char *stdout = strchr(buffer, '>'); // Research the symbol >
        if (stdout !=0){ // It is a stdout
            *stdout = '\0'; // changes the character in \0 for the commande being executing
            output_file = stdout + 1;
            while (*output_file == ' ') output_file++;  // researches the next word
            //write(STDOUT_FILENO, "stdout détecté et > remplacé \r\n", 32);
        } 

        char *stdin = strchr(buffer, '<'); //Research the symbol <
        if (stdin !=0){ // It is a stin
            *stdin = '\0';
            input_file = stdin + 1;
            while (*input_file == ' ') input_file++;
            //write(STDOUT_FILENO, "stdin détecté et < remplacé \r\n", 31);
        }

        // Pipe type
        char *pgrep = strchr(buffer, '|'); //Research the first commande
        if (pgrep !=0){ // It is a pgrep
            *pgrep = '\0';
            char *part1 = buffer;
            char *part2 = pgrep + 1;

            // The first command 
            int i = 0;
            pgrep_argv1[i] = strtok(part1, " ");
            while (pgrep_argv1[i] != NULL) {
                pgrep_argv1[++i] = strtok(NULL, " ");
            }

            // The second command
            int j = 0;
            pgrep_argv2[j] = strtok(part2, " ");
            while (pgrep_argv2[j] != NULL) {
                pgrep_argv2[++j] = strtok(NULL, " ");
            }
            write(STDOUT_FILENO, "pgrep détecté et | remplacé \r\n", 34);
        } 

        int arg_count = 0;
        char *argv_start = buffer;
        char *argv_end;

        memset (argv, 0, sizeof(argv)); // argv becomes empty

        while ((argv_end = strchr(argv_start, ' ')) != NULL && arg_count < 10){
            *argv_end = '\0'; // Remplacement of the space by \0 
        
            if (*argv_start != '\0') { // Avoiding add too argument (two space por exemple)
                argv[arg_count] = argv_start;
                arg_count++;
            }
            argv_start = argv_end + 1;
        }

        //The last parameter after the last space and if the number of parameter is <10
        if (*argv_start != '\0' && arg_count < 10) { 
            argv[arg_count] = argv_start;
            arg_count++;
        }
        argv[arg_count] = NULL;

        pid_t pid;
        pid_t pid1;
        pid_t pid2;
        int status;

        //Measure of the time of a execution
        clock_gettime(CLOCK_MONOTONIC, &start);

        if(pgrep != 0 ){ // The case of pgrep we have two duplication of the father process
            int pipefd[2]; // two file, on for the reading and the other for the writing
            int pipepgrep = pipe(pipefd); // Creation of an anonym tube
            if ( pipepgrep == -1){
                perror("La création du tube s'est mal passé");
                exit(EXIT_FAILURE);
            }

            pid1 = fork();
            if ((pid1 < 0)) {
                perror("enseash: fork error");
                exit(EXIT_FAILURE); // The processus did not spilt
            }

            if (pid1 == 0) {
                // child 1 process: the first command
                close(pipefd[0]); // Without utility
                dup2(pipefd[1], STDOUT_FILENO); // the first command is ls so writing 
                close (pipefd[1]);
                execvp(pgrep_argv1[0], pgrep_argv1);
                // If the command doesn't find
                perror("enseash: command not found");
                exit(EXIT_FAILURE); 
            } 

            pid2 = fork();
            if ((pid1 < 0)||(pid1 < 0)) {
                perror("enseash: fork error");
                exit(EXIT_FAILURE); // The processus did not spilt
            }

            if (pid2 == 0) {
                // child 2 process: the second command
                close (pipefd[1]); // Without utiliy 
                dup2(pipefd[0], STDIN_FILENO); // the second command is wc so reading 
                close (pipefd[0]);
                execvp(pgrep_argv2[0], pgrep_argv2);
                // If the command doesn't find
                perror("enseash: command not found");
                exit(EXIT_FAILURE);   
            } 
            else {
                // father process
                close(pipefd[0]);
                close(pipefd[1]);
                if (waitpid(pid1, NULL, 0) == -1) {
                    perror("enseash: waitpid error on the first son");  // wait the end of the child
                }
                if (waitpid(pid2, NULL, 0) == -1) {
                    perror("enseash: waitpid error on the second son");  // wait the end of the child
                }
                // if (WIFEXITED(status)){
                //     last_exit_status = WEXITSTATUS(status);              
                // } else if (WIFSIGNALED(status)){
                //     last_exit_status = - WTERMSIG(status); 
                // }
            }
        }
        else {
            pid = fork();
            if (pid < 0) {
                perror("enseash: fork error");
                exit(EXIT_FAILURE); // The processus did not spilt
            }
            if (pid == 0) {
                // child process
                
                //stin and stout files gestion:

                if (output_file != 0){
                    int fd_out = open (output_file, O_WRONLY | O_CREAT);
                    if (fd_out ==  0){
                        perror("erreur à l'ouverture");
                        exit(EXIT_FAILURE);
                    }
                    dup2(fd_out, STDOUT_FILENO); // dup2 (.., STDOUT_FILENO) can write in the fd_out
                    close (fd_out);
                }

                if (input_file != 0){
                    int fd_in = open (input_file, O_RDONLY);
                    if (fd_in ==  0){
                        perror("erreur à l'ouverture");
                        exit(EXIT_FAILURE);
                    }
                    dup2(fd_in, STDIN_FILENO); // dup2 (.., STDOUT_FILENO) can read the fd_out
                    // Warning a input file , the second parameter need STDIN_FILENO and not STDOUT_FILENO 
                    close (fd_in);
                }

                execvp(argv[0], argv);

                // If the command doesn't find
                perror("enseash: command not found");
                exit(EXIT_FAILURE); 
                
            } else {
                // father process
                if (waitpid(pid, &status, 0) == -1) {
                    perror("enseash: waitpid error");  // wait the end of the child
                }
                if (WIFEXITED(status)){
                    last_exit_status = WEXITSTATUS(status);              
                } else if (WIFSIGNALED(status)){
                    last_exit_status = - WTERMSIG(status); 
                }
            }
        }

        //End of the Shell

        if (nbytes == 0) { // the end with the Ctrl+D
            write(STDOUT_FILENO, out2_message, strnlen(out2_message, 128));
            break;
        }

        if (strncmp(buffer, "exit", 4) == 0) { // the end if the buffer = "exit"
            write(STDOUT_FILENO, out1_message, strnlen(out1_message, 128));
            break;
        }

    clock_gettime(CLOCK_MONOTONIC, &end);

    long time_ms = (end.tv_sec - start.tv_sec)*1000 + (end.tv_nsec - start.tv_nsec)/1000000;
    ms = (int) time_ms;

    }

    return 0;
}


// some functions to convert an int in string (for print the signal or the exit)
static void reverse_string(char *str, int length) {
    /*Inversion of a string 
    Inputs: a pointeur to the string which we want reverse
            the number of the characters which we want to reverse
    Goal: we use two indice in each extremety of the string and we increse one and decrease the other*/
    int start = 0;
    int end = length - 1;
    char temp; 

    while (start < end) {
        
        temp = str[start];
        str[start] = str[end];
        str[end] = temp;
        
        start++;
        end--;
    }
}

int int_to_string(int value, char *buffer) {
    /* Converion an int in a string
    Inputs: the int which we want to convert 
            a pointeur to a table of characters which we write in the int
    Goal: Each number is place in the buffer and to have the right number we revee all the chain.
    Output: the size of the chain 
    */
    
    int i = 0;
    int is_negative = 0;
    long temp_value = value; 

    if (value == 0) {
        buffer[i++] = '0';
        buffer[i] = '\0';
        return 1;
    }
    
    if (value < 0) {
        is_negative = 1;
        temp_value = -temp_value; 
    }
    
    while (temp_value > 0) {

        buffer[i++] = (temp_value % 10) + '0'; 
        
        temp_value /= 10;
    }
    
    if (is_negative) {
        buffer[i++] = '-';
    }
    
    buffer[i] = '\0'; // Adding the \'0'

    reverse_string(buffer, i);
    
    return i; // Size of the chain (without '\0')
}