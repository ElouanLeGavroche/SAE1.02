#include <stdio.h>

#include "src/fonction_tete.h"

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
	effacer(les_x[TAILLE_JOUEUR], les_y[TAILLE_JOUEUR]);
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

void dessiner_serpent(corp_longeur les_x, corp_longeur les_y,char tete_joueur)
{
	/**
	 * @brief Dessine le joueur en premier, et fait une boucle pour dessiner les enfants
	 * (le corps) du joueur en fonction de la taille du joueur.
	 * @param les_x liste des valeurs en x
	 * @param les_y liste des valeurs en y
	 */
	int i;
	for (i = 1; i < TAILLE_JOUEUR; i++)
	{
		afficher(les_x[i], les_y[i], CORP_JOUEUR);
	}
	afficher(les_x[0], les_y[0], tete_joueur);
	fflush(stdout);
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
