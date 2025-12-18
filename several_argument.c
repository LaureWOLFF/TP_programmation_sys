#include <sys/types.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <time.h> // adding to use clock_gettime 

// Constants
#define MAX_COMMANDE_LENGTH 256
#define PROMPT_SIZE 64
#define MAX_ARGS 2 

// Functions for the execution 
static void reverse_string(char *str, int length);
int int_to_string(int value, char *buffer);




int main (){
    char buffer[MAX_COMMANDE_LENGTH];
    char *prompt =" "; 
    char *argv[64]; // It's possible to have 10 parameters in one command
    char *arg_storage[64][256];
    ssize_t nbytes;
    char status_str[12]; // Buffer to stockage the converted number(ex: "127")
    char time_str[12];
    int value_to_convert;
    int status_len;
    int time_len;
    int ms;
    int last_exit_status; 

    struct timespec start, end; 

    const char *welcom_message = "$./enseash \n Bienvenue dans le Shell ENSEA. \n Pour quitter, taper 'exit'. \n";
    const char *out1_message = "\n Bye, see you soon \n Fermeture d'enseash.\n";
    const char *out2_message = "\n Bye, bye ... Ctr+D killed me :( \n Fermeture d'enseash.\n";


    write(STDOUT_FILENO, welcom_message, strnlen(welcom_message, 128));

    while (1){
        
        write(STDOUT_FILENO, "enseash [", 9); 

        // 2. Detreminate the type of the statut (exit or sign)
        if (last_exit_status >= 0) {
            // It is an exit (exit:N)
            write(STDOUT_FILENO, "exit:", 5);
            value_to_convert = last_exit_status;
        } else {
            // It is a signal (sign:N). We use the positive value
            write(STDOUT_FILENO, "sign:", 5);
            value_to_convert = -last_exit_status;
        }

        // 3. Conversion in string
        status_len = int_to_string(value_to_convert, status_str);
        time_len = int_to_string(ms, time_str);

        // Writing in the shell
        write(STDOUT_FILENO, status_str, status_len);

        write(STDOUT_FILENO, "|time:", 6);

        write(STDOUT_FILENO, time_str, time_len);

        write(STDOUT_FILENO, "ms] % ", 6);

        nbytes = read(STDIN_FILENO, buffer, MAX_COMMANDE_LENGTH); // Reading the command

        //Execution of a command wtih several parameters
        buffer[strcspn(buffer, "\n")] = '\0';

        int arg_count = 0;
        char *argv_start = buffer;
        char *argv_end;

        memset (argv, 0, sizeof(argv)); // argv becomes empty

        while ((argv_end = strchr(argv_start, ' ')) != NULL && arg_count < 10){
            *argv_end = '\0'; // On coupe la chaîne ici en remplaçant l'espace par \0
        
            if (*argv_start != '\0') { // On évite d'ajouter des arguments vides (si doubles espaces)
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
        int status;

        //Measure of the time of a execution
        clock_gettime(CLOCK_MONOTONIC, &start);

        pid = fork();
        if (pid < 0) {
            perror("enseash: fork error");
            
        } else if (pid == 0) {
            // child process
            
            //execlp(buffer, buffer, (char *) NULL); // the output of this function is an int

            /*execlp("sh", "sh", "-c", buffer, (char *) NULL);  
            /!\ adding some parramettre to be able to use more than 
            1 parameter in the command but use a system call*/

            execvp(argv[0], argv);


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
            if (WIFEXITED(status)){
                last_exit_status = WEXITSTATUS(status);              
            } else if (WIFSIGNALED(status)){
                last_exit_status = - WTERMSIG(status); 
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