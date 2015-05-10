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
Cette fonction va enregistrer la grille dans un fichier txt qui pourra ensuite être imprimé.
Une grille est dessinée afin de rendre le tout plus lisible.
Les cases et les régions ne sont pas délimitées de la même manière.
*/
void impression (int grille[9][9], char* nomFichier)
{
    int i, j, n, k, l;
    FILE *fichier;
    fichier = fopen (nomFichier, "w+");

    l = 1;

    for(i = 0; i < 9; i++)
    {
        k = 0;
        for(j = 0; j < 9; j++)
        {
            n = grille[i][j];
            k++;
            if(n == 0) {fprintf(fichier, "   ");} // On remplace les 0 par des espaces pour ne pas les imprimer
            else {fprintf(fichier, " %d ", n);}

            if(k == 3 || k == 6) {fprintf(fichier, " * ");}
            else if (k != 9) {fprintf(fichier, "|");}
        }
        if(l == 3 || l == 6) {fprintf(fichier, "\n* * * * * * * * * * * * * * * * * * *\n");}
        else if(l == 9) {fprintf(fichier, "\n");}
        else fprintf(fichier, "\n----------- * ----------- * ----------\n");
        l++;
    }
    fclose(fichier);
}
