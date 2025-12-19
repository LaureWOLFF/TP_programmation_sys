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

Dans le fichier signal1, la gestion des signaux semble fonctionner correctement. En effet l'implementation pour les sorties et les signaux intern fonctionnent de la même manière. Dans ENSEA Shell, on a bien que l'exit vaut 0 si la commande est correctement executée/executable sinon on a un exit qui vaut 1  en cas d'erreur genérale (retour du EXIT_FAILURE de l'exécution). Il est aussi arrivé d'avoir l'exit qui vaux 127, cela correspond à une commande introuvable.

#### Remarque et attention: 
Pour réussir à visualiser un signal il faut lancer un processsus prennat du temps tel que sleep 100 et dans sun terminal , encore dans enseash le tuer avec pkill -9 ou une autre fonction. Cepenandant cela ne marcher pas.De plus pour pouvoir appeler l'instruction sleep il faut forcement un second argument. Pour cela j'ai modifier mon execution en l'ecrivant ainis: execlp("sh", "sh", "-c", buffer, NULL). ATTENTION, dans cette ligne je fait un appel système se qui n'est pas securisée et pas une pratique de code recommendé car on delaisse le traitement de l'instruction au terminal Linux lui-même.

### Question 5: Le temps d'éxection

Pour pouvoir calculer la durée d'une exécution on utilise la fonction clock.gettime() de la librairie time.h. Cette fonction est plus précise que la fonction time normale, pouvant aller jusquà la nanoseconde. 
Gettimme prend en aguments deux paramêtres:
Le premier paramêtre est une constatnte definnisant le type de clock. Ici on utilise CLOCK_MONOTONIC, c'est à dire un point de départ qui sera toujours le même, pour toutes les itérations du programme peut import la configuration de l'appareil , elle permet de mesuer un intervalle de temps. Le second paramtètre est une structure de données contennat le temps en seconde et celui en nanoseconde. 
Afin de pouvoir afficher le temps en milliseconde on utilise la relation suivante:
$$durée_ms = (fin en seconde - debut en seconde) /times 1000 + /frac{fin en nanoseconde - début en nanoseconde}{1000000}$$
Cela permit de convertir un milliseconde, le temps d'exécution de chaque intrusction.

Pour la visualisation du temps sur la console, on a utilisé la même methode que pour afficher les signaux et les sorties. C'est à dire convertir le nombre en chaine de charctères et le rendre visible par un write.

#### Remarque: 
quand on execute une commande ouvrant une autre page telle que l'instruction man, le temsp augmente et cela renvoie le temps passé sur cette instruction. Cela prouve que le temps d'exécution est corresctement implémenté et fonctionnel 

### Question 6:  Une commande complexe (sans un system call)
Dans les questions précédente on a utililsé un sys call. Cependant cela n'est pas correct donc on va essayer de trouver une autre manière de traiter et d'exécuter une instruction complexe.

Etape 1: Modification du buffer et création d'un tableau d'argument argv (v pour vecteurs)

Avec une commande complexe, le buffer devient également plus complexe, en effet il y a des espace, plusieurs instruction/paramètres donc il faut pouvoir construire un tableau pouvant prendre un nombre aléatoire (inférieur à une limite qu'on a posé au debut 10 puis 64), le argv.

Initialement,il faut toujours en un  retour chariot à la fin d'une séance d'instruction donc comme dans la question 2 il faut l'enlever. Cependant on a choisi une autre méthode: buffer[strcspn(buffer, "\n")] = '\0';
La fonction strcspn permet de renvoiyer l'indice de buffer où se trouve le caractère \n, autrement dit à la fin de l'instruction car il est généré par la touche entréee. Bien que la première instruction soit toujours valable, elle est moins précise et en cas d'une mauvaise intruction/manipulation dans le code il est possible que buffer[nbytes - 1] pointe nul part (si le buffer devient nul avant).

Une fois qu'on a une chaine de caractère sans \n, on peut essayer de decouper le buffer pour créer notre tableau de paramètre qu'on appelle argv. Pour cela on introduit quelques variables pour le construire: 
- un entier: initilalisé à 0 il permet compter la longueur de ce tableau (le nombre de paramètre) et servira d'indice dans ce tableau lors de sa construction
- deux pointeurs: l'un initialisé sur le debut de buffer et un autre pour l'instant vide mais qui pointera vers la fin de l'argument.

On réalise un boucle while pour reperer les espaces afin de délimiter un argument. et on initialise argv_end au niveau de l'espace et on le remplace par \0.
En parcourant cette partie du buffer (entre argv_start et argv_end et tant que argv-start ne voit pas \0, l'ancien espace) on copit l'adresse pointé par argv_start dans le tableau dynamiqe grace à l'indice argv-count. Puis on modifie argv_start pour qu'il point sur l'addres suivante que celle de la fin de cette instruction à savoir argv_end.

Attention, cella permet tous les paramètres ayant un espace à la fin donc il faut gérer le dernier paramètre hors de cette boucle. Pour cela on verifie que le contenu de argv_start ne vaut pas \0 (remplacement du retour chariot) et que la limite allouer au tableau n'est pas déja atteinte (c'est pour cela qu'on a pris un nombre d'arguments limite important). Puis on proceède de la mème manière. 

Attention, à la fin il faut ajouter NULL à la fin du tableau pour pouvoir utiliser la fonction d'execution execvp. 

Etape 2: Changement de la fonction d'execution 

La fonction execlp n'est pas recommandais car cela traite une uniquement une liste de paramètres donnés. Ce qui étati faisable acev un unique argument (nom de la commande etait egalment le seu paramêtre) n'est maintenat pls possible. Pour remedier à cela, il faut utilisé la fonction execvp qui permet une gestion dynamique des parmètre. Elle prend un argument un tableau de pointeurs et permet de s'adapter au differente commande que l'utilisateur va entrer. 

Premier test:

Lors du premier test, on a reussit d'effectuer plusieures instructions simple mais seulement une seule complexe.  

Rectification: 

Ce bug vient du fait qu'on n'a pas réinitialisé le tableau argv à chaque boucle donc cela bloquer le shell et resortait une erreur d'execution car la combinaision des différentes instruction est inconnue pour le noyau linux. Pour resoudre cela, on ah=joute une ligne juste avant la création du tableau: memset (argv, 0, sizeof(argv));

Cette fonction permet de remettre à zéro ce le tableau argv. 

### Queston 7: Gestion de la redirection STDIN et STDOUT
--> parker du syst call 
les instruction d'affichage sont ecrite dans un fichier,
wc (word compte) < tous les elements du ficheir deviennent les objet de la commande 

### Question 8: Gestion de la redirection 



Compte rendu: vendredi 18h30 
bien netoyyer les codes
on peut continuer les questions
expliquer tous les choix d'implement tation (quoi comment, pourquoi fonction et ne fonctionne pas signal et argv, execp) 
