/**
 * @file main.c
 * @brief Programme d'un jeu de snake autonome
 * @author Dhennin Elouan, Martin Esmeralda
 * @version 1
 * @date 10/12/24
 *
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <termio.h>
#include <time.h>
#include <stdbool.h>
#include <string.h>

// CONSTANTE DU JEU

/*Constante du joueur*/
#define TETE_JOUEUR 'O' // Tête du joueur
#define CORP_JOUEUR 'X' // Corp du joueur
#define POS_INITIAL_JOUEUR_X 40
#define POS_INITIAL_JOUEUR_Y 20
#define LONGEUR_MAX 20
#define TAILLE_INITIAL_DU_JOUEUR 10 // Taille de base du joueur (en réalité 10, car la boucle for commence à 0)
int taille_joueur = 10;

/*Les constantes du jeu*/
#define NB_POMMES 10
#define CACHER_CURSEUR 42 // Valeur absurde pour être sur d'être coller en bas de la console
#define TAILLE_TABLEAU_Y 40
#define TAILLE_TABLEAU_X 80

/*Constantes des éléments graphique hors joueur*/
#define CARACTERE_EFFACER ' ' // Pour effacer un élément
#define MUR '#'
#define POMME '6'
#define VITESSE 200 // micro_sec

#define FERMER_JEU 'a' // Condition d'arrêt

// Définir les types tableau
// + 1 car la dernière valeur sert au tampon du dernier élément,
// pour l'effacer correctementpomme
typedef int corp_longeur[LONGEUR_MAX + 1];
typedef int t_pomme[NB_POMMES];

// Définir le type du tableau à deux dimensions
typedef char type_tableau_2d[TAILLE_TABLEAU_Y][TAILLE_TABLEAU_X];

/* Initialisations des fonctions et des procédure */
int kbhit();						  // équivalent d'un INKEYS en BASIC
int dans_tableau(int eltx, int elty); // Vérifie que l'élément se trouve dans le tableau pour le dessiner
int confirmer_position(int indice, corp_longeur liste, int nouvelle_position);
int collision_avec_pomme(int x_tete, int y_tete, int x_pomme, int y_pomme);
int val_abs(int x);

void teleportation(int *tete_x, int *tete_y);
void goto_x_y(int x, int y);		 // équivalent d'un LOCATE en Basic
void afficher(int x, int y, char c); // Affiche en lien avec goto_x_y un caractère à l'écran
void init_plateau(type_tableau_2d tableau);
void dessiner_serpent(corp_longeur les_x, corp_longeur les_y);
void effacer_serpent(corp_longeur les_x, corp_longeur les_y);
void effacer(int x, int y);
void progresser(corp_longeur les_x, corp_longeur les_y, bool *collision_joueur, int *before_x_apple, int *before_y_apple);
void creation_du_serpent(int x, int y, corp_longeur les_x, corp_longeur les_y);
void enable_echo();
void disable_echo();
void dessiner_plateau(type_tableau_2d plateau);
void precal_path(int pomme_x, int pomme_y, int tete_x, int tete_y, int *before_x_apple, int *before_y_apple);

char lire_entrer();

