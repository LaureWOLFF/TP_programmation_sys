# TP_programmation_sys
This GitHub reposity is my report of the lab works in the system programation course

## Compte-rendu séance 1
### Question 1 : Création du terminal ENSEA Shell

On définit un tableau de caractères (buffer) de 128 octets. Cela correspondra à la taille maximale d'un prompt que l'on peut écrire dans le terminal. Puis, on utilise la fonction write afin d'afficher les messages. La fonction write est une fonction essentielle dans ce projet. En effet, elle permet d'afficher des tableaux de caractères dans le terminal.

### Question 2 : REPL

Étape 1 : Lire une commande 

La fonction read permet de récupérer la commande écrite dans le terminal et de déterminer sa longueur.

Étape 2 : Exécuter la commande 

Pour exécuter une fonction, il faut commencer par scinder un processus en deux. Dans le processus enfant, on exécute la commande qui est contenue dans le buffer. Attention : La commande exec se décline en plusieurs variantes. En fonction de l'ajout de deux autres lettres, la fonction va nécessiter différents paramètres. Dans notre cas, on a choisi la fonction execlp qui ne demande que le fichier (et pas le chemin) ainsi que les différents arguments (ici le buffer contenant la commande et rien d'autre, d'où le (char *) NULL). Cependant, cela a posé des problèmes lors de la compilation. Pour les résoudre, il ne faut pas oublier d'enlever le dernier caractère du buffer : \n. Ce dernier caractère rend impossible la compréhension d'une commande par le noyau et, lors de l'exécution du terminal, on se retrouve avec uniquement des commandes dites inconnues par le terminal. Remarque : Le choix de la fonction execlp permet de faire fonctionner le shell lorsqu'il n'y a aucun argument dans la commande, mais cela pourra poser problème plus tard.

Étape 3 : La boucle 

Pour que le terminal soit fonctionnel, il faut mettre dans une boucle while infinie, au minimum, la lecture de la commande et son exécution. Ainsi, on peut entrer différentes commandes les unes à la suite des autres et avoir un terminal fonctionnel.

### Question 3 : La sortie du terminal

Premier moyen de sortie : exit 

Pour pouvoir sortir du terminal quand on tape exit, on crée une boucle if avec comme test : si la commande contenue dans le buffer vaut "exit", alors on sort de la boucle infinie et le main retourne 0 (équivalent à un EXIT_SUCCESS). De plus, comme pour la question 1, on crée un message fixe que l'on envoie à l'utilisateur grâce à la fonction write.

Second moyen : le contrôle D 

Ce moyen est un peu plus délicat à mettre en place car, premièrement, il faut comprendre quelle est l'action du contrôle D dans un terminal. Dans un terminal Linux, le contrôle D envoie le signal EOF (End Of File) au programme en cours d'exécution. Cela revient à faire le test que la taille du buffer est nulle. Dans ce cas-là, on envoie un nouveau message à l'utilisateur puis on sort de la boucle infinie. Remarque : Quand on ne fait qu'appuyer sur la touche Entrée, cela ne déclenche pas l'interruption car le buffer est alors composé de \n\n ; donc, même après le retrait d'un \n, nbyte ne sera pas nul.

### Question 4 : Gestion des signaux

Dans cette partie, il faut premièrement modifier le prompt en début de chaque ligne et réussir à capter le signal transmis entre les différents processus.

Récupération des signaux On travaille dans le processus père et on reçoit les informations du processus fils. Les fonctions principales que l'on va utiliser sont wait, WIFEXITED, WEXITSTATUS, WIFSIGNALED et WTERMSIG.

Modification du prompt Le prompt peut poser problème. J'ai eu une erreur de segmentation (core fault). Après avoir posé plusieurs write comme témoins pour être sûr que le code fonctionne, on a remarqué que cela venait de la définition de prompt que j'avais instancié comme un simple caractère alors qu'il fallait que ce soit un tableau. Après cela, je n'ai plus eu d'erreur. Cependant, pour le moment, le code ne semble pas exécuter la boucle traitant des retours de fonction ou les signaux.

