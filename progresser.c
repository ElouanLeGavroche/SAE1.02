#include <stdbool.h>

#include "src/fonction_tete.h"

// Fonction pour déplacer le serpent
void deplacer_serpent(corp_longeur les_x_joueur, corp_longeur les_y_joueur, int *x_avant_pomme, int *y_avant_pomme, int direction_x, int direction_y) {
    //Déplace le corps du serpent
    for (int i = TAILLE_JOUEUR; i > 0; i--) {
        les_x_joueur[i] = les_x_joueur[i - 1];
        les_y_joueur[i] = les_y_joueur[i - 1];
    }

    les_x_joueur[0] += direction_x;
    les_y_joueur[0] += direction_y;
    *x_avant_pomme -= direction_x;
    *y_avant_pomme -= direction_y;
}

// Fonction pour déplacer le serpent
void deplacer_serpent_2(corp_longeur les_x_joueur, corp_longeur les_y_joueur, int *x_avant_pomme, int *y_avant_pomme, int direction_x, int direction_y) {
    //Déplace le corps du serpent
    for (int i = TAILLE_JOUEUR; i > 0; i--) {
        les_x_joueur[i] = les_x_joueur[i - 1];
        les_y_joueur[i] = les_y_joueur[i - 1];
    }

    les_x_joueur[0] += direction_x;
    les_y_joueur[0] += direction_y;
    *x_avant_pomme -= direction_x;
    *y_avant_pomme -= direction_y;
}


bool mouvement_possible(corp_longeur les_x_joueur1, corp_longeur les_y_joueur1, conteneur block_x, conteneur block_y, corp_longeur les_x_joueur2, corp_longeur les_y_joueur2, int direction_x, int direction_y) {
    //Au lieu de l'appeler comme un bourrin à chaque condition, on as décidé de le mettre dans une fonction
    //Vérifie donc toutes les éventuelle collision du joueur
    return collision_mur_2(les_x_joueur1[0] + direction_x, les_y_joueur1[0] + direction_y) == true &&
           collision_avec_lui_meme_2(les_x_joueur1, les_y_joueur1, les_x_joueur1[0] + direction_x, les_y_joueur1[0] + direction_y) == false &&
           collision_pave_2(block_x, block_y, les_x_joueur1[0] + direction_x, les_y_joueur1[0] + direction_y) == false &&
           collision_avec_joueur_2(les_x_joueur1[0] + direction_x, les_y_joueur1[0] + direction_y, les_x_joueur2, les_y_joueur2) == false;
}

bool mouvement_possible_2(corp_longeur les_x_joueur1, corp_longeur les_y_joueur1, conteneur block_x, conteneur block_y, corp_longeur les_x_joueur2, corp_longeur les_y_joueur2, int direction_x, int direction_y) {
    //Au lieu de l'appeler comme un bourrin à chaque condition, on as décidé de le mettre dans une fonction
    //Vérifie donc toutes les éventuelle collision du joueur
    return collision_mur(les_x_joueur1[0] + direction_x, les_y_joueur1[0] + direction_y) == true &&
           collision_avec_lui_meme(les_x_joueur1, les_y_joueur1, les_x_joueur1[0] + direction_x, les_y_joueur1[0] + direction_y) == false &&
           collision_pave(block_x, block_y, les_x_joueur1[0] + direction_x, les_y_joueur1[0] + direction_y) == false &&
           collision_avec_joueur(les_x_joueur1[0] + direction_x, les_y_joueur1[0] + direction_y, les_x_joueur2, les_y_joueur2) == false;
}

