#include <stdbool.h>

#include "src/fonction_tete.h"

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
	for (i = 0; i < TAILLE_JOUEUR; i++)
	{
		if ((tete_x == les_x[i]) && (tete_y == les_y[i]))
		{
			collision = true;
		}
	}
	return collision;
}

int collision_avec_lui_meme_2(corp_longeur les_x, corp_longeur les_y, int tete_x, int tete_y)
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
	for (i = 0; i < TAILLE_JOUEUR; i++)
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
	if (((tete_x >= TAILLE_TABLEAU_X) && (tete_y == TAILLE_TABLEAU_Y / 2)) ||
		((tete_x <= 1) && (tete_y == TAILLE_TABLEAU_Y / 2)) ||
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

int collision_mur_2(int tete_x, int tete_y)
{
	bool dedans = false;

	// Gestion des téléporteur au quatre coin du niveau
	if (((tete_x >= TAILLE_TABLEAU_X) && (tete_y == TAILLE_TABLEAU_Y / 2)) ||
		((tete_x <= 1) && (tete_y == TAILLE_TABLEAU_Y / 2)) ||
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
	/*
		Parcours d'une boucle à trois dimension :
		z -> les n pavés
		i -> n largeur des pavés
		y -> n profondeur des pavés
	*/
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

int collision_pave_2(conteneur block_x, conteneur block_y, int tete_x, int tete_y)
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
	/*
		Parcours d'une boucle à trois dimension :
		z -> les n pavés
		i -> n largeur des pavés
		y -> n profondeur des pavés
	*/
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

int collision_avec_pomme_2(int x_tete, int y_tete, int x_pomme, int y_pomme)
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

bool collision_avec_joueur(int tete_x_joueur1, int tete_y_joueur1, corp_longeur les_x_joueur2, corp_longeur les_y_joueur2)
{
	int i;
	bool collision = false;
	for (i = 0; i < TAILLE_JOUEUR; i++)
	{
		if ((tete_x_joueur1 == les_x_joueur2[i]) && (tete_y_joueur1 == les_y_joueur2[i]))
		{
			collision = true;
		}
	}
	return collision;
}


bool collision_avec_joueur_2(int tete_x_joueur1, int tete_y_joueur1, corp_longeur les_x_joueur2, corp_longeur les_y_joueur2)
{
	int i;
	bool collision = false;
	for (i = 0; i < TAILLE_JOUEUR; i++)
	{
		if ((tete_x_joueur1 == les_x_joueur2[i]) && (tete_y_joueur1 == les_y_joueur2[i]))
		{
			collision = true;
		}
	}
	return collision;
}