int main()
{
	system("clear");

	char lettre = CARACTERE_EFFACER; // Valeur du caractère espace

	type_tableau_2d plateau; // Plateau de jeu
	corp_longeur les_x;		 // Position du corps en X
	corp_longeur les_y;		 // Position du corps en Y

	bool collision_joueur = false;

	/*variable lié à la pomme*/
	t_pomme les_pomme_x = {75, 75, 78, 2, 8, 78, 74, 2, 72, 5};
	t_pomme les_pomme_y = {8, 39, 2, 2, 5, 39, 33, 38, 35, 2};
	int nb = 0;
	int pomme_x = les_pomme_x[nb];
	int pomme_y = les_pomme_y[nb];
	int nbMouv = 0;
	bool pomme_ramasser;

	// apparaître à 20 sur le tableau et non sur la console (20 + décalage du tableau dans la console)

	creation_du_serpent(POS_INITIAL_JOUEUR_X + 1, POS_INITIAL_JOUEUR_Y + 1, les_x, les_y);

	init_plateau(plateau);
	dessiner_plateau(plateau);

	disable_echo();

	int before_apple_x = 0;
	int before_apple_y = 0;

	precal_path(pomme_x, pomme_y, les_x[0], les_y[0], &before_apple_x, &before_apple_y);

	afficher(pomme_x, pomme_y, POMME);
	clock_t begin = clock();
	do
	{

		// Lire les entrer au clavier
		lettre = lire_entrer();
		/*
			Cette condition est ici pour éviter d'effacer le bout du serpent
			alors qu'il n'y a eu aucun déplacement à la fin du jeu.
		*/

		progresser(les_x, les_y, &collision_joueur, &before_apple_x, &before_apple_y);
		nbMouv++;

		/*Collision et gestion du jeu avec la pomme*/
		pomme_ramasser = collision_avec_pomme(les_x[0], les_y[0], pomme_x, pomme_y);

		// collision_joueur = dans_tableau(les_x[0], les_y[0]);

		if (pomme_ramasser == true)
		{

			nb++;
			pomme_x = les_pomme_x[nb];
			pomme_y = les_pomme_y[nb];
			
			if (nb == NB_POMMES)
			{
				collision_joueur = true;
			}
			else{
				precal_path(pomme_x, pomme_y, les_x[0], les_y[0], &before_apple_x, &before_apple_y);
				afficher(pomme_x, pomme_y, POMME);
			}
			
		}

		// Si le joueur obtien la dernière pomme, on veux qu'ils l'efface
		// Alors malgré le fait qu'il aie fini, on actualise ça position pour
		// Montrer la pomme manger
		if (collision_joueur != 1 || nb == NB_POMMES)
		{
			effacer_serpent(les_x, les_y);
			dessiner_serpent(les_x, les_y);
			usleep(VITESSE);
		}

		/*
		Le temps de jeu étant dérisoire, nous prenons en compte uniquement le temps
		D'attente entre deux image. Qui est finalement la majorité du temps de jeu
		du joueur. Attendre...
		*/
	} while ((lettre != FERMER_JEU) && nb != NB_POMMES);
	clock_t end = clock();
	double tmpsCPU = ((end - begin) * 1.0) / CLOCKS_PER_SEC;
	enable_echo();

	goto_x_y(1, CACHER_CURSEUR);
	printf("Temps CPU : %f\n", tmpsCPU);
	printf("Nombre de mouvements : %d\n", nbMouv);

	return EXIT_SUCCESS;
}

void precal_path(int pomme_x, int pomme_y, int tete_x, int tete_y, int *before_x_apple, int *before_y_apple)
{
	*before_x_apple = pomme_x - tete_x;
	*before_y_apple = pomme_y - tete_y;

	if (*before_x_apple < 20){
		
	}

}

void afficher(int x, int y, char c)
{
	/**
	 * @brief affiche un caractère à une position donner
	 * @param x position x.
	 * @param y position y.
	 * @param c caractère a afficher aux cordonnées donner.
	 */

	goto_x_y(x, y);
	printf("%c", c);
	goto_x_y(1, CACHER_CURSEUR); // cacher le curseur pour une meilleur lisibilité
}

void init_plateau(type_tableau_2d tableau)
{
	/**
	 * @brief Permet de dessiner le tableau de jeu. Mais aussi l'interface autour !
	 * @param tableau : tableau à deux dimension qui représente le tableau de jeu
	 */

	int lig = 0;
	int col = 0;

	for (lig = 0; lig < TAILLE_TABLEAU_Y; lig++)
	{

		for (col = 0; col < TAILLE_TABLEAU_X; col++)
		{

			if (col == TAILLE_TABLEAU_X / 2 ||
				lig == TAILLE_TABLEAU_Y / 2 ||
				(col == TAILLE_TABLEAU_Y - 1) / 2 ||
				(lig == TAILLE_TABLEAU_Y - 1) / TAILLE_TABLEAU_Y / 2)
			{
				tableau[lig][col] = CARACTERE_EFFACER;
			}
			else if (lig == 0 || col == 0 ||
					 lig == TAILLE_TABLEAU_Y - 1 || col == TAILLE_TABLEAU_X - 1)
			{
				tableau[lig][col] = MUR;
			}

			else
			{
				tableau[lig][col] = CARACTERE_EFFACER;
			}
		}
	}
}

