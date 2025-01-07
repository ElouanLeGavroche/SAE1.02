#ifndef FONCTION_TETE_H
#define FONCTION_TETE_H

#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include "types.h"

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

#endif