## Compte-rendu de la seance 2

### Retour sur la question 4: gestion des signaux

Dans le fichier signal1, la gestion des signaux semble fonctionner correctement. En effet, l'implémentation pour les sorties et les signaux internes fonctionne de la même manière. Dans ENSEA Shell, on a bien que l'exit vaut 0 si la commande est correctement exécutée/exécutable, sinon on a un exit qui vaut 1 en cas d'erreur générale (retour du EXIT_FAILURE de l'exécution). Il est aussi arrivé d'avoir l'exit qui vaut 127, cela correspond à une commande introuvable.

 Remarque et attention: 
 
Pour réussir à visualiser un signal, il faut lancer un processus prenant du temps tel que sleep 100 et, dans un terminal, encore dans enseash, le tuer avec pkill -9 ou une autre fonction. Cependant, cela ne marchait pas. De plus, pour pouvoir appeler l'instruction sleep, il faut forcément un second argument. Pour cela, j'ai modifié mon exécution en l'écrivant ainsi : execlp("sh", "sh", "-c", buffer, NULL). ATTENTION, dans cette ligne je fais un appel système, ce qui n'est pas sécurisé et n'est pas une pratique de code recommandée car on délaisse le traitement de l'instruction au terminal Linux lui-même.

### Question 5: Le temps d'éxecution

Pour pouvoir calculer la durée d'une exécution, on utilise la fonction clock_gettime() de la librairie time.h. Cette fonction est plus précise que la fonction time normale, pouvant aller jusqu'à la nanoseconde. gettime prend en arguments deux paramètres : le premier paramètre est une constante définissant le type de clock. Ici on utilise CLOCK_MONOTONIC, c'est-à-dire un point de départ qui sera toujours le même pour toutes les itérations du programme, peu importe la configuration de l'appareil ; elle permet de mesurer un intervalle de temps. Le second paramètre est une structure de données contenant le temps en secondes et celui en nanosecondes. Afin de pouvoir afficher le temps en millisecondes, on utilise la relation suivante :
$$durée ms = (fin en seconde - debut en seconde) * 1000 + (fin en nanoseconde - début en nanoseconde)/1000000$$
Cela permet de convertir en millisecondes le temps d'exécution de chaque instruction.

Pour la visualisation du temps sur la console, on a utilisé la même méthode que pour afficher les signaux et les sorties. C'est-à-dire convertir le nombre en chaîne de caractères et le rendre visible par un write.

Remarque: 

Quand on exécute une commande ouvrant une autre page telle que l'instruction man, le temps augmente et cela renvoie le temps passé sur cette instruction. Cela prouve que le temps d'exécution est correctement implémenté et fonctionnel.

### Question 6:  Une commande complexe (sans un system call)
Dans les questions précédentes, on a utilisé un "sys call". Cependant, cela n'est pas correct, donc on va essayer de trouver une autre manière de traiter et d'exécuter une instruction complexe.

Etape 1: Modification du buffer et création d'un tableau d'argument argv (v pour vecteurs)

