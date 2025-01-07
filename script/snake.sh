#!/bin/bash

#CONSTANTES
readonly PATH_EXEC="../bin"
readonly PATH_SRC="../src"
readonly PATH_PRG=".."

mkdir -p $PATH_EXEC
mkdir -p $PATH_SRC

rm -f $PATH_EXEC/*.out # supprimer les fichiers .out
# compilation séparée

gcc -Wall -c $PATH_PRG/fonction_donner.c 
gcc -Wall -c $PATH_PRG/gestion_graphique.c 
gcc -Wall -c $PATH_PRG/collisions.c
gcc -Wall -c $PATH_PRG/progresser.c
gcc -Wall -c $PATH_PRG/calcul_dir.c
gcc -Wall -c $PATH_PRG/initialisation_elt_jeu.c
gcc -Wall -c $PATH_PRG/main.c

# edition de lien
gcc -Wall $PATH_SRC/const.h $PATH_SRC/types.h $PATH_SRC/fonction_tete.h main.o fonction_donner.o gestion_graphique.o collisions.o progresser.o calcul_dir.o initialisation_elt_jeu.o -o $PATH_EXEC/main.out
rm -f *.o # supprimer les fichiers .o
rm -f *.gch # supprimer les fichiers .gch

# lancement du programme … si tout va bien
./$PATH_EXEC/main.out

