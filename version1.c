/* Fichiers inclus */
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <unistd.h>
#include <termios.h>
#include <time.h>
#include <stdbool.h>

/*Constantes symboliques*/

//Directions zqsd
#define HAUT 'z'
#define GAUCHE 'q'
#define BAS 's'
#define DROITE 'd'

//Taille du plateau
#define LARGEUR 81
#define HAUTEUR 41

/* Déclaration des constantes*/
const char TETE = 'O', CORPS = 'X', STOP = 'a', MUR = '#', VIDE = ' ', POMME = '6';
const int TEMPS = 200000;//200000;
const int TAILLE = 10;
const int LIM_MAX = 40;
const int LIM_MIN = 1;
const int DEPART_X = 40;
const int DEPART_Y = 20;
const int MIN_LARGEUR = 1;
const int MIN_HAUTEUR = 1;
#define NB_POMME 10

/*Déclarations variables globales*/
int tailleSerpent = TAILLE;

/*Structures*/
typedef char aireDeJeu[LARGEUR+1][HAUTEUR+1];
typedef int t_pomme[NB_POMME];

typedef struct{
    t_pomme pommeX;
    t_pomme pommeY;
    char apparence;
    int nb;
}stPomme;

/* Variables globales*/
aireDeJeu plateau;  // Déclaration globale de l'aireDeJeu

/* Déclaration des procédures */
void gotoXY(int x, int y);
void afficher(int x, int y, char c);
void effacer(int x, int y);
void dessinerSerpent(int lesX[], int lesY[]);
void progresser(int lesX[], int lesY[], bool *collision , bool *mangePomme, stPomme pomme);
void initPlateau(aireDeJeu plateau, int lesX[], int lesY[]);
void afficherPlateau(aireDeJeu plateau);
void teleportation (int lesX[], int lesY[]);
void ajouterPomme(stPomme *pomme);
void disableEcho();
void enableEcho();

/* Déclaration des fonctions */
bool estObstacle(int x, int y);
int kbhit();

int main()
{
    int tailleSerpent = TAILLE;  // Taille initiale du serpent
    int temps=TEMPS; //Vitesse de délpacement du serpent
    int lesX[TAILLE]; // Tab coordonnées en x
    int lesY[TAILLE]; // Tab coordonnées en y
    int coordX, coordY; // Valeurs des tableaux de coordonnées
    char toucheSaisie = DROITE;	
    bool collision; // Si serpent touche une bordure, lui-même ou un pavé
    bool mangePomme = true;//Si le serpent mange une pomme
	
    stPomme pomme = {
        {75, 75, 78, 2, 8, 78, 74, 2, 72, 5},
        {8, 39, 2, 2, 5, 39, 33, 38, 35, 2}, 
        POMME,
        0};

	//Début programme
    
    coordX = DEPART_X;
    coordY = DEPART_Y;
    collision = false;

    for (int i = 0; i < tailleSerpent; i++) {//Calculer et enregistrer dans les tab les positions de tous les élem du serpent
        lesX[i] = coordX;
        coordX = coordX - 1;
        lesY[i] = coordY;
    }

    system("clear"); // Vide la console
    initPlateau(plateau, lesX, lesY); // Initialise le plateau de jeu
    afficherPlateau(plateau); // Affiche le plateau
    dessinerSerpent(lesX, lesY); // Dessine le serpent
    ajouterPomme(&pomme);// Ajoute une nouvelle pomme
    
    disableEcho();//Enlève les caractères frappés à l'écran pour qu'ils ne soient pas visibles
    clock_t begin = clock();
    while (toucheSaisie != STOP && !collision && pomme.nb != NB_POMME) { // Tant que touche != 'a'  et qu'il n'y a pas de collision et que le serpent n'a pas mangé 10 pommes faire
		
        usleep(temps); // Temporisation
        if (kbhit() == 1) {//Si une touche a été frappée alors
            scanf("%c", &toucheSaisie); // Lire cette touche
        }
		
        progresser(lesX, lesY, &collision, &mangePomme, pomme);// Faire progresser le serpent d'une position vers la direction souhaitée (=calculer sa nouvelle position), vérifie si collision, vérifie si mange pomme

        if (mangePomme)//Si le serpent a mangé une pomme
        {
            pomme.nb ++;

            if(pomme.nb != NB_POMME){
                ajouterPomme(&pomme);// Ajoute une nouvelle pomme
            }
            
            mangePomme = false;  // Réinitialisation de la variable mangePomme
        }
    }
    clock_t end = clock();
    double tmpsCPU = ((end - begin)*1.0) / CLOCKS_PER_SEC;
    printf("temps cpu : %f", tmpsCPU);   
    enableEcho();//Rend à nouveau visibles les caractères frappés à l'écran
    printf("\n");

    return EXIT_SUCCESS;
}

