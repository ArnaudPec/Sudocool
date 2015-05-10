#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "structures.h"
#include "verifications.h"
#include "resoudre.h"
#include "fichiers.h"
#include "generer.h"


int evaluer_niveau(int grille[9][9], int nb_cases)
{
    int lvl, i, j, n, nb_directs = 0, nb_a_trouver = 0, nbre;
    nb grille_poss[9][9];
    verif_possibilites(grille, grille_poss);

    for(i = 0; i < 9; i++)
    {
        for(j = 0; j < 9; j++)
        {
            nbre = 0;
            for(n = 0; n < 9; n++)
            {
                if(grille_poss[i][j].nombre[n] == 1) {nbre++;}
            }
            if(nbre == 1) {nb_directs++;} // On compte le nombre de cases où il n'y a qu'une seule possibilité
        }
    }
    nb_a_trouver = nb_cases - nb_directs; // Dans le nombre de cases à trouver on retire les cases les plus directes

    // On attribue le niveau selon le nombre de cases non directes à trouver
    if(nb_a_trouver >= BORNE_FACILE && nb_a_trouver < BORNE_MOYEN) {lvl = 1;}
    else if(nb_a_trouver >= BORNE_MOYEN && nb_a_trouver < BORNE_DIFFICILE) {lvl = 2;}
    else if(nb_a_trouver >= BORNE_DIFFICILE) {lvl = 3;}
    else {lvl = 0;} // Si il ne manque pas assez de cases

    return lvl;

}



void generer (int grille[9][9], int lvl)
{
    int i, j, n, nb, p, continuer = 1, lvl_actuel, grille_test[9][9];

    srand(time(NULL)); // Initialisation de rand

// Initialisation de la grille (on ne sait pas ce qu'elle contient, ça peut être une ancienne grille)
    for(i = 0; i < 9; i++)
    {
        for(j = 0; j < 9; j++)
        {
            grille[i][j] = 0;
        }
    }


/* Les 3 régions de la diagonale peuvent être remplies aléatoirement sans se soucier des lignes ni des colonnes (juste en faisant attention à ne pas mettre 2 fois le même nombre dans la région) */
    for(i = 0; i < 3; i++) // Remplissage de la région située en haut à gauche
    {
        for(j = 0; j < 3; j++)
        {
            n = (int)((rand()/(double)RAND_MAX)*9 +1); //+1 Car avec la castation on risque d'obtenir 0
            if(n > 9) {n = 9;} // On vérifie qu'on ne dépasse pas les bornes
            p = verif_case(grille, n, i, j);
            while(!p)
            {
                if(n==9) {n=1;}
                else {n++;}
                p = verif_case(grille, n, i, j);
            }
            grille[i][j] = n; // Une fois qu'on arrive ici c'est que le n est valide
        }
    }

    for(i = 3; i < 6; i++) // Remplissage de la région située au milieu
    {
        for(j = 3; j < 6; j++)
        {
            n = (int)((rand()/(double)RAND_MAX)*9 +1); // Même résonnement
            if(n > 9) {n = 9;}
            p = verif_case(grille, n, i, j);
            while(!p)
            {
                if(n==9) {n=1;}
                else {n++;}
                p = verif_case(grille, n, i, j);
            }
            grille[i][j] = n;
        }
    }

    for(i = 6; i < 9; i++) // Remplissage de la région située en bas à droite
    {
        for(j = 6; j < 9; j++)
        {
            n = (int)((rand()/(double)RAND_MAX)*9 +1); // Même résonnement
            if(n > 9) {n = 9;}
            p = verif_case(grille, n, i, j);
            while(!p)
            {
                if(n==9) {n=1;}
                else {n++;}
                p = verif_case(grille, n, i, j);
            }
            grille[i][j] = n;
        }
    }


/* La diagonale est générée, on peut finir de compléter la grille en faisant appel à la fonction résoudre et en supposant les cases manquantes */
    resoudre(grille);


/* On obtient donc une grille complète, il faut retirer certaines cases pour la rendre jouable */
    continuer = 1;
    nb = 0;

    while(continuer)
    {
        i = (int)((rand()/(double)RAND_MAX)*8);
        j = (int)((rand()/(double)RAND_MAX)*8);
        if(grille[i][j] != 0)
        {
            n = grille[i][j];
            grille_test[i][j] = 0;
            p = resoudre_sans_supposition(grille_test); // On vérifie qu'il n'y ai qu'une résolution possible
            if(p)
            {
                grille[i][j] = 0;
                nb++;
                lvl_actuel = evaluer_niveau(grille, nb);
                if(lvl_actuel == lvl) {continuer = 0;}
            }
            copier_grille(grille, grille_test);
        }
    }
}










