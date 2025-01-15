#include <stdlib.h>

#include "src/fonction_tete.h"

void precalcul_pomme_joueur1(t_pomme les_pommes_x, t_pomme les_pommes_y, int nb, int tete_x, int tete_y, int *x_avant_pomme, int *y_avant_pomme)
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

void precalcul_pomme_joueur2(t_pomme les_pommes_x, t_pomme les_pommes_y, int nb, int tete_x, int tete_y, int *x_avant_pomme, int *y_avant_pomme)
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

void teleportation(int *tete_x, int *tete_y)
{
	/**
	 * @brief téléporte le joueur d'un coté à l'autre du monde avec les "portail"
	 * @param *tete_x tête du joueur en x en sortie
	 * @param *tete_y tête du joueur en y en sortie
	 */

	if (*tete_x > TAILLE_TABLEAU_X && *tete_y == TAILLE_TABLEAU_Y / 2)
	{
		*tete_x = 1; // Téléporteur à droite du monde
	}
	else if (*tete_x < 1 && *tete_y == TAILLE_TABLEAU_Y / 2)
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