void gotoXY(int x, int y)
{
	//Procedure donnée sur moodle
	/**
	* @brief procédure gotoXY, permet de se placer a un certain endroit de la console
	* @param x de type entier, entrée : coordonées en x
	* @param y de type entier, entrée : coordonées en y
	*/
    printf("\033[%d;%df", y, x);
}

void afficher(int x, int y, char c)
{
	/**
	* @brief procédure afficher, affiche le caractère c à la position (x, y), où x représente le numéro de colonne
    et y le numéro de ligne.
	* @param x de type entier, entrée : coordonées en x
	* @param y de type entier, entrée : coordonées en y
	* @param c de type caractere, entrée : caractère a afficher
	*/
    gotoXY(x,y);
    printf("%c",c);
    gotoXY(LIM_MIN,LIM_MAX);//Place le curseur hors du serpent
}

void effacer(int x, int y)
{
    /**
	* @brief procédure effacer, affiche un espace à la position (x, y), où x représente le numéro de colonne et y
    le numéro de ligne.
	* @param x de type entier, entrée : coordonées en x
	* @param y de type entier, entrée : coordonées en y
	*/
    gotoXY(x,y);
    printf(" ");
}

void dessinerSerpent(int lesX[], int lesY[])
{
    
    /**
	* @brief procédure dessinerSerpent, affiche à l’écran un à un les éléments du serpent dont les coordonnées
    sont fournies dans le(s) tableau(x) en paramètre(s).
	* @param lesX tableau de type entier, entrée : coordonées en x
	* @param lesY tableau de type entier, entrée : coordonées en y
	*/
    for (int i = 0; i < TAILLE; i++)
    {
		if ((lesX[i] >= LIM_MIN) && (lesY[i] >= LIM_MIN))//Affiche le serpent seulement s'il est dans la console
		{
			if (i==0)
			{
				afficher(lesX[i],lesY[i],TETE);//Affiche la tete du serpent
			}
			else
			{
				afficher(lesX[i],lesY[i],CORPS);//Affiche le corps du serpent
			}
		}
    }
    
}

bool estObstacle(int x, int y)
{	
	/**
	* @brief fonction estObstacle, permet de savoir si un caractère '#' est présent aux coordonnée x,y
	* @param x de type entier, entrée : coordonées en x
	* @param y de type entier, entrée : coordonées en y
	* @return true ou false si caractère présent ou non
	*/
    return plateau[x][y] == MUR;  // Renvoie true si il y a un '#' à la case x,y du plateau
}

void progresser(int lesX[], int lesY[], bool *collision, bool *mangePomme, stPomme pomme)
{	
    /**
	* @brief procédure progresser, calcule et affiche la prochaine position du serpent. 
	* Par exemple, si le serpent a sa tête en (12, 3), la procédure doit faire en sorte que 
	* le serpent évolue d’une position vers la droite et donc l’affiche avec la tête en position (13, 3)
	* @param lesX tableau de type entier, entrée : coordonnées en x
	* @param lesY tableau de type entier, entrée : coordonnées en y
	* @param direction de type caractère, entrée : direction du serpent 'z' pour haut, 's' pour bas, 'q' pour gauche, 'd' pour droite
	* @param collision de type booléen, sortie : true ou false si le serpent arrive sur une bordure, un obstacle ou lui-même
	* @param mangePomme de type booléen, sortie : true ou false si le serpent mange une pomme
	*/
    
    // Effacer la position actuelle du serpent
    for (int i = 0; i < tailleSerpent; i++)
    {
        effacer(lesX[i], lesY[i]);
    }


    int tampon = lesX[0];
    if(lesX[0] != pomme.pommeX[pomme.nb]){
        lesX[0] = (lesX[0] <= pomme.pommeX[pomme.nb]) ? lesX[0] + 1 : lesX[0] - 1;
    }
    else if (tampon == lesX[0]){
        if(lesY[0] != pomme.pommeY[pomme.nb]){
            lesY[0] = (lesY[0] <= pomme.pommeY[pomme.nb]) ? lesY[0] + 1 : lesY[0] - 1;
        }
    }

    // Vérifie si le serpent mange une pomme
    if (lesX[0]==pomme.pommeX[pomme.nb] && lesY[0] == pomme.pommeY[pomme.nb] )
    {
        *mangePomme = true;
    }
    else
    {
        *mangePomme = false;//Renvoie false si pomme non mangée
    }

    if(pomme.nb < NB_POMME || *mangePomme == true){
        //Décale les valeurs des tableaux d'une case pour permettre à la tête d'avoir ses nouvelles coordonnées
        for (int i = tailleSerpent; i >= 1; i--) {
            lesX[i] = lesX[i - 1];
            lesY[i] = lesY[i - 1];
        }
    }


    // Vérifie si collision : si la prochaine position de la tête est un obstacle
    if (estObstacle(lesX[0], lesY[0])) {
        *collision = true;  // Collision
    }
    else
    {
        *collision = false; // Pas de collision
    }

    // Redessine le serpent à sa nouvelle position
    dessinerSerpent(lesX, lesY);
}