Avec une commande complexe, le buffer devient également plus complexe, en effet il y a des espace, plusieurs Avec une commande complexe, le buffer devient également plus complexe ; en effet, il y a des espaces, plusieurs instructions/paramètres, donc il faut pouvoir construire un tableau pouvant prendre un nombre aléatoire (inférieur à une limite qu'on a posée au début à 10 puis 64), le argv. 

Initialement, il faut toujours un retour chariot à la fin d'une séance d'instructions, donc comme dans la question 2, il faut l'enlever. Cependant, on a choisi une autre méthode : buffer[strcspn(buffer, "\n")] = '\0';. La fonction strcspn permet de renvoyer l'indice du buffer où se trouve le caractère \n, autrement dit à la fin de l'instruction car il est généré par la touche entrée. Bien que la première instruction soit toujours valable, elle est moins précise et, en cas d'une mauvaise instruction/manipulation dans le code, il est possible que buffer[nbytes - 1] pointe nulle part (si le buffer devient nul avant).

Une fois qu'on a une chaîne de caractères sans \n, on peut essayer de découper le buffer pour créer notre tableau de paramètres qu'on appelle argv. Pour cela, on introduit quelques variables pour le construire :
- Un entier : initialisé à 0, il permet de compter la longueur de ce tableau (le nombre de paramètres) et servira d'indice dans ce tableau lors de sa construction.
- Deux pointeurs : l'un initialisé sur le début du buffer et un autre, pour l'instant vide, mais qui pointera vers la fin de l'argument.

On réalise une boucle while pour repérer les espaces afin de délimiter un argument, et on initialise argv_end au niveau de l'espace et on le remplace par \0. En parcourant cette partie du buffer (entre argv_start et argv_end, et tant que argv_start ne voit pas \0, l'ancien espace), on copie l'adresse pointée par argv_start dans le tableau dynamique grâce à l'indice argv_count. Puis on modifie argv_start pour qu'il pointe sur l'adresse suivante de celle de la fin de cette instruction, à savoir argv_end.

Attention, cela permet de gérer tous les paramètres ayant un espace à la fin, donc il faut gérer le dernier paramètre hors de cette boucle. Pour cela, on vérifie que le contenu de argv_start ne vaut pas \0 (remplacement du retour chariot) et que la limite allouée au tableau n'est pas déjà atteinte (c'est pour cela qu'on a pris un nombre d'arguments limite important). Puis on procède de la même manière. Attention, à la fin il faut ajouter NULL à la fin du tableau pour pouvoir utiliser la fonction d'exécution execvp.

Etape 2: Changement de la fonction d'execution 

La fonction execlp n'est pas recommandée car elle traite uniquement une liste de paramètres donnés. Ce qui était faisable avec un unique argument (le nom de la commande était également le seul paramètre) n'est maintenant plus possible. Pour remédier à cela, il faut utiliser la fonction execvp qui permet une gestion dynamique des paramètres. Elle prend en argument un tableau de pointeurs et permet de s'adapter aux différentes commandes que l'utilisateur va entrer.

Premier test:

Lors du premier test, on a réussi à effectuer plusieurs instructions simples, mais seulement une seule complexe.

Rectification: 

Ce bug vient du fait qu'on n'a pas réinitialisé le tableau argv à chaque boucle, donc cela bloquait le shell et ressortait une erreur d'exécution car la combinaison des différentes instructions était inconnue pour le noyau Linux. Pour résoudre cela, on ajoute une ligne juste avant la création du tableau : memset(argv, 0, sizeof(argv));. Cette fonction permet de remettre à zéro le tableau argv.

### Queston 7: Gestion de la redirection STDIN et STDOUT

Le but de cette question est de pouvoir créer un fichier et de le manipuler avec des commandes et les signes > ou < afin d'écrire dedans (STDOUT) ou de le lire (STDIN). Cette question revient à faire du traitement de fichier. Il faut dans un premier temps repérer ces symboles puis, dans le processus, orienter le code afin d'exécuter ce que l'utilisateur attend, ce qui diffère en fonction de STDOUT et STDIN. Pour cette gestion de fichier, on va créer des tableaux de pointeurs, initialisés sur rien au début:
- input_file : c'est pour la gestion de STDIN, on prend les éléments du fichier comme instructions.
- output_file : c'est pour la gestion de STDOUT, on utilise un fichier (ou on le crée, mais c'est un peu plus compliqué avec les permissions) pour y écrire les instructions que l'utilisateur a demandées (fait office de terminal).
  
Etape 1: Reperer un STDIN ou STDOUT

Le premier réflexe a été de chercher les symboles de STDIN et STDOUT afin d'identifier les instructions particulières et de pouvoir initialiser les variables qui seront utiles dans l'exécution du processus fils. Premièrement, cette recherche se place dans le code après la modification du buffer où on remplace le retour chariot par \0 et avant la création de argv. Pour trouver ces symboles, on utilise la fonction strchr, elle permet de parcourir le buffer à la recherche d'un caractère précis et renvoie NULL (0) si ce caractère n'est pas dans la liste. Si on a trouvé < ou >, on remplace ce symbole par \0 afin de séparer le buffer, puis on initialise output_file si c'est un STDOUT ou input_file si c'est un STDIN. Pour être sûr que ce pointeur pointe vers le fichier à manipuler, on fait une boucle while pour incrémenter le pointeur jusqu'au nom du fichier.

