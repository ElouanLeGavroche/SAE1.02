#include <stdbool.h>

#include "src/fonction_tete.h"


void progresser1(corp_longeur les_x_joueur1, corp_longeur les_y_joueur1, corp_longeur les_x_joueur2, corp_longeur les_y_joueur2, bool *collision_joueur, int *x_avant_pomme, int *y_avant_pomme, conteneur block_x, conteneur block_y)
{
    /**
     * @brief Calcule la nouvelle position du serpent quand il avance sans intervention du joueur
     * @param les_x_joueur1 Liste des positions en X des segments du serpent 1
     * @param les_y_joueur1 Liste des positions en Y des segments du serpent 1
     * @param les_x_joueur2 Liste des positions en X des segments du serpent 2
     * @param les_y_joueur2 Liste des positions en Y des segments du serpent 2
     * @param *collision_joueur Pointeur vers une variable qui indique si le joueur est en collision (true si oui, false sinon)
     * @param *x_avant_pomme Distance restante en X entre la tête du serpent et la pomme
     * @param *y_avant_pomme Distance restante en Y entre la tête du serpent et la pomme
     */

    // Vérifie s'il n'y a pas de collision avec le joueur (le serpent peut progresser)
    if (*collision_joueur == false)
    {
        int i = 0;

        // Détermine la orientation du mouvement selon la position relative de la pomme
        // Cela grâce à une double opération ternaire
        // - indicateur_y = -1 (vers le haut), 1 (vers le bas), 0 (aucun mouvement vertical)
        // - indicateur_x = -1 (vers la gauche), 1 (vers la droite), 0 (aucun mouvement horizontal)
        int indicateur_y = ((*y_avant_pomme) == 0) ? 0 : (*y_avant_pomme < 0) ? -1 : 1;
        int indicateur_x = ((*x_avant_pomme) == 0) ? 0 : (*x_avant_pomme < 0) ? -1 : 1;

        // Décale chaque segment du serpent pour suivre le mouvement de la tête.
        for (i = TAILLE_JOUEUR; i >= 1; i--)
        {
            les_x_joueur1[i] = les_x_joueur1[i - 1];
            les_y_joueur1[i] = les_y_joueur1[i - 1];
        }

        /* Déplacement sur l'axe Y (haut/bas) */

        // Vérifie si le mouvement vertical est possible :
        // 1. La tête ne touche pas un mur
        // 2. La prochaine case ne contient pas le serpent lui-même
        // 3. La case après la prochaine (prévision) ne contient pas le serpent
        // 4. Vérification de la précense d'un pavé
        if ((collision_mur(les_x_joueur1[0], les_y_joueur1[0] + indicateur_y) == true) 
            && (collision_avec_lui_meme(les_x_joueur1, les_y_joueur1, les_x_joueur1[0], les_y_joueur1[0] + indicateur_y) == false) 
            && (collision_avec_lui_meme(les_x_joueur1, les_y_joueur1, les_x_joueur1[0], les_y_joueur1[0] + indicateur_y + indicateur_y) == false) 
            && (collision_pave(block_x, block_y, les_x_joueur1[0], les_y_joueur1[0] + indicateur_y) == false)
            && (collision_avec_joueur(les_x_joueur1[0],les_y_joueur1[0]+indicateur_y,les_x_joueur2,les_y_joueur2)==false))
        {
            // Déplace la tête du serpent sur l'axe Y
            les_y_joueur1[0] = les_y_joueur1[0] + indicateur_y;
            (*y_avant_pomme) = (*y_avant_pomme) - indicateur_y; // Met à jour la distance sur Y avant la pomme
        }
        else
        {
            /* Déplacement sur l'axe X (gauche/droite) */

            // Vérifie si le mouvement horizontal est possible :
            // Même principe que plus tôt mais sur l'axe X
            if (collision_mur(les_x_joueur1[0] + indicateur_x, les_y_joueur1[0]) == true 
                && (collision_avec_lui_meme(les_x_joueur1, les_y_joueur1, les_x_joueur1[0] + indicateur_x, les_y_joueur1[0]) == false) 
                && (collision_avec_lui_meme(les_x_joueur1, les_y_joueur1, les_x_joueur1[0] + indicateur_x + indicateur_x, les_y_joueur1[0]) == false) 
                && (collision_pave(block_x, block_y, les_x_joueur1[0] + indicateur_x, les_y_joueur1[0]) == false)
                && (collision_avec_joueur(les_x_joueur1[0]+indicateur_x,les_y_joueur1[0],les_x_joueur2,les_y_joueur2)==false))

            {
                // Déplace la tête du serpent sur l'axe X
                les_x_joueur1[0] = les_x_joueur1[0] + indicateur_x;
                (*x_avant_pomme) = (*x_avant_pomme) - indicateur_x; // Met à jour la distance sur X avant la pomme
            }
            else
            {
                /* Si le serpent ne peut pas avancer dans la orientation initiale, on cherche une alternative */

                if ((collision_avec_joueur(les_x_joueur1[0],les_y_joueur1[0]+indicateur_y,les_x_joueur2,les_y_joueur2)==false) 
                    || (collision_pave(block_x, block_y, les_x_joueur1[0], les_y_joueur1[0] + indicateur_y) == false) || indicateur_y == 0)
                {
                    // Sinon, change de orientation verticalement (haus ou bas) selon les collisions
                    // (L'on change la priorité à la collision plutôt qu'à la distance)
                    indicateur_y = (collision_avec_joueur(les_x_joueur1[0],les_y_joueur1[0]+1,les_x_joueur2,les_y_joueur2)==false)? 1 : (collision_avec_lui_meme(les_x_joueur1, les_y_joueur1, les_x_joueur1[0], les_y_joueur1[0] + 1) == false) ? 1 : -1;

                    // Vérifie si le nouveau mouvement vertical est possible.
                    if ((collision_mur(les_x_joueur1[0], les_y_joueur1[0] + indicateur_y) == true) 
                        && (collision_avec_lui_meme(les_x_joueur1, les_y_joueur1, les_x_joueur1[0], les_y_joueur1[0] + indicateur_y) == false) 
                        && (collision_avec_lui_meme(les_x_joueur1, les_y_joueur1, les_x_joueur1[0], les_y_joueur1[0] + indicateur_y + indicateur_y) == false) 
                        && (collision_pave(block_x, block_y, les_x_joueur1[0], les_y_joueur1[0] + indicateur_y) == false)
                        && (collision_avec_joueur(les_x_joueur1[0],les_y_joueur1[0]+indicateur_y,les_x_joueur2,les_y_joueur2)==false))
                    {
                        les_y_joueur1[0] = les_y_joueur1[0] + indicateur_y;
                        (*y_avant_pomme) = (*y_avant_pomme) - indicateur_y;
                    }
                    else
                    {
                        // Passe par les x si bloqué en y
                        indicateur_x = (collision_avec_joueur(les_x_joueur1[0]+indicateur_x,les_y_joueur1[0]+1,les_x_joueur2,les_y_joueur2)==false)? 1 : (collision_avec_lui_meme(les_x_joueur1, les_y_joueur1, les_x_joueur1[0] + 1, les_y_joueur1[0]) == false) ? 1 : -1;

                        les_x_joueur1[0] = les_x_joueur1[0] + indicateur_x;
                        (*x_avant_pomme) = (*x_avant_pomme) - indicateur_x;
                    }
                }

                else
                {
                    // Même principe que plus tôt mais sur (gauche ou droite)
                    indicateur_x = (collision_avec_joueur(les_x_joueur1[0]+indicateur_x,les_y_joueur1[0]+1,les_x_joueur2,les_y_joueur2)==false)? 1 : (collision_avec_lui_meme(les_x_joueur1, les_y_joueur1, les_x_joueur1[0] + 1, les_y_joueur1[0]) == false) ? 1 : -1;


                    // Vérifie si le nouveau mouvement horizontal est possible.
                    if ((collision_mur(les_x_joueur1[0] + indicateur_x, les_y_joueur1[0]) == true) 
                        && (collision_avec_lui_meme(les_x_joueur1, les_y_joueur1, les_x_joueur1[0] + indicateur_x, les_y_joueur1[0]) == false) 
                        && (collision_avec_lui_meme(les_x_joueur1, les_y_joueur1, les_x_joueur1[0] + indicateur_x + indicateur_x, les_y_joueur1[0]) == false) 
                        && (collision_pave(block_x, block_y, les_x_joueur1[0] + indicateur_x, les_y_joueur1[0]) == false)
                        && (collision_avec_joueur(les_x_joueur1[0]+indicateur_x,les_y_joueur1[0],les_x_joueur2,les_y_joueur2)==false))

                    {
                        les_x_joueur1[0] = les_x_joueur1[0] + indicateur_x;
                        (*x_avant_pomme) = (*x_avant_pomme) - indicateur_x;
                    }
                }
            }
        }
    }
}



