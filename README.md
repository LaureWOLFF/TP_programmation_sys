# TP_programmation_sys
This GitHub reposity is my report of the lab works in the system programation course

## Compte-rendu séance 1
### Question 1 : Création du terminal ENSEA Shell

On définit un tableau de caractères (buffer) de 128 octets. Cela correspondra à la taille maximale d'un prompt que l'on peut écrire dans le terminal. Puis, on utilise la fonction write afin d'afficher les messages. La fonction write est une fonction essentielle dans ce projet. En effet, elle permet d'afficher des tableaux de caractères dans le terminal.

### Question 2 : REPL

Étape 1 : Lire une commande La fonction read permet de récupérer la commande écrite dans le terminal et de déterminer sa longueur.

Étape 2 : Exécuter la commande Pour exécuter une fonction, il faut commencer par scinder un processus en deux. Dans le processus enfant, on exécute la commande qui est contenue dans le buffer. Attention : La commande exec se décline en plusieurs variantes. En fonction de l'ajout de deux autres lettres, la fonction va nécessiter différents paramètres. Dans notre cas, on a choisi la fonction execlp qui ne demande que le fichier (et pas le chemin) ainsi que les différents arguments (ici le buffer contenant la commande et rien d'autre, d'où le (char *) NULL). Cependant, cela a posé des problèmes lors de la compilation. Pour les résoudre, il ne faut pas oublier d'enlever le dernier caractère du buffer : \n. Ce dernier caractère rend impossible la compréhension d'une commande par le noyau et, lors de l'exécution du terminal, on se retrouve avec uniquement des commandes dites inconnues par le terminal. Remarque : Le choix de la fonction execlp permet de faire fonctionner le shell lorsqu'il n'y a aucun argument dans la commande, mais cela pourra poser problème plus tard.

Étape 3 : La boucle Pour que le terminal soit fonctionnel, il faut mettre dans une boucle while infinie, au minimum, la lecture de la commande et son exécution. Ainsi, on peut entrer différentes commandes les unes à la suite des autres et avoir un terminal fonctionnel.

### Question 3 : La sortie du terminal

Premier moyen de sortie : exit Pour pouvoir sortir du terminal quand on tape exit, on crée une boucle if avec comme test : si la commande contenue dans le buffer vaut "exit", alors on sort de la boucle infinie et le main retourne 0 (équivalent à un EXIT_SUCCESS). De plus, comme pour la question 1, on crée un message fixe que l'on envoie à l'utilisateur grâce à la fonction write.

Second moyen : le contrôle D Ce moyen est un peu plus délicat à mettre en place car, premièrement, il faut comprendre quelle est l'action du contrôle D dans un terminal. Dans un terminal Linux, le contrôle D envoie le signal EOF (End Of File) au programme en cours d'exécution. Cela revient à faire le test que la taille du buffer est nulle. Dans ce cas-là, on envoie un nouveau message à l'utilisateur puis on sort de la boucle infinie. Remarque : Quand on ne fait qu'appuyer sur la touche Entrée, cela ne déclenche pas l'interruption car le buffer est alors composé de \n\n ; donc, même après le retrait d'un \n, nbyte ne sera pas nul.

### Question 4 : Gestion des signaux

Dans cette partie, il faut premièrement modifier le prompt en début de chaque ligne et réussir à capter le signal transmis entre les différents processus.

Récupération des signaux On travaille dans le processus père et on reçoit les informations du processus fils. Les fonctions principales que l'on va utiliser sont wait, WIFEXITED, WEXITSTATUS, WIFSIGNALED et WTERMSIG.

Modification du prompt Le prompt peut poser problème. J'ai eu une erreur de segmentation (core fault). Après avoir posé plusieurs write comme témoins pour être sûr que le code fonctionne, on a remarqué que cela venait de la définition de prompt que j'avais instancié comme un simple caractère alors qu'il fallait que ce soit un tableau. Après cela, je n'ai plus eu d'erreur. Cependant, pour le moment, le code ne semble pas exécuter la boucle traitant des retours de fonction ou les signaux.

## Compte-rendu de la seance 2

### Retour sur la question 4: gestion des signaux

### Question 5: Le temps d'éxection

### Question 6:  Une commande complexe (sans un system call)

### Queston 7: Gestion de la redirection STDIN et STDOUT

les instruction d'affichage sont ecrite dans un fichier,
wc (word compte) < tous les elements du ficheir deviennent les objet de la commande 

### Question 8: Gestion de la redirection 



Compte rendu: vendredi 18h30 
bien netoyyer les codes
on peut continuer les questions
expliquer tous les choix d'implement tation (quoi comment, pourquoi fonction et ne fonctionne pas signal et argv, execp) 