Etape 2: Modification du processus fils

Dans le processus fils, après le fork(), on vérifie si output_file ou input_file n'est pas nul. 
Si ce n'est pas le cas, on effectue simplement l'exécution de argv, et cela signifie que l'utilisateur souhaite exécuter une fonction "classique", c'est-à-dire avec inscription dans le terminal et sans facteur extérieur (fichier).

  Etape 2': Gestion du STDOUT

  Si output_file n'est pas nul, on crée un entier fd_out, un descripteur du fichier output_file. On indique qu'on souhaite qu'il soit ouvert en écriture seulement et que, s'il n'existe pas, il faut le créer. Cette fonction permet de s'assurer que l'utilisateur a bien les droits pour ouvrir et modifier le fichier. 
  Après avoir ouvert en écriture, on utilise la fonction dup2. Cette fonction permet d'écrire le résultat de la commande tapée par l'utilisateur dans le fichier. Puis il ne faut pas oublier de fermer le "file descriptor" (fd).
  
  Etape 2'': Gestion du STDIN
  
  Si input_file n'est pas nul, on crée un entier fd_in, un descripteur du fichier, comme pour la gestion d'un STDOUT. On indique qu'on souhaite qu'il soit ouvert en lecture seulement. Cette fonction permet de s'assurer que l'utilisateur a bien les droits pour ouvrir et lire le fichier. 
  Après avoir ouvert en lecture, on utilise la fonction dup2. Cette fonction permet de lire le contenu du fichier et d'écrire le résultat de la commande tapée par l'utilisateur dans le terminal. Puis, comme pour la gestion d'un STDOUT, il ne faut pas oublier de fermer le "file descriptor" (fd).
  
Test: 

Lors du premier test, on a une erreur "segmentation fault" au niveau du STDOUT. Pour régler cela, on a placé plusieurs write afin de relever la ligne posant problème. 

Après l'implémentation des processus, on avait un problème avec la gestion STDIN.

Rectification:

Les différentes erreurs que l'on a pu observer (Segmentation core fault ou commande inconnue) sont majoritairement dues à des copier-coller entre les codes de STDIN et STDOUT où l'on a mal modifié les variables, comme dup2(fd_out, STDOUT_FILENO) à la place de dup2(fd_in, STDIN_FILENO). 

Après une relecture rigoureuse, la gestion des fichiers par STDIN et STDOUT fonctionne bien, mais dans le prompt du shell, exit est devenu exexit et on ne comprend pas comment c'est possible et on n'a pas su le résoudre. Mis à part cela, le shell est parfaitement fonctionnel, il traite à la fois les commandes simples, complexes et les gestions STDIN et STDOUT.

Remarque:

L'implémentation de la fonction open est incomplète car, si le fichier n'est pas créé avant, aucune personne n'a la permission de l'ouvrir en écriture/lecture, donc cela crée un bug. Pour résoudre cela, il faut ajouter 0644 (6 : le propriétaire peut écrire et lire, et 4 : le groupe et les autres n'y ont accès qu'en lecture).

### Question 8: Gestion de la redirection 

Le but de cette question est de pouvoir exécuter deux commandes en une ligne et avoir un résultat final, celui qui intéresse l'utilisateur. Pour cela, on crée un "pipe", un tube anonyme entre deux processus. C'est ce qu'on va essayer de réaliser dans cette partie.

Etape 1: Reperer un pipe type

Comme pour la gestion STDIN et STDOUT, le premier réflexe a été de chercher le symbole d'un pipe (|) afin d'identifier les instructions particulières et de pouvoir initialiser les variables qui seront utiles dans l'exécution de cette commande particulière. 