void progresser2(corp_longeur les_x_joueur1, corp_longeur les_y_joueur1, corp_longeur les_x_joueur2, corp_longeur les_y_joueur2, bool *collision_joueur, int *x_avant_pomme, int *y_avant_pomme, conteneur block_x, conteneur block_y)
{
    /**
     * @brief Calcule la nouvelle position du serpent quand il avance sans intervention du joueur
     * @param les_x_joueur1 Liste des positions en X des segments du serpent 1
     * @param les_y_joueur1 Liste des positions en Y des segments du serpent 1
     * @param les_x_joueur2 Liste des positions en X des segments du serpent 2
     * @param les_y_joueur2 Liste des positions en Y des segments du serpent 2
     * @param *collision_joueur Pointeur vers une variable qui indique si le joueur est en collision (true si oui, false sinon)
     * @param *x_avant_pomme Distance restante en X entre la tête du serpent et la pomme
     * @param *y_avant_pomme Distance restante en Y entre la tête du serpent et la pomme
     */

    // Vérifie s'il n'y a pas de collision avec le joueur (le serpent peut progresser)
    if (*collision_joueur == false)
    {
        int i = 0;

        // Détermine la orientation du mouvement selon la position relative de la pomme
        // Cela grâce à une double opération ternaire
        // - indicateur_y = -1 (vers le haut), 1 (vers le bas), 0 (aucun mouvement vertical)
        // - indicateur_x = -1 (vers la gauche), 1 (vers la droite), 0 (aucun mouvement horizontal)
        int indicateur_y = ((*y_avant_pomme) == 0) ? 0 : (*y_avant_pomme < 0) ? -1 : 1;
        int indicateur_x = ((*x_avant_pomme) == 0) ? 0 : (*x_avant_pomme < 0) ? -1 : 1;

        // Décale chaque segment du serpent pour suivre le mouvement de la tête.
        for (i = TAILLE_JOUEUR; i >= 1; i--)
        {
            les_x_joueur1[i] = les_x_joueur1[i - 1];
            les_y_joueur1[i] = les_y_joueur1[i - 1];
        }

        /* Déplacement sur l'axe Y (haut/bas) */

        // Vérifie si le mouvement vertical est possible :
        // 1. La tête ne touche pas un mur
        // 2. La prochaine case ne contient pas le serpent lui-même
        // 3. La case après la prochaine (prévision) ne contient pas le serpent
        // 4. Vérification de la précense d'un pavé
        if ((collision_mur(les_x_joueur1[0], les_y_joueur1[0] + indicateur_y) == true) 
            && (collision_avec_lui_meme(les_x_joueur1, les_y_joueur1, les_x_joueur1[0], les_y_joueur1[0] + indicateur_y) == false) 
            && (collision_avec_lui_meme(les_x_joueur1, les_y_joueur1, les_x_joueur1[0], les_y_joueur1[0] + indicateur_y + indicateur_y) == false) 
            && (collision_pave(block_x, block_y, les_x_joueur1[0], les_y_joueur1[0] + indicateur_y) == false)
            && (collision_avec_joueur(les_x_joueur1[0],les_y_joueur1[0]+indicateur_y,les_x_joueur2,les_y_joueur2)==false))
        {
            // Déplace la tête du serpent sur l'axe Y
            les_y_joueur1[0] = les_y_joueur1[0] + indicateur_y;
            (*y_avant_pomme) = (*y_avant_pomme) - indicateur_y; // Met à jour la distance sur Y avant la pomme
        }
        else
        {
            /* Déplacement sur l'axe X (gauche/droite) */

            // Vérifie si le mouvement horizontal est possible :
            // Même principe que plus tôt mais sur l'axe X
            if (collision_mur(les_x_joueur1[0] + indicateur_x, les_y_joueur1[0]) == true 
                && (collision_avec_lui_meme(les_x_joueur1, les_y_joueur1, les_x_joueur1[0] + indicateur_x, les_y_joueur1[0]) == false) 
                && (collision_avec_lui_meme(les_x_joueur1, les_y_joueur1, les_x_joueur1[0] + indicateur_x + indicateur_x, les_y_joueur1[0]) == false) 
                && (collision_pave(block_x, block_y, les_x_joueur1[0] + indicateur_x, les_y_joueur1[0]) == false)
                && (collision_avec_joueur(les_x_joueur1[0]+indicateur_x,les_y_joueur1[0],les_x_joueur2,les_y_joueur2)==false))

            {
                // Déplace la tête du serpent sur l'axe X
                les_x_joueur1[0] = les_x_joueur1[0] + indicateur_x;
                (*x_avant_pomme) = (*x_avant_pomme) - indicateur_x; // Met à jour la distance sur X avant la pomme
            }
            else
            {
                /* Si le serpent ne peut pas avancer dans la orientation initiale, on cherche une alternative */

                if ((collision_avec_joueur(les_x_joueur1[0],les_y_joueur1[0]+indicateur_y,les_x_joueur2,les_y_joueur2)==false) 
                    || (collision_pave(block_x, block_y, les_x_joueur1[0], les_y_joueur1[0] + indicateur_y) == false) || indicateur_y == 0)
                {
                    // Sinon, change de orientation verticalement (haus ou bas) selon les collisions
                    // (L'on change la priorité à la collision plutôt qu'à la distance)
                    indicateur_y = (collision_avec_joueur(les_x_joueur1[0],les_y_joueur1[0]+1,les_x_joueur2,les_y_joueur2)==false)? 1 : (collision_avec_lui_meme(les_x_joueur1, les_y_joueur1, les_x_joueur1[0], les_y_joueur1[0] + 1) == false) ? 1 : -1;

                    // Vérifie si le nouveau mouvement vertical est possible.
                    if ((collision_mur(les_x_joueur1[0], les_y_joueur1[0] + indicateur_y) == true) 
                        && (collision_avec_lui_meme(les_x_joueur1, les_y_joueur1, les_x_joueur1[0], les_y_joueur1[0] + indicateur_y) == false) 
                        && (collision_avec_lui_meme(les_x_joueur1, les_y_joueur1, les_x_joueur1[0], les_y_joueur1[0] + indicateur_y + indicateur_y) == false) 
                        && (collision_pave(block_x, block_y, les_x_joueur1[0], les_y_joueur1[0] + indicateur_y) == false)
                        && (collision_avec_joueur(les_x_joueur1[0],les_y_joueur1[0]+indicateur_y,les_x_joueur2,les_y_joueur2)==false))
                    {
                        les_y_joueur1[0] = les_y_joueur1[0] + indicateur_y;
                        (*y_avant_pomme) = (*y_avant_pomme) - indicateur_y;
                    }
                    else
                    {
                        // Passe par les x si bloqué en y
                        indicateur_x = (collision_avec_joueur(les_x_joueur1[0]+indicateur_x,les_y_joueur1[0]+1,les_x_joueur2,les_y_joueur2)==false)? 1 : (collision_avec_lui_meme(les_x_joueur1, les_y_joueur1, les_x_joueur1[0] + 1, les_y_joueur1[0]) == false) ? 1 : -1;

                        les_x_joueur1[0] = les_x_joueur1[0] + indicateur_x;
                        (*x_avant_pomme) = (*x_avant_pomme) - indicateur_x;
                    }
                }

                else
                {
                    // Même principe que plus tôt mais sur (gauche ou droite)
                    indicateur_x = (collision_avec_joueur(les_x_joueur1[0]+indicateur_x,les_y_joueur1[0]+1,les_x_joueur2,les_y_joueur2)==false)? 1 : (collision_avec_lui_meme(les_x_joueur1, les_y_joueur1, les_x_joueur1[0] + 1, les_y_joueur1[0]) == false) ? 1 : -1;


                    // Vérifie si le nouveau mouvement horizontal est possible.
                    if ((collision_mur(les_x_joueur1[0] + indicateur_x, les_y_joueur1[0]) == true) 
                        && (collision_avec_lui_meme(les_x_joueur1, les_y_joueur1, les_x_joueur1[0] + indicateur_x, les_y_joueur1[0]) == false) 
                        && (collision_avec_lui_meme(les_x_joueur1, les_y_joueur1, les_x_joueur1[0] + indicateur_x + indicateur_x, les_y_joueur1[0]) == false) 
                        && (collision_pave(block_x, block_y, les_x_joueur1[0] + indicateur_x, les_y_joueur1[0]) == false)
                        && (collision_avec_joueur(les_x_joueur1[0]+indicateur_x,les_y_joueur1[0],les_x_joueur2,les_y_joueur2)==false))

                    {
                        les_x_joueur1[0] = les_x_joueur1[0] + indicateur_x;
                        (*x_avant_pomme) = (*x_avant_pomme) - indicateur_x;
                    }
                }
            }
        }
    }
}
