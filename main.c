/**
 * @file main.c
 * @brief Programme d'un jeu de snake autonome
 * @author Dhennin Elouan, Martin Esmeralda
 * @version 4
 * @date 13/01/25
 *
 *
 */
#include <stdbool.h>
#include <string.h>
#include <time.h>
#include <stdio.h>
#include <unistd.h>


#include "src/fonction_tete.h"

int main()
{
	system("clear");
	char lettre = CARACTERE_EFFACER; // Valeur du caractère espace
	type_tableau_2d plateau;		 // Plateau de jeu
	corp_longeur les_x_joueur1;				 // Position du corps en X joueur1
	corp_longeur les_y_joueur1;				 // Position du corps en Y joueur1
	corp_longeur les_x_joueur2;				 // Position du corps en X joueur2
	corp_longeur les_y_joueur2;				 // Position du corps en Y joueur2
	bool collision_joueur1 = false;
	bool collision_joueur2 = false;

	/*variable lié à la pomme*/
	t_pomme les_pommes_x = {75, 75, 78, 2, 8, 78, 74, 2, 72, 5};
	t_pomme les_pommes_y = {8, 39, 2, 2, 5, 39, 33, 38, 35, 2};

	int pomme_actuel = 0;
	int nb_mouvement_joueur1  = 0;
	int nb_mouvement_joueur2  = 0;
	bool pomme_ramasser;

	conteneur pos_des_paves_x;
	conteneur pos_des_paves_y;

	// apparaître à 20 sur le tableau et non sur la console (20 + décalage du tableau dans la console)
	creation_du_serpent_joueur(POS_INITIAL_JOUEUR1_X , POS_INITIAL_JOUEUR1_Y , les_x_joueur1, les_y_joueur1);//joueur 1
	creation_du_serpent_joueur(POS_INITIAL_JOUEUR2_X , POS_INITIAL_JOUEUR2_Y , les_x_joueur2, les_y_joueur2);//joueur2
	init_plateau(plateau);
	dessiner_plateau(plateau);
	deposer_pave(pos_des_paves_x, pos_des_paves_y);


	disable_echo();

	int x_avant_pomme_joueur1 = 0;
	int y_avant_pomme_joueur1 = 0;

	int x_avant_pomme_joueur2 = 0;
	int y_avant_pomme_joueur2 = 0;

	// Va servir pour savoir si le joueur à été téléporter ou non
	int tamp_x = les_x_joueur1[0];
	int tamp_y = les_y_joueur1[0];

	precalcul_pomme_joueur1(les_pommes_x, les_pommes_y, pomme_actuel, les_x_joueur1[0], les_y_joueur1[0], &x_avant_pomme_joueur1, &y_avant_pomme_joueur1);
	precalcul_pomme_joueur2(les_pommes_x, les_pommes_y, pomme_actuel, les_x_joueur2[0], les_y_joueur2[0], &x_avant_pomme_joueur2, &y_avant_pomme_joueur2);
	afficher(les_pommes_x[pomme_actuel], les_pommes_y[pomme_actuel], POMME);
	clock_t begin = clock();
	do
	{
		// Lire les entrer au clavier
		nb_mouvement_joueur1 ++;
		lettre = lire_entrer();
		/*
			Cette condition est ici pour éviter d'effacer le bout du serpent
			alors qu'il n'y a eu aucun déplacement à la fin du jeu.
		*/
		progresser_joueur1(les_x_joueur1, les_y_joueur1, &collision_joueur1, &x_avant_pomme_joueur1, &y_avant_pomme_joueur1, pos_des_paves_x, pos_des_paves_y);

		tamp_x = les_x_joueur1[0];
		tamp_y = les_y_joueur1[0];

		teleportation(&les_x_joueur1[0], &les_y_joueur1[0]);

		// Si le joueur est téléporter, alors on recalcule la distance
		if (tamp_x != les_x_joueur1[0] || tamp_y != les_y_joueur1[0])
		{
			precalcul_pomme_joueur1(les_pommes_x, les_pommes_y, pomme_actuel, les_x_joueur1[0], les_y_joueur1[0], &x_avant_pomme_joueur1, &y_avant_pomme_joueur1);
		}

		/*Collision et gestion du jeu avec la pomme*/
		pomme_ramasser = collision_avec_pomme(les_x_joueur1[0], les_y_joueur1[0], les_pommes_x[pomme_actuel], les_pommes_y[pomme_actuel]);

		if (pomme_ramasser == true)
		{
			pomme_actuel++;

			if (pomme_actuel == NB_POMMES)
			{
				collision_joueur1 = true;
			}
			else
			{
				precalcul_pomme_joueur1(les_pommes_x, les_pommes_y, pomme_actuel, les_x_joueur1[0], les_y_joueur1[0], &x_avant_pomme_joueur1, &y_avant_pomme_joueur1);
				afficher(les_pommes_x[pomme_actuel], les_pommes_y[pomme_actuel], POMME);
				
			}
		}
		else
		{
			// Lire les entrer au clavier
			nb_mouvement_joueur2 ++;
			lettre = lire_entrer();
			/*
				Cette condition est ici pour éviter d'effacer le bout du serpent
				alors qu'il n'y a eu aucun déplacement à la fin du jeu.
			*/
			progresser_joueur2(les_x_joueur2, les_y_joueur2, &collision_joueur2, &x_avant_pomme_joueur2, &y_avant_pomme_joueur2, pos_des_paves_x, pos_des_paves_y);

			tamp_x = les_x_joueur2[0];
			tamp_y = les_y_joueur2[0];

			teleportation(&les_x_joueur2[0], &les_y_joueur2[0]);

			// Si le joueur est téléporter, alors on recalcule la distance
			if (tamp_x != les_x_joueur2[0] || tamp_y != les_y_joueur2[0])
			{
				precalcul_pomme_joueur2(les_pommes_x, les_pommes_y, pomme_actuel, les_x_joueur2[0], les_y_joueur2[0], &x_avant_pomme_joueur2, &y_avant_pomme_joueur2);
			}

			/*Collision et gestion du jeu avec la pomme*/
			pomme_ramasser = collision_avec_pomme(les_x_joueur2[0], les_y_joueur2[0], les_pommes_x[pomme_actuel], les_pommes_y[pomme_actuel]);

			if (pomme_ramasser == true)
			{
				pomme_actuel++;

				if (pomme_actuel == NB_POMMES)
				{
					collision_joueur2 = true;
				}
				else
				{
					precalcul_pomme_joueur2(les_pommes_x, les_pommes_y, pomme_actuel, les_x_joueur2[0], les_y_joueur2[0], &x_avant_pomme_joueur2, &y_avant_pomme_joueur2);
					afficher(les_pommes_x[pomme_actuel], les_pommes_y[pomme_actuel], POMME);
				}
			}
			// Si le joueur obtien la dernière pomme, on veux qu'ils l'efface
			// Alors malgré le fait qu'il aie fini, on actualise ça position pour
			// Montrer la pomme manger
			if (collision_joueur2 != 1 || pomme_actuel == NB_POMMES)
			{
				effacer_serpent(les_x_joueur1, les_y_joueur1);
				effacer_serpent(les_x_joueur2,les_y_joueur2);
				dessiner_serpent(les_x_joueur1, les_y_joueur1);
				dessiner_serpent(les_x_joueur2, les_y_joueur2);
				usleep(VITESSE);
			}
		}
		// Si le joueur obtien la dernière pomme, on veux qu'ils l'efface
		// Alors malgré le fait qu'il aie fini, on actualise ça position pour
		// Montrer la pomme manger
		if (collision_joueur1 != 1 || pomme_actuel == NB_POMMES)
		{
			effacer_serpent(les_x_joueur1, les_y_joueur1);
			effacer_serpent(les_x_joueur2,les_y_joueur2);
			dessiner_serpent(les_x_joueur1, les_y_joueur1);
			dessiner_serpent(les_x_joueur2, les_y_joueur2);
			usleep(VITESSE);
		}
		
		/*
		Le temps de jeu étant dérisoire, nous prenons en compte uniquement le temps
		D'attente entre deux image. Qui est finalement la majorité du temps de jeu
		du joueur. Attendre...
		*/
	} while ((lettre != FERMER_JEU) && pomme_actuel != NB_POMMES);

	clock_t end = clock();
	double tmpsCPU = ((end - begin) * 1.0) / CLOCKS_PER_SEC;

	enable_echo();
	goto_x_y(1, CACHER_CURSEUR);

	printf("Temps CPU : %.4f secondes\n", tmpsCPU);
	printf("Nombre de mouvements joueur 1: %d\n", nb_mouvement_joueur1 );
	printf("Nombre de mouvements joueur 2: %d\n", nb_mouvement_joueur2 );

	return EXIT_SUCCESS;
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


																																																																																																				/*
																																																																																																									_________________________
																																																																																																									|						 |
																																																																																																									|						 |						
																																																																																																									|					|	 |
																																																																																																									|			*			 |
																																																																																																									|	|   				 |
																																																																																																									|						 |
																																																																																																									|		 6	|	8		 |
																																																																																																									|________________________|
																																																																																																				*/
