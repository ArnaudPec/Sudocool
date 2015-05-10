#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "fichiers.h"


void copier_grille(int grille[9][9], int copie[9][9]) // Copie grille dans copie
{
    int i, j;
    for(i = 0; i < 9; i++)
    {
        for(j = 0; j < 9; j++)
        {
            copie[i][j] = grille[i][j];
        }
    }
}

int comparer_grille(int grille[9][9], int autreGrille[9][9])
{
    int i, j;
    for(i = 0; i < 9; i++)
    {
        for(j = 0; j < 9; j++)
        {
            if (grille[i][j]!=autreGrille[i][j]) return 0;
        }
    }

      return 1;
}

int enregistrer (int grille[9][9], char* nomFichier) // Retourne 1 si ça marche, 0 si il y a une erreur
{
    int i, j;
    FILE *fichier = NULL;

        fichier = fopen(nomFichier, "a"); // On ouvre le fichier


    if(fichier == NULL) // On vérifie l'ouverture
    {
        return 0;
    }
    else
    {
        for(i = 0; i < 9; i++)
        {
            for(j = 0; j < 9; j++)
            {
                fprintf(fichier, "%d", grille[i][j]);
            }
        }
        fprintf(fichier, "\n"); // On retourne à la ligne quand toute la grille est rentrée

        fclose(fichier);
        return 1;
    }

}

int lecture (int grille[9][9], char* nomFichier)
{
    int i, j, n, k, nb = 0;
    FILE *fichier = NULL;
    char c;

    // On ouvre le bon fichier de niveau
    fichier = fopen(nomFichier, "r");



    if(fichier == NULL) {return 0;}

    else
    {
        while((c = fgetc(fichier)) != EOF)
        {
            if(c == '\n') {nb++;} // On compte le nombre de lignes
        }
        n = (int)((rand() / RAND_MAX) * nb); // On tire un n au sort pour savoir quelle grille on ouvre
        k = 1;
        fseek(fichier, 0, SEEK_SET);
        while(k < n)
        {
            c = fgetc(fichier);
            if(c == '\n') {k++;} // On se place à la bonne ligne
        }
        for(i = 0; i < 9; i++)
        {
            for(j = 0; j < 9; j++)
            {
                grille[i][j] = fgetc(fichier) - 48; // -48 pour passer de ASCII à base 10
            }
        }
        fclose(fichier);

        return 1;

    }
}