Premièrement, cette recherche se place dans le code après la recherche des STDIN et STDOUT et avant la création de argv. Attention, pour la gestion de cette commande particulière, on ne va plus utiliser le tableau argv mais deux tableaux : un nommé pgrep_argv1 qui va pointer sur l'adresse de la première instruction, et un second tableau, pgrep_argv2, contenant la seconde. 

Pour trouver le symbole, on utilise de nouveau la fonction strchr. Puis, si on a trouvé |, on le remplace par \0. Ensuite, on sépare la commande écrite par l'utilisateur : la première partie est le buffer et la seconde est la commande qui va prendre en argument le résultat de la première instruction. Puis on va construire les deux tableaux pgrep_argv1 et pgrep_argv2 avec la fonction strtok. Cette fonction permet de séparer les éléments d'une liste lorsqu'elle rencontre un délimiteur (espace). 

Pour être sûr que cette partie soit effectuée sans problème, le write permet de s'en assurer.

Etape 2: Modification de l'exécution de la commande

Dans cette question, il faut complètement restructurer l'exécution des commandes. En effet, lorsqu'il y a une instruction avec un pipe, il faut générer deux processus fils au lieu d'un seul. De plus, on souhaite garder le terminal actuel fonctionnel, donc on réalise une boucle if...else où, si on a bien une commande de type pipe, on réalise un code d'exécution de la commande particulier, sinon on garde la même structure.

Si on a un pipe type:

Etape 2': Creéation d'un pipe (un tube anonyme)

Au départ, on crée deux "file descriptors" (en effet, il y en a un pour l'écriture et un pour la lecture). Pour initialiser le tube, on utilise la fonction pipe(). Cela permettra d'établir la connexion entre les deux instructions.

Etape 2": Creation du premier processus fils

On réalise un premier processus fils qui traitera la première instruction. Premièrement, on ferme le "file descriptor" de lecture (donc le premier) car on souhaite écrire (instruction ls) avec la fonction dup2. Elle envoie la sortie de la première instruction vers l'extrémité d'écriture du pipe. On force la commande exécutée à transmettre ses résultats au processus suivant. Attention, après il faut quand même exécuter la commande contenue dans pgrep_argv1 et fermer le "file descriptor" utilisé.

Etape 2"': Creation du second processus fils

Une fois le premier processus fini, on retourne dans le processus père et là, on fait un second fork() créant un second fils qui va traiter la seconde commande, celle de lecture. Ainsi, il faut fermer le second "file descriptor" (écriture) et travailler sur le premier, celui ouvert pour la lecture ; puis, comme pour le premier fils, on applique les mêmes instructions, juste en inversant les rôles des deux "file descriptors" et en exécutant cette fois-ci l'instruction contenue dans pgrep_argv2. 

Attention, dans le dup2, on demande ici à ce que les résultats soient dirigés vers le terminal (ou plutôt que l'entrée vienne du tube), donc on utilise STDIN_FILENO.

Etape 2"": Le processus père

Dans le processus père, on ferme les deux "file descriptors" car ils ne jouent aucun rôle dans le tube au niveau du père. Cependant, c'est dans ce processus qu'on va attendre la fin des deux processus enfants pour savoir s'il y a eu un problème et, si oui, dans lequel des deux.

Test: 

Lors des premiers tests, on avait bien la réalisation d'instructions données par une commande contenant un pipe. Un problème est apparu sur le temps d'exécution qui était aléatoire.

Rectification:

Pour régler ce problème, on a initialisé toutes les variables entrant en jeu dans le temps à 0 à chaque nouvelle entrée dans la boucle. Ainsi, il n'y a plus eu de problèmes.

Remarque:

Dans cette partie, on ne se préoccupe pas des signaux car l'implémentation de l'exécution est déjà très complexe. De plus, pour analyser les signaux, il faudrait prendre en compte les deux processus fils et les synchroniser

Au final, mis à part l'erreur dans le prompt (exexit), on a un terminal fonctionnel qui sait gérer des instructions simples et complexes, mais également des redirections du type STDIN, STDOUT et pipe.
