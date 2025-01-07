/**
 * @file main.c
 * @brief Programme d'un jeu de snake autonome
 * @author Dhennin Elouan, Martin Esmeralda
 * @version 2
 * @date 16/12/24
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
#define NB_PAVES 5

/*Constantes des pavées*/
#define ZONE_DE_PROTECTION_X 15
#define ZONE_DE_PROTECTION_Y 5
#define TAILLE_PAVE 5
#define NOMBRE_PAVE 4

/*Constantes des éléments graphique hors joueur*/
#define CARACTERE_EFFACER ' ' // Pour effacer un élément
#define MUR '#'
#define POMME '6'
#define VITESSE 90000  // micro_sec
#define FERMER_JEU 'a' // Condition d'arrêt

/*Constantes des position des téléporteurs*/
#define T_HAUT_X 41
#define T_HAUT_Y 1 - 1

#define T_BAS_X 41
#define T_BAS_Y 40 + 1

#define T_DROITE_X 80 + 1
#define T_DROITE_Y 21

#define T_GAUCHE_X 1 - 1
#define T_GAUCHE_Y 21


// Définir les types tableau
// + 1 car la dernière valeur sert au tampon du dernier élément,
// pour l'effacer correctementpomme
typedef int corp_longeur[LONGEUR_MAX + 1];
typedef int t_pomme[NB_POMMES];

// Définir le type du tableau à deux dimensions
typedef char type_tableau_2d[TAILLE_TABLEAU_Y][TAILLE_TABLEAU_X];
typedef int conteneur[NOMBRE_PAVE][TAILLE_PAVE][TAILLE_PAVE];


// Définir le type du tableau à deux dimensions
typedef char type_tableau_2d[TAILLE_TABLEAU_Y][TAILLE_TABLEAU_X];

/* Initialisations des fonctions et des procédures */
void teleportation(int *tete_x, int *tete_y);
void init_plateau(type_tableau_2d tableau);
void creation_du_serpent(int x, int y, corp_longeur les_x, corp_longeur les_y);

/*Procédures de calcule de direction pour le CPU*/
void precalcul_pomme(t_pomme les_pommes_x, t_pomme les_pommes_y, int nb, int tete_x, int tete_y, int *x_avant_pomme, int *y_avant_pomme);
void progresser(corp_longeur les_x, corp_longeur les_y, bool *collision_joueur, int *x_avant_pomme, int *y_avant_pomme, conteneur block_x, conteneur block_y);

/*Procédures/Fonctions de communication entre la machine et l'utilisateur*/
char lire_entrer();
int kbhit(); // équivalent d'un INKEYS en BASIC

/*Procédures d'affichage*/
void afficher(int x, int y, char c); // Affiche en lien avec goto_x_y un caractère à l'écran
void dessiner_serpent(corp_longeur les_x, corp_longeur les_y);
void effacer_serpent(corp_longeur les_x, corp_longeur les_y);
void effacer(int x, int y);
void goto_x_y(int x, int y); // équivalent d'un LOCATE en Basic
void dessiner_plateau(type_tableau_2d plateau);
void enable_echo();
void disable_echo();

/*Fonctions de calcul de collision*/
int collision_mur(int tete_x, int tete_y);
int collision_avec_pomme(int x_tete, int y_tete, int x_pomme, int y_pomme);
int collision_avec_lui_meme(corp_longeur les_x, corp_longeur les_y, int tete_x, int tete_y);

/*Fonction pour les pavées*/
void deposer_pave(conteneur pos_des_paves_x, conteneur pos_des_paves_y);
int confirmer_position(int indice, corp_longeur liste, int nouvelle_position);
int collision_pave(conteneur block_x, conteneur block_y, int tete_x, int tete_y);

