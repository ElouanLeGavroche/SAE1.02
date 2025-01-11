#include "src/fonction_tete.h"
#include <stdio.h>
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
				lig == TAILLE_TABLEAU_Y / 2 - 1 ||
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
	for (i = 0; i < TAILLE_JOUEUR; i++)
	{
		les_x[i] = x - i;
		les_y[i] = y;
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

		for (i = 0; i < TAILLE_PAVE; i++)
		{
			for (y = 0; y < TAILLE_PAVE; y++)
			{
				/* + 1 pour géré les bordures*/
				afficher(y + lesPavesX[z], i + lesPavesY[z], MUR);
				
				position_x[z][i][y] = y + lesPavesX[z];
				position_y[z][i][y] = i + lesPavesY[z];
				
			}

		}
	}
}