void dessiner_plateau(type_tableau_2d plateau)
{
	/**
	 * @brief va faire apparaître au lancement du jeu les bordure du tableau
	 * @param plateau : tableau à deux dimensions déjà initialiser
	 */
	int i, y;
	for (i = 0; i < TAILLE_TABLEAU_Y; i++)
	{
		for (y = 0; y < TAILLE_TABLEAU_X; y++)
		{
			afficher(y + 1, i + 1, plateau[i][y]);
		}
	}
	fflush(stdout);

	goto_x_y((TAILLE_TABLEAU_X / 2) - 5, TAILLE_TABLEAU_Y + 1); // Centrer le titre
	printf("Snake V1 SAE 1.02");
}

void teleportation(int *tete_x, int *tete_y)
{
	/**
	 * @brief téléporte le joueur d'un coté à l'autre du monde avec les "portail"
	 * @param *tete_x tête du joueur en x en sortie
	 * @param *tete_y tête du joueur en y en sortie
	 */

	if (*tete_x > TAILLE_TABLEAU_X && *tete_y == TAILLE_TABLEAU_Y / 2 + 1)
		*tete_x = 1; // Téléporteur à droite du monde

	else if (*tete_x < 1 && *tete_y == TAILLE_TABLEAU_Y / 2 + 1)
		*tete_x = TAILLE_TABLEAU_X; // Téléporteur à gauche du monde

	else if ((*tete_y > TAILLE_TABLEAU_Y) && (*tete_y == TAILLE_TABLEAU_X / 2 + 1))
		*tete_y = 1; // Téléporteur en bas du monde

	else if ((*tete_y < 1) && (*tete_x == TAILLE_TABLEAU_X / 2 + 1))
		*tete_y = TAILLE_TABLEAU_Y; // Téléporteur en haut du monde
}

void progresser(corp_longeur les_x, corp_longeur les_y, bool *collision_joueur, int *before_apple_x, int *before_apple_y)
{
	/**
	 * @brief Calcule la nouvelle position du serpent quand il avance sans intervention du joueur
	 * @param les_x liste des valeurs en x
	 * @param les_y liste des valeurs en y
	 * @param *collision_joueur vérifie s'il y a une collision 0 si non, 1 si oui
	 */

	if (*collision_joueur == false)
	{

		int i = 0;
		int indicator = 0;

		les_x[taille_joueur + 1] = les_x[taille_joueur];
		les_y[taille_joueur + 1] = les_y[taille_joueur];

		for (i = taille_joueur; i >= 1; i--)
		{
			les_x[i] = les_x[i - 1];
			les_y[i] = les_y[i - 1];
		}

		if (*before_apple_x != 0)
		{
			indicator = ((*before_apple_x) < 0) ? -1 : 1;
			les_x[0] = les_x[0] + indicator;
			(*before_apple_x) = (*before_apple_x) - indicator;
		}
		else
		{
			indicator = ((*before_apple_y) < 0) ? -1 : 1;
			les_y[0] = les_y[0] + indicator;
			(*before_apple_y) = (*before_apple_y) - indicator;
		}

		// teleportation(&les_x[0], &les_y[0]);
	}
}

int dans_tableau(int eltx, int elty)
{
	/**
	 * @brief va calculer si un éléments se trouve dans le tableau
	 * @param eltx : position de l'élément en x
	 * @param elty : position de l'élément en y
	 * @return 1 si l'élément est dans le tableaun. 0 S'il se trouve en dehors
	 */
	int dedans = 1;
	// Gestion des téléporteur au quatre coin du niveau
	if (((eltx >= TAILLE_TABLEAU_X) && (elty == TAILLE_TABLEAU_Y / 2 + 1)) 
		|| ((eltx <= 1) && (elty == TAILLE_TABLEAU_Y / 2 + 1)) 
		|| ((elty >= TAILLE_TABLEAU_Y) && (eltx == TAILLE_TABLEAU_X / 2 + 1)) 
		|| ((elty <= 1) && (eltx == TAILLE_TABLEAU_X / 2 + 1))
	   )
	{
		dedans = 0;
	}
	else if ((eltx < TAILLE_TABLEAU_X	 // Si le joueur touche le mur droit
			  && eltx >= 2)				 // Si le joueur touche le mur gauche
			 && (elty < TAILLE_TABLEAU_Y // Si le joueur touche le mur bas
				 && elty >= 2))			 // Si le joueur touche le mur haut
	{
		dedans = 0;
	}
	return dedans;
}

