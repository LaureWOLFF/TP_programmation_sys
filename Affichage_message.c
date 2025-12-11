#include <sys/types.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <string.h>
#include <unistd.h>


#define MAX_COMMANDE_LENGTH 128
#define MAX_ARGS 2 

int main (){
    char buffer[MAX_COMMANDE_LENGTH];
    ssize_t nbytes; 

    const char *welcom_message = "$./enseash \n Bienvenue dans le Shell ENSEA. \n Pour quitter, taper 'exit'. \n";
    write(STDOUT_FILENO, welcom_message, strnlen(welcom_message, 128));
    write (STDOUT_FILENO, "enseash % \n", 10);

    while (1){
        
    }

    return 0;
}