void afficherPlateau(aireDeJeu plateau)
{
	/**
	* @brief procédure afficherPlateau, affiche tout le plateau et ses bords
	* @param plateau tableau de type aireDeJeu, entrée : éléments à placer sur la console
	*/
    for (int i = 0; i < LARGEUR; i++)
    {
        for (int j = 0; j < HAUTEUR; j++)
        {
            afficher(i, j, plateau[i][j]);  // Afficher chaque case du plateau
        }
        printf("\n");//Retour à la ligne après avoir fini une ligne
    }
}

void initPlateau(aireDeJeu plateau, int lesX[], int lesY[])
{
	/**
	* @brief procédure initPlateau, initialise tout le plateau et ses bords
	* @param plateau tableau de type aireDeJeu, entrée : éléments à placer sur la console
	* @param lesX tableau de type entier, entrée : coordonées en x
	* @param lesY tableau de type entier, entrée : coordonées en y
	*/ 
    // Initialise les bords du plateau
    for (int i = 0; i < LARGEUR; i++)
    {
        for (int j = 0; j < HAUTEUR; j++)
        {
            if (i == MIN_LARGEUR || i == LARGEUR - 1 || j == MIN_HAUTEUR || j == HAUTEUR - 1) {
                plateau[i][j] = MUR;  //Remplir les bords du plateau avec des '#'
            } else {
                plateau[i][j] = VIDE;  //Remplir l'intérieur du plateau avec des ' '
            }
        }
    }
    
}

void ajouterPomme(stPomme *pomme) 
{
    /**
	* @brief procédure ajouterPomme, ajoute une pomme sur le plateau
	* @param plateau tableau de type aireDeJeu, entrée : éléments à placer sur la console
	* @param lesX tableau de type entier, entrée : coordonnées en x du serpent
	* @param lesY tableau de type entier, entrée : coordonnées en y du serpent
	* @param tailleSerpent de type entier, entrée : taille actuelle du serpent
	*/ 
    afficher(pomme->pommeX[pomme->nb],pomme->pommeY[pomme->nb],pomme->apparence);//Affiche la pomme sur le plateau
}


void disableEcho()
{
	//Procédure donnée sur moodle
	/**
	* @brief procédure disableEcho, pour éviter que les caractères utilisés pour diriger le serpent s’affichent à l’écran
	*/
    struct termios tty;

    // Obtenir les attributs du terminal
    if (tcgetattr(STDIN_FILENO, &tty) == -1) {
        perror("tcgetattr");
        exit(EXIT_FAILURE);
    }

    // Desactiver le flag ECHO
    tty.c_lflag &= ~ECHO;

    // Appliquer les nouvelles configurations
    if (tcsetattr(STDIN_FILENO, TCSANOW, &tty) == -1) {
        perror("tcsetattr");
        exit(EXIT_FAILURE);
    }
}

void enableEcho() 
{	
	//Procédure donnée sur moodle
	/**
	* @brief procédure enableEcho, pour afficher à nouveau les caractères sur la console
	*/
    struct termios tty;

    // Obtenir les attributs du terminal
    if (tcgetattr(STDIN_FILENO, &tty) == -1) {
        perror("tcgetattr");
        exit(EXIT_FAILURE);
    }

    // Reactiver le flag ECHO
    tty.c_lflag |= ECHO;

    // Appliquer les nouvelles configurations
    if (tcsetattr(STDIN_FILENO, TCSANOW, &tty) == -1) {
        perror("tcsetattr");
        exit(EXIT_FAILURE);
    }
}

int kbhit()
{	
	/**
	* @brief fonction kbhit, permet de savoir si une touche a été frappée sur le clavier
	* @return unCaractere, 1 si un caractere est present, 0 si pas de caractere present
	*/
	//Fonction donnée sur moodle
	// la fonction retourne :
	// 1 si un caractere est present
	// 0 si pas de caractere present
	
	int unCaractere=0;
	struct termios oldt, newt;
	int ch;
	int oldf;

	// mettre le terminal en mode non bloquant
	tcgetattr(STDIN_FILENO, &oldt);
	newt = oldt;
	newt.c_lflag &= ~(ICANON | ECHO);
	tcsetattr(STDIN_FILENO, TCSANOW, &newt);
	oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
	fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);
 
	ch = getchar();

	// restaurer le mode du terminal
	tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
	fcntl(STDIN_FILENO, F_SETFL, oldf);
 
	if(ch != EOF){
		ungetc(ch, stdin);
		unCaractere=1;
	} 
	return unCaractere;
}
