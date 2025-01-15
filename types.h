#ifndef TYPES_H
#define TYPES_H

#include "const.h"

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

// Définir le type des tuple à deux éléments entier
typedef int tuple_2_elt[TAILLE_TUPLE];

#endif