void progresser1(corp_longeur les_x_joueur1, corp_longeur les_y_joueur1, corp_longeur les_x_joueur2, corp_longeur les_y_joueur2, bool *collision_joueur, int *x_avant_pomme, int *y_avant_pomme, conteneur block_x, conteneur block_y)
{

    if (*collision_joueur == false)
    {

        //Opération ternaire, qui va nous rapprocher le plus possible de la pomme
        int indicateur_y = (*y_avant_pomme < 0) ? -1 : ((*y_avant_pomme > 0) ? 1 : 0);
        int indicateur_x = (*x_avant_pomme < 0) ? -1 : ((*x_avant_pomme > 0) ? 1 : 0);

        // Tente de se déplacer dans la direction initiale (vers la pomme)
        if (indicateur_y != 0 && mouvement_possible(les_x_joueur1, les_y_joueur1, block_x, block_y, les_x_joueur2, les_y_joueur2, 0, indicateur_y))
        {
            deplacer_serpent(les_x_joueur1, les_y_joueur1, x_avant_pomme, y_avant_pomme, 0, indicateur_y);
        }
        else if (indicateur_x != 0 && mouvement_possible(les_x_joueur1, les_y_joueur1, block_x, block_y, les_x_joueur2, les_y_joueur2, indicateur_x, 0))
        {
            deplacer_serpent(les_x_joueur1, les_y_joueur1, x_avant_pomme, y_avant_pomme, indicateur_x, 0);
        }
        else
        {
            // Si bloqué, tente de contourner l'obstacle en priorisant le contournement

            // Essaye toutes les directions possibles
            int directions[4][2] = {{0, 1}, {1, 0}, {0, -1}, {-1, 0}}; // Haut, droite, bas, gauche
            //for (int i = 0; i < 4; i++)
            int i = 0;
            int direction_x = directions[i][0];
            int direction_y = directions[i][1];
            do
            {
                direction_x = directions[i][0];  
                direction_y = directions[i][1]; 
                if (mouvement_possible(les_x_joueur1, les_y_joueur1, block_x, block_y, les_x_joueur2, les_y_joueur2, direction_x, direction_y))
                {
                    deplacer_serpent(les_x_joueur1, les_y_joueur1, x_avant_pomme, y_avant_pomme, direction_x, direction_y);
                    break;
                }

                i ++;

            }while(mouvement_possible(les_x_joueur1, les_y_joueur1, block_x, block_y, les_x_joueur2, les_y_joueur2, direction_x, direction_y) != true);

        }
    }
}


void progresser2(corp_longeur les_x_joueur1, corp_longeur les_y_joueur1, corp_longeur les_x_joueur2, corp_longeur les_y_joueur2, bool *collision_joueur, int *x_avant_pomme, int *y_avant_pomme, conteneur block_x, conteneur block_y)
{
 
    if (*collision_joueur == false)
    {

        //Opération ternaire, qui va nous rapprocher le plus possible de la pomme
        int indicateur_y = (*y_avant_pomme < 0) ? -1 : ((*y_avant_pomme > 0) ? 1 : 0);
        int indicateur_x = (*x_avant_pomme < 0) ? -1 : ((*x_avant_pomme > 0) ? 1 : 0);

        // Tente de se déplacer dans la direction initiale (vers la pomme)
        if (indicateur_y != 0 && mouvement_possible_2(les_x_joueur1, les_y_joueur1, block_x, block_y, les_x_joueur2, les_y_joueur2, 0, indicateur_y))
        {
            deplacer_serpent_2(les_x_joueur1, les_y_joueur1, x_avant_pomme, y_avant_pomme, 0, indicateur_y);
        }
        else if (indicateur_x != 0 && mouvement_possible_2(les_x_joueur1, les_y_joueur1, block_x, block_y, les_x_joueur2, les_y_joueur2, indicateur_x, 0))
        {
            deplacer_serpent_2(les_x_joueur1, les_y_joueur1, x_avant_pomme, y_avant_pomme, indicateur_x, 0);
        }
        else
        {
            // Si bloqué, tente de contourner l'obstacle en priorisant le contournement

            // Essaye toutes les directions possibles
            int directions[4][2] = {{0, 1}, {1, 0}, {0, -1}, {-1, 0}}; // Haut, droite, bas, gauche

            int i = 0;
            int direction_x = directions[i][0];
            int direction_y = directions[i][1];
            do
            {
                direction_x = directions[i][0];  
                direction_y = directions[i][1]; 
                if (mouvement_possible_2(les_x_joueur1, les_y_joueur1, block_x, block_y, les_x_joueur2, les_y_joueur2, direction_x, direction_y))
                {
                    deplacer_serpent_2(les_x_joueur1, les_y_joueur1, x_avant_pomme, y_avant_pomme, direction_x, direction_y);
                    break;
                }

                i ++;

            }while(mouvement_possible_2(les_x_joueur1, les_y_joueur1, block_x, block_y, les_x_joueur2, les_y_joueur2, direction_x, direction_y) != true);

        }
    }
}
