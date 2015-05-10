#include <stdio.h>
#include <stdlib.h>
#include "structures.h"
#include "verifications.h"
#include "resoudre.h"
#include "fichiers.h"
#include "generer.h"


/* Cette fonction vérifie si une case de coordonnées (i;j) peut prendre la valeur n */
int verif_case(int grille[9][9], int n, int i, int j)
{
    int i_case, j_case, k, l;

/* On se place dans la case du coin superieur droit du carré de 3x3 qui contient la case (i;j) */
    if (i < 3) {i_case = 0;}
    else if (i < 6 && i >= 3) {i_case = 3;}
    else {i_case = 6;}

    if (j < 3) {j_case = 0;}
    else if (j < 6 && j >= 3) {j_case = 3;}
    else {j_case = 6;}



    if (grille[i][j] != 0) // On vérifie si il y a déjà un chiffre dans la case, auquel cas les étapes suivantes ne sont pas nécéssaires, aucun coup n'est possible dans cette case.
    {
        return 0;
    }

    else
    {
    /* On vérifie si il y a déjà une case contenant n dans ce carré */
        for (k = i_case; k < i_case + 3;  k++) // Il y a deux cases en plus de celle de départ dans le carré (en ligne et en colonne)
        {
            for (l = j_case; l < j_case + 3;  l++ )
            {
                if (grille[k][l] == n)
                {
                    return 0; // Si il y a une case du carré qui contient n, pas besoin d'aller plus loin : on arrête et retourne 0
                }
            }
        }
    // Si aucune case du carré ne contient n, on test la ligne et la colonne
        for (k = 0; k < 9; k++)
        {
            if (grille[k][j] == n) // On teste la colonne
            {
                return 0; // Si il y a déjà un n dans la colonne, on arrête et retourne 0
            }

            else if (grille[i][k] == n) // On teste la ligne
            {
                return 0; // Si il y a déjà un n dans la ligne, on arrête et retourne 0
            }
        }

    return 1; // Si on arrive jusqu'ici c'est qu'il n'y a pas de n dans le carré, la colonne et la ligne, donc la case peut contenir n
    }
}

/* On vérifie toutes les cases de la grille en utilisant la fonction verif_case */
void verif_possibilites (int grille[9][9], nb grille_poss[9][9])
{
    int i, j, n;
    for (i = 0; i < 9; i++)
    {
        for (j = 0;  j < 9;j++)
        {
            for (n = 0;  n < 9; n++)
            {
                grille_poss[i][j].nombre[n] = verif_case(grille, (n+1), i, j); // n+1 car ici n de 0 à 8 (décalage dû au tableau, alors qu'on veut tester le vrai n)
            }
        }
    }
}


void verif_fini(int grille[9][9], int *fini)
{
    int i, j;
    *fini = 1; // On affecte la valeur 1 à fini puis on vérifie si il reste des cases blanches, auquel cas fini prend la valeur 0 (= faux, soit non fini)
    for (i= 0;  i < 9; i++)
    {
        for (j = 0; j < 9;  j++)
        {
            if(grille[i][j] == 0)
            {
                *fini = 0;
            }
        }
    }
}



int verif_impossible(int grille[9][9]) // Vérifie si la résolution de la grille est possible (pas de cases où il n'y a pas de possibilités)
{
    int i, j, n, impossible = 0, coup_possible;
//    nb grille_poss[9][9] = {0};
    nb grille_poss[9][9] ;

    verif_possibilites(grille, grille_poss);

    for(i = 0; i < 9; i++)
    {
        for(j = 0; j < 9; j++)
        {
            if(grille[i][j] == 0)
            {
                coup_possible = 0;
                for(n = 0; n < 9; n++)
                {
                    if(grille_poss[i][j].nombre[n] != 0) {coup_possible = 1;} // On regarde si au moins un coup est possible
                }
                if(coup_possible == 0) {impossible = 1;} // Si aucun coup n'est possible la grille est impossible
            }
        }
    }
    return impossible;
}



int verif_valide(int grille[9][9])
{
    int i, j, n, valide, p;

    valide = 1;

    for(i = 0; i < 9; i++)
    {
        for(j = 0; j < 9; j++)
        {
            n = grille[i][j]; // On sauvegarde le chiffre présent dans la case
            grille[i][j] = 0; // On vide cette case pour savoir si n peut aller dedans
            p = verif_case(grille, n, i, j); // On vérifie si le coup est possible
            grille[i][j] = n; // On replace n
            if(!p && n != 0) {valide = 0;} // Si le coup n'est pas possible la grille n'est pas valide
        }
    }

    return valide;
}