void dessiner_serpent(corp_longeur les_x, corp_longeur les_y)
{
	/**
	 * @brief Dessine le joueur en premier, et fait une boucle pour dessiner les enfants
	 * (le corps) du joueur en fonction de la taille du joueur.
	 * @param les_x liste des valeurs en x
	 * @param les_y liste des valeurs en y
	 */

	int i;
	for (i = 1; i < taille_joueur; i++)
	{

		afficher(les_x[i], les_y[i], CORP_JOUEUR);
	}

	afficher(les_x[0], les_y[0], TETE_JOUEUR);

	fflush(stdout);
}

void effacer(int x, int y)
{
	/**
	 * @brief effacer un caractère à une position voulu
	 * @param x position x
	 * @param y position y
	 */
	afficher(x, y, CARACTERE_EFFACER);
	fflush(stdout);
}

void effacer_serpent(corp_longeur les_x, corp_longeur les_y)
{
	/**
	 * @brief Va de la fin jusqu'à la tête du joueur pour remplacer avec un caractère vide
	 * @param les_x liste des valeurs en x
	 * @param les_y liste des valeurs en y
	 */

	// Efface uniquement le dernier éléments du joueura
	effacer(les_x[taille_joueur], les_y[taille_joueur]);
}

char lire_entrer()
{
	/**
	 * @brief va lire les entrer de l'utilisateur
	 * @return retourne la lettre qui a été entrer par l'utilisateur
	 */

	int entrer;
	char lettre = CARACTERE_EFFACER;
	entrer = kbhit();
	if (entrer == 1)
	{						// Vérifie à chaque "tour de jeu" si quelque chose est rentré
		lettre = getchar(); // Va regarder dans le buffer la lettre à rechercher
	}
	return lettre;
}

void creation_du_serpent(int x, int y, corp_longeur les_x, corp_longeur les_y)
{
	/**
	 * @brief Fonction qui viens crée le serpent la premièrer fois.
	 * @param x position initial en x
	 * @param y position initial en y
	 * @param les_x tableau des position en x
	 * @param les_y tableau des position en y
	 *
	 */
	int i = 0;

	for (i = 0; i < taille_joueur; i++)
	{
		les_x[i] = x - i;
		les_y[i] = y;
	}
}

int collision_avec_pomme(int x_tete, int y_tete, int x_pomme, int y_pomme)
{
	/**
	 * @brief véfifie si le joueur est en collision avec la pomme
	 * @param x_tete tête du joueur en x
	 * @param y_tete tête du joueur en y
	 * @param x_pomme position de la pomme en x
	 * @param y_pomme position de la pomme en y
	 * @return
	 */

	int collision = 0;
	if ((x_tete == x_pomme) && (y_tete == y_pomme))
	{
		collision = 1;
	}
	return collision;
}

/*		FONCTION DONNER PAR LES INSTRUCTIONS       */

int kbhit()
{
	// la fonction retourne :
	// 1 si un caractere est present
	// 0 si pas de caractere present

	int un_caractere = 0;
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

	if (ch != EOF)
	{
		ungetc(ch, stdin);
		un_caractere = 1;
	}
	return un_caractere;
}

void goto_x_y(int x, int y)
{
	/**
	 * @brief Fonction qui déplace le curseur dans la console
	 * @param x position x.
	 * @param y position y.
	 */
	printf("\033[%d;%df", y, x);
}

void disable_echo()
{
	struct termios tty;

	// Obtenir les attributs du terminal
	if (tcgetattr(STDIN_FILENO, &tty) == -1)
	{
		perror("tcgetattr");
		exit(EXIT_FAILURE);
	}

	// Desactiver le flag ECHO
	tty.c_lflag &= ~ECHO;

	// Appliquer les nouvelles configurations
	if (tcsetattr(STDIN_FILENO, TCSANOW, &tty) == -1)
	{
		perror("tcsetattr");
		exit(EXIT_FAILURE);
	}
}

void enable_echo()
{
	struct termios tty;

	// Obtenir les attributs du terminal
	if (tcgetattr(STDIN_FILENO, &tty) == -1)
	{
		perror("tcgetattr");
		exit(EXIT_FAILURE);
	}

	// Reactiver le flag ECHO
	tty.c_lflag |= ECHO;

	// Appliquer les nouvelles configurations
	if (tcsetattr(STDIN_FILENO, TCSANOW, &tty) == -1)
	{
		perror("tcsetattr");
		exit(EXIT_FAILURE);
	}
}
