// Bibliothèques :
#include <stdio.h>
#include <stdlib.h>
#include <time.h>


// Fichiers headers des autres fonctions :
#include "structures.h"
#include "verifications.h"
#include "resoudre.h"
#include "fichiers.h"
#include "generer.h"



/*
Fonction qui aide le joueur en donnant une case.
Retourne 1 si tout se passe bien, 0 si la grille n'est pas résolvable.
*/
int aide(int grille[9][9])
{
    int k, l, copie[9][9], possible, fini;


    // On résoud une copie de la grille
    copier_grille(grille, copie);
    possible = resoudre(copie);

    verif_fini(grille, &fini);
    if(fini) {return 0;}
    if(!possible) {return 0;}

    else
    {
        // On donne une case au hasard
        do
        {
            k = (int)((rand()/(double)RAND_MAX)*8);
            l = (int)((rand()/(double)RAND_MAX)*8);
        } while(grille[k][l] != 0);

        grille[k][l] = copie[k][l]; // On place la valeur

        return 1;

    }
}
