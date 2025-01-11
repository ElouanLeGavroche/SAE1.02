#ifndef CONST_H
#define CONST_H

// CONSTANTE DU JEU
/*Constante du joueur*/
#define TETE_JOUEUR 'O' // Tête du joueur
#define CORP_JOUEUR 'X' // Corp du joueur
#define POS_INITIAL_JOUEUR_X 40
#define POS_INITIAL_JOUEUR_Y 19
#define LONGEUR_MAX 20
#define TAILLE_JOUEUR 10
#define TAILLE_INITIAL_DU_JOUEUR 10 // Taille de base du joueur (en réalité 10, car la boucle for commence à 0)

/*Les constantes du jeu*/
#define NB_POMMES 10
#define CACHER_CURSEUR 42 // Valeur absurde pour être sur d'être coller en bas de la console
#define TAILLE_TABLEAU_Y 40
#define TAILLE_TABLEAU_X 80
#define NB_PAVES 6

/*Constantes des pavées*/
#define ZONE_DE_PROTECTION_X 15
#define ZONE_DE_PROTECTION_Y 5
#define TAILLE_PAVE 5
#define NOMBRE_PAVE 6 

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
#define T_DROITE_Y 20

#define T_GAUCHE_X 1 - 1
#define T_GAUCHE_Y 20

#endif