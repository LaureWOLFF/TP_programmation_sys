# TP_programmation_sys
This GitHub reposity is my report of the lab works in the system programation course

## Compte-rendu séance 1

### Question 1: Création du terminal ENSEA Shell

On définit un tableau de caratères (buffer) de  128 octets. Cela correspondra à la taille maximale d'un prompt qu'on peut écrire dans le terminal. Puis on utilise la fonction write afin d'afficher les messsages.

La fonction write est une fonction essentielle dans ce projet. En effet la fonction write permet d'afficher des tableaux de charactères  dans le terminal. 


### Question 2: REPL
#### Etape 1: Lire une commande 

La fonction read permet de recupérer la commande écrit dans le terminal et de déterminer sa longueur

#### Etape 2: Executer la commande

Pour executer un fonction il faut commencer par scinder un processus en deux. Dans le processus enfant on execute la commande qui est contenu dans le buffer.

Attention: La commande exec se decline en plusieurs différentes variantes. En effet en fonction de l'ajout de deux autre lettres la fonction va nécessiter différents paramètres.

Dans notre cas on a choisi la fonction execlp qui demande que le fichier et pas le chemen et les differents arguments (ici buffer contenant la commande et rien d'autre d'où le (char *) NULL). Cependan cela a poser des problèmes lorsqu'on compiler. Pour le resoudre il ne faut pas oublié d'enlever le dernier caratères du buffer /n.Ce dernier caractère rends impossible la comprehension d'une commade du noyaux et lors de l'exectuion du terminal on se retourouve avec que des commandes dites inconnnue par le terminal.

Remarque: le choix de la fonction execlp permet de faire fonctionner le shell lorsqu'il  n'y a aucun argument dans la commande. Mais cela pourrra poser problème plus tard.

#### Etape 3: la boucle

Pour que le termibnal soit fonctionnel il faut mettre dans une boucle while infinie, au minimum, la lecture de la commande et son exécution. Ainsi on peux entrer différente commande les une à la suite des autres et avoir un terminal fonctionnel.

### Question 3 : La sortie du terminal
#### Premier moyen de sortie: exit

Pour pouvoir sortie du teminal quand on tape exit, on crée une boucle if avec comme test: si la commade contenue dans le buffer vaux "exit" alors on sort de la boucle infinie et le main reourne 0 (équivalent à un EXIT_SUCCESS). Deplus comme pour la question 1 on crée un message fixe que l'on envoit à l'utilisatuer grace à la fonction write.

#### Second moyen: le controle D

Ce moyen est un peu plus délicat à mettre en place car premièrement il faut compprendre qu'elle est l'action du controle D dans un terminale . Dans un terminale Linux, le controle D envoie le signal EOF (End Of File) au programme en cours d'exécution. Cela reviens à faile le test que la taille du buffer est nul. Et dans ce cas la on envoie un nouveau message à l'utilisatuer puis on soit de la boucle infinie.

Remarque: Quan on ne fait qu'appuyer sur la touche entrée cela ne declenche pas l'interruption car le buffer est alors composé de \n\n donc même appres le retrait d'un \n nbyte ne sera pas nul. 


### Question 4: Gestion des signaux

Dans cette partie il faut premièrement modifier le prompt en debu de chaque ligne et réussir à capter le signal transmis entre le différent processus.

#### Recuperation de signaux

On travaille dans le processus père et on recoit les information de processus fils. 
les fonctions principales qu'on va utiliser sont wait, WIFEXITED, WEXITSTATUS, WIFSIGNALED et WTERMSIG. 

#### modifacation du prompt

Le promt peux poser problème. J'ai eu une erreur de segmentaion core fault. Après avoir poser plusieur xrite comme temoins pour etre sur que le code fonction, on a reamrque que cela vennait de la definition de prompt que j'avais instantion comme un simple caratères alors qu'ils falliat que cela soit un tableaux. Apres cela je n'ai plus eu d'erreur.

Cependant pour le moment, le code ne semble pas executé la boucle traitant des retours de fonction ou les signaux.

