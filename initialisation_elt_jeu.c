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

void creation_du_serpent_joueur(int x, int y, corp_longeur les_x, corp_longeur les_y, int dir)
{
	/**
	 * @brief Fonction qui viens crée le serpent la premièrer fois.
	 * @param x position initial en x
	 * @param y position initial en y
	 * @param les_x tableau des position en x
	 * @param les_y tableau des position en y
	 * @param dir 1 = droite | 2 = gauche
	 *
	 */
	int i = 0;

	int direction = (dir == 1)? 1 : -1;

	for (i = 0; i < TAILLE_JOUEUR; i ++)
	{
		les_x[i] = x - i * direction;
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
	int les_paves_x[NB_PAVES] = { 4, 73, 4, 73, 38, 38};
    int les_paves_y[NB_PAVES] = { 4, 4, 33, 33, 14, 22};


	for (z = 0; z < NB_PAVES; z++)
	{

		for (i = 0; i < TAILLE_PAVE; i++)
		{
			for (y = 0; y < TAILLE_PAVE; y++)
			{
				/* + 1 pour géré les bordures*/
				afficher(y + les_paves_x[z], i + les_paves_y[z], MUR);
				
				position_x[z][i][y] = y + les_paves_x[z];
				position_y[z][i][y] = i + les_paves_y[z];
				
			}

		}
	}
}