int main()
{
	system("clear");
	char lettre = CARACTERE_EFFACER; // Valeur du caractère espace
	type_tableau_2d plateau;		 // Plateau de jeu
	corp_longeur les_x;				 // Position du corps en X
	corp_longeur les_y;				 // Position du corps en Y
	bool collision_joueur = false;

	/*variable lié à la pomme*/
	t_pomme les_pommes_x = {75, 75, 78, 2, 8, 78, 74, 2, 72, 5};
	t_pomme les_pommes_y = {8, 39, 2, 2, 5, 39, 33, 38, 35, 2};

	int nb = 0;
	int nbMouv = 0;
	bool pomme_ramasser;

	conteneur pos_des_paves_x;
	conteneur pos_des_paves_y;

	// apparaître à 20 sur le tableau et non sur la console (20 + décalage du tableau dans la console)
	creation_du_serpent(POS_INITIAL_JOUEUR_X + 1, POS_INITIAL_JOUEUR_Y + 1, les_x, les_y);
	init_plateau(plateau);
	dessiner_plateau(plateau);
	//deposer_pave(pos_des_paves_x, pos_des_paves_y);

	disable_echo();

	int x_avant_pomme = 0;
	int y_avant_pomme = 0;

	// Va servir pour savoir si le joueur à été téléporter ou non
	int tamp_x = les_x[0];
	int tamp_y = les_y[0];

	precalcul_pomme(les_pommes_x, les_pommes_y, nb, les_x[0], les_y[0], &x_avant_pomme, &y_avant_pomme);
	afficher(les_pommes_x[nb], les_pommes_y[nb], POMME);
	clock_t begin = clock();

	do
	{
		// Lire les entrer au clavier
		nbMouv++;
		lettre = lire_entrer();
		/*
			Cette condition est ici pour éviter d'effacer le bout du serpent
			alors qu'il n'y a eu aucun déplacement à la fin du jeu.
		*/
		progresser(les_x, les_y, &collision_joueur, &x_avant_pomme, &y_avant_pomme, pos_des_paves_x, pos_des_paves_y);

		tamp_x = les_x[0];
		tamp_y = les_y[0];

		teleportation(&les_x[0], &les_y[0]);

		// Si le joueur est téléporter, alors on recalcule la distance
		if (tamp_x != les_x[0] || tamp_y != les_y[0])
		{
			precalcul_pomme(les_pommes_x, les_pommes_y, nb, les_x[0], les_y[0], &x_avant_pomme, &y_avant_pomme);
		}

		/*Collision et gestion du jeu avec la pomme*/
		pomme_ramasser = collision_avec_pomme(les_x[0], les_y[0], les_pommes_x[nb], les_pommes_y[nb]);

		if (pomme_ramasser == true)
		{
			nb++;

			if (nb == NB_POMMES)
			{
				collision_joueur = true;
			}
			else
			{
				precalcul_pomme(les_pommes_x, les_pommes_y, nb, les_x[0], les_y[0], &x_avant_pomme, &y_avant_pomme);
				afficher(les_pommes_x[nb], les_pommes_y[nb], POMME);
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

	printf("Temps CPU : %.4f secondes\n", tmpsCPU);
	printf("Nombre de mouvements : %d\n", nbMouv);

	return EXIT_SUCCESS;
}

void precalcul_pomme(t_pomme les_pommes_x, t_pomme les_pommes_y, int nb, int tete_x, int tete_y, int *x_avant_pomme, int *y_avant_pomme)
{

	/*
		Cumul la distance joueur -> portail_x et portail_opposé_x -> pomme
		Pour savoir si un téléporteur est intéressant.
		Si la distance pomme/tete est inférieur, alors les valeurs ne changent pas
	*/

	/*gestion des téléporteurs*/
	*x_avant_pomme = les_pommes_x[nb] - tete_x; // Chemin sur les X de base
	*y_avant_pomme = les_pommes_y[nb] - tete_y; // Chemin sur les Y de base

	/*Téléporteur gauche*/
	if (
	   	  abs(T_GAUCHE_X - tete_x) + abs(les_pommes_x[nb] - T_DROITE_X) 
		  + abs(T_GAUCHE_Y - tete_y) + abs(les_pommes_y[nb] - T_DROITE_Y) 
		  < abs(*x_avant_pomme + *y_avant_pomme)
	   )
	{
		// Assignation du nouveau chemin
		*x_avant_pomme = T_GAUCHE_X - tete_x;
		*y_avant_pomme = T_GAUCHE_Y - tete_y;
	}

	/*téléporteur droite*/
	else if (
		abs(T_DROITE_X - tete_x) + abs(les_pommes_x[nb] - T_GAUCHE_X)
		+ abs(T_DROITE_Y - tete_y) + abs(les_pommes_y[nb] - T_GAUCHE_Y)
		< abs(*x_avant_pomme + *y_avant_pomme)
	)
	{
		*x_avant_pomme = T_DROITE_X - tete_x;
		*y_avant_pomme = T_DROITE_Y - tete_y;
	}

	/*Téléporteur haut*/
	else if (
		abs(T_HAUT_X - tete_x) + abs(les_pommes_x[nb] - T_BAS_X)
		+ abs(T_HAUT_Y - tete_y) + abs(les_pommes_y[nb] - T_BAS_Y)
		< abs(*x_avant_pomme + *y_avant_pomme)
	)
	{
		*x_avant_pomme = T_HAUT_X - tete_x;
		*y_avant_pomme = T_HAUT_Y - tete_y;
	}

	/*téléporteur bas*/
	else if (
		abs(T_BAS_X - tete_x) + abs(les_pommes_x[nb] - T_HAUT_X)
		+ abs(T_BAS_Y - tete_y) + abs(les_pommes_y[nb] - T_BAS_Y)
		< abs(*x_avant_pomme + *y_avant_pomme)
	)
	{
		*x_avant_pomme = T_BAS_X - tete_x;
		*y_avant_pomme = T_BAS_Y - tete_y;
	}

}
int collision_avec_lui_meme(corp_longeur les_x, corp_longeur les_y, int tete_x, int tete_y)
{
	/**
	 * @brief regarde si la tête du joueur rentre dans le corps du joueur
	 * @param les_x corps en x du joueur
	 * @param les_y corps en y du joueur
	 * @param tete_x
	 * @param tete_y
	 * @return 0 si pas de collision, 1 si collision
	 */
	int i;
	bool collision = false;
	for (i = 1; i < taille_joueur; i++)
	{
		if ((tete_x == les_x[i]) && (tete_y == les_y[i]))
		{
			collision = true;
		}
	}
	return collision;
}

int collision_mur(int tete_x, int tete_y)
{
	bool dedans = false;

	// Gestion des téléporteur au quatre coin du niveau
	if (((tete_x >= TAILLE_TABLEAU_X) && (tete_y == TAILLE_TABLEAU_Y / 2 + 1)) ||
		((tete_x <= 1) && (tete_y == TAILLE_TABLEAU_Y / 2 + 1)) ||
		((tete_y >= TAILLE_TABLEAU_Y) && (tete_x == TAILLE_TABLEAU_X / 2 + 1)) ||
		((tete_y <= 1) && (tete_x == TAILLE_TABLEAU_X / 2 + 1)))
	{
		dedans = 1;
	}

	if ((tete_x < TAILLE_TABLEAU_X	  // Si le joueur touche le mur droit
		 && tete_x >= 2)			  // Si le joueur touche le mur gauche
		&& (tete_y < TAILLE_TABLEAU_Y // Si le joueur touche le mur bas
			&& tete_y >= 2))		  // Si le joueur touche le mur haut
	{
		dedans = true;
	}

	return dedans;
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
	{
		*tete_x = 1; // Téléporteur à droite du monde
	}
	else if (*tete_x < 1 && *tete_y == TAILLE_TABLEAU_Y / 2 + 1)
	{
		*tete_x = TAILLE_TABLEAU_X; // Téléporteur à gauche du monde
	}

	else if ((*tete_y > TAILLE_TABLEAU_Y) && (*tete_y == TAILLE_TABLEAU_X / 2 + 1))
	{
		*tete_y = 1; // Téléporteur en bas du monde
	}

	else if ((*tete_y < 1) && (*tete_x == TAILLE_TABLEAU_X / 2 + 1))
	{
		*tete_y = TAILLE_TABLEAU_Y; // Téléporteur en haut du monde
	}
}

void progresser(corp_longeur les_x, corp_longeur les_y, bool *collision_joueur, int *x_avant_pomme, int *y_avant_pomme, conteneur block_x, conteneur block_y)
{
    /**
     * @brief Calcule la nouvelle position du serpent quand il avance sans intervention du joueur
     * @param les_x Liste des positions en X des segments du serpent
     * @param les_y Liste des positions en Y des segments du serpent
     * @param *collision_joueur Pointeur vers une variable qui indique si le joueur est en collision (true si oui, false sinon)
     * @param *x_avant_pomme Distance restante en X entre la tête du serpent et la pomme
     * @param *y_avant_pomme Distance restante en Y entre la tête du serpent et la pomme
     */

    // Vérifie s'il n'y a pas de collision avec le joueur (le serpent peut progresser)
    if (*collision_joueur == false)
    {
        int i = 0;

        // Détermine la direction du mouvement selon la position relative de la pomme
		// Cela grâce à une double opération ternaire
        // - indicateur_y = -1 (vers le haut), 1 (vers le bas), 0 (aucun mouvement vertical)
        // - indicateur_x = -1 (vers la gauche), 1 (vers la droite), 0 (aucun mouvement horizontal)
        int indicateur_y = ((*y_avant_pomme) == 0) ? 0 : (*y_avant_pomme < 0) ? -1 : 1;
        int indicateur_x = ((*x_avant_pomme) == 0) ? 0 : (*x_avant_pomme < 0) ? -1 : 1;

        // Décale chaque segment du serpent pour suivre le mouvement de la tête.
        for (i = taille_joueur; i >= 1; i--)
        {
            les_x[i] = les_x[i - 1];
            les_y[i] = les_y[i - 1];
        }

        /* Déplacement sur l'axe Y (haut/bas) */

        // Vérifie si le mouvement vertical est possible :
        // 1. La tête ne touche pas un mur
        // 2. La prochaine case ne contient pas le serpent lui-même
        // 3. La case après la prochaine (prévision) ne contient pas le serpent
        if ((collision_mur(les_x[0], les_y[0] + indicateur_y) == true)
            && (collision_avec_lui_meme(les_x, les_y, les_x[0], les_y[0] + indicateur_y) == false)
            && (collision_avec_lui_meme(les_x, les_y, les_x[0], les_y[0] + indicateur_y + indicateur_y) == false)
			&& (collision_pave(block_x, block_y, les_x[0], les_y[0] + indicateur_y) == false))
        {
            // Déplace la tête du serpent sur l'axe Y
            les_y[0] = les_y[0] + indicateur_y;
            (*y_avant_pomme) = (*y_avant_pomme) - indicateur_y; // Met à jour la distance sur Y avant la pomme
        }
        else
        {
            /* Déplacement sur l'axe X (gauche/droite) */

            // Vérifie si le mouvement horizontal est possible :
            // Même principe que plus tôt mais sur l'axe X
            if (collision_mur(les_x[0] + indicateur_x, les_y[0]) == true
                && (collision_avec_lui_meme(les_x, les_y, les_x[0] + indicateur_x, les_y[0]) == false)
                && (collision_avec_lui_meme(les_x, les_y, les_x[0] + indicateur_x + indicateur_x, les_y[0]) == false)
				&& (collision_pave(block_x, block_y, les_x[0] + indicateur_x, les_y[0]) == false))
			
            {
                // Déplace la tête du serpent sur l'axe X
                les_x[0] = les_x[0] + indicateur_x;
                (*x_avant_pomme) = (*x_avant_pomme) - indicateur_x; // Met à jour la distance sur X avant la pomme
            }
            else
            {
                /* Si le serpent ne peut pas avancer dans la direction initiale, on cherche une alternative */

                // Si l'indicateur de mouvement vertical est nul (aucun mouvement Y prévu)
                if (indicateur_y == 0)
                {
                    // Sinon, change de direction verticalement (haus ou bas) selon les collisions
					// (L'on change la priorité à la collision plutôt qu'à la distance)
                    indicateur_y = (collision_avec_lui_meme(les_x, les_y, les_x[0], les_y[0] + 1) == false) ? 1 : -1;

                    // Vérifie si le nouveau mouvement vertical est possible.
                    if ((collision_mur(les_x[0], les_y[0] + indicateur_y) == true)
					   && (collision_avec_lui_meme(les_x, les_y, les_x[0], les_y[0] + indicateur_y) == false)
            		   && (collision_avec_lui_meme(les_x, les_y, les_x[0], les_y[0] + indicateur_y + indicateur_y) == false)
					   && (collision_pave(block_x, block_y, les_x[0], les_y[0] + indicateur_y) ==  false))
                    {
                        les_y[0] = les_y[0] + indicateur_y;
                        (*y_avant_pomme) = (*y_avant_pomme) - indicateur_y;
                    }
                }
                else
                {
                    // Même principe que plus tôt mais sur (gauche ou droite)
                    indicateur_x = (collision_avec_lui_meme(les_x, les_y, les_x[0] + 1, les_y[0]) == false) ? 1 : -1;

                    // Vérifie si le nouveau mouvement horizontal est possible.
                    if ((collision_mur(les_x[0] + indicateur_x, les_y[0]) == true)
					   && (collision_avec_lui_meme(les_x, les_y, les_x[0] + indicateur_x, les_y[0]) == false)
                	   && (collision_avec_lui_meme(les_x, les_y, les_x[0] + indicateur_x + indicateur_x, les_y[0]) == false)
					   && (collision_pave(block_x, block_y, les_x[0] + indicateur_x, les_y[0]) ==  false))

                    {
                        les_x[0] = les_x[0] + indicateur_x;
                        (*x_avant_pomme) = (*x_avant_pomme) - indicateur_x;
                    }
                }
            }
        }
    }
}


void deposer_pave(conteneur position_x, conteneur position_y)
{
	/**
	 * @brief dépose les pavés dans le monde
	 * @param position_x position des x des différent pavés
	 * @param position_y position des y des différent pavés
	 */
	int i, y, z;

	/*variable lié aux pavées*/
	int lesPavesX[NB_PAVES] = { 3, 74, 3, 74, 38, 38};
	int lesPavesY[NB_PAVES] = { 3, 3, 34, 34, 21, 15};


	for (z = 0; z < NB_PAVES; z++)
	{

		// Génération aléatoire en x et en y des pavé, en evitant qu'ils apparraissent sur le joueur
		// En créant une protection de 10 block autour de la tête (devant, derrière, haut et en bas)

		for (i = lesPavesY[z]; i < (TAILLE_PAVE + lesPavesY[z]); i++)
		{
			for (y = lesPavesX[z]; y < (TAILLE_PAVE + lesPavesX[z]); y++)
			{
				/* + 1 pour géré les bordures*/
				afficher(y, i, MUR);
				position_x[z][i][y] = y;
				position_y[z][i][y] = i;

			}

		}
	}
	fflush(stdout);
}

int confirmer_position(int indice, corp_longeur liste, int nouvelle_position)
{
	/**
	 * @brief permet de savoir si la position voulu par le pavé, n'as pas déjà été prise par un autre block
	 * @param indice est le n eme pavé poser
	 * @param liste liste des position déjà utilisé par les pavé précédent
	 * @param nouvelle_position la nouvelle position choisit par le pavé
	 * @return 0 si la position est bonne, 1 si elle est déjà utiliser par un autre pavé
	 */
	int i;
	int identique = 0;

	for (i = 0; i < indice; i++)
	{
		if (liste[i] == nouvelle_position)
		{
			identique = 1;
		}
	}
	return identique;
}

int collision_pave(conteneur block_x, conteneur block_y, int tete_x, int tete_y)
{
	/**
	 * @brief regarde s'il y a collision avec les différent pavé du jeu
	 * @param block_x liste à 3 dimensions des x des pavés
	 * @param block_y liste à 3 dimensions des y des pavés
	 * @param tete_x position de la tête en x
	 * @param tete_x position de la tête en y
	 */

	int i, y, z;
	bool collision = 0;

	for (z = 0; z <= NOMBRE_PAVE; z++)
	{
		for (i = 0; i <= TAILLE_PAVE; i++)
		{
			for (y = 0; y <= TAILLE_PAVE; y++)

			{

				if (block_x[z][i][y] == tete_x && tete_y == block_y[z][i][y])
				{
					collision = 1;
				}
			}
		}
	}

	return collision;
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