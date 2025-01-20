#ifndef FONCTION_TETE_H
#define FONCTION_TETE_H

#include <stdlib.h>
#include <stdbool.h>

#include "types.h"

/* Initialisations des fonctions et des procédures */
void teleportation(int *tete_x, int *tete_y);
void init_plateau(type_tableau_2d tableau);
void creation_du_serpent_joueur(int x, int y, corp_longeur les_x, corp_longeur les_y, int dir);

/*Procédures de calcule de direction pour le CPU*/
void precalcul_pomme_joueur1(t_pomme les_pommes_x, t_pomme les_pommes_y, int nb, int tete_x, int tete_y, int *x_avant_pomme, int *y_avant_pomme);
void precalcul_pomme_joueur2(t_pomme les_pommes_x, t_pomme les_pommes_y, int nb, int tete_x, int tete_y, int *x_avant_pomme, int *y_avant_pomme);
void progresser1(corp_longeur les_x_joueur1, corp_longeur les_y_joueur1, corp_longeur les_x_joueur2, corp_longeur les_y_joueur2, bool *collision_joueur, int *x_avant_pomme, int *y_avant_pomme, conteneur block_x, conteneur block_y);
void progresser2(corp_longeur les_x_joueur1, corp_longeur les_y_joueur1, corp_longeur les_x_joueur2, corp_longeur les_y_joueur2, bool *collision_joueur, int *x_avant_pomme, int *y_avant_pomme, conteneur block_x, conteneur block_y);
bool calcule_avancer_de_direction(int tete_x, int tete_y, int direction, conteneur block_x, conteneur block_y, int x_avant_pomme, int y_avant_pomme);

/*Procédures/Fonctions de communication entre la machine et l'utilisateur*/
char lire_entrer();
int kbhit(); // équivalent d'un INKEYS en BASIC

/*Procédures d'affichage*/
void afficher(int x, int y, char c); // Affiche en lien avec goto_x_y un caractère à l'écran
void dessiner_serpent(corp_longeur les_x, corp_longeur les_y,char tete_joueur);
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
bool collision_avec_joueur (int tete_x_joueur1, int tete_y_joueur1, corp_longeur les_x_joueur2, corp_longeur les_y_joueur2);

/*Fonction pour les pavées*/
void deposer_pave(conteneur pos_des_paves_x, conteneur pos_des_paves_y);
int confirmer_position(int indice, corp_longeur liste, int nouvelle_position);
int collision_pave(conteneur block_x, conteneur block_y, int tete_x, int tete_y);

#endif
