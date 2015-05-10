#include <stdio.h>
#include <stdlib.h>
#include "structures.h"
#include "verifications.h"
#include "resoudre.h"
#include "fichiers.h"
#include "generer.h"


int resoudre_sans_supposition (int grille[9][9])
{
    int i = 0, j = 0, n = 0, posi = 0, posj = 0, posn = 0, nb_poss = 0, posi2 = 0, posj2 = 0, posn2 = 0, nb_poss2 = 0, fini = 0, k;
    nb grille_poss[9][9];

    // Par defaut on met toutes les possibilités à 1 (= vrai), puis on fait le test et on met à 0 si c'est impossible
    for (i=0; i < 9; i++)
    {
        for (j = 0;j < 9 ; j++)
        {
            for (n = 0;  n < 9; n++)
            {
                grille_poss[i][j].nombre[n] = 1;
            }
        }
    }


    // On appelle la fonction verif_possibilites pour savoir quels chiffres on peut mettre dans chaque case
    verif_possibilites(grille, grille_poss);

   k = 1;

    while(!fini && k)
    {
        k = 0; // k nous sert à savoir si on a trouvé une case ou si on est bloqué
        for(i = 0; i < 9; i++)
        {
            for(j = 0; j < 9; j++)
            {
                nb_poss = 0;
                for(n = 0; n < 9; n++) // On regarde si pour une case donnée un seul coup est possible
                {
                    if(grille_poss[i][j].nombre[n])
                    {
                        nb_poss++;
                        posn = n;
                        posi = i;
                        posj = j;
                    }
                }
                if (nb_poss == 1)
                {
                    grille[posi][posj] = posn + 1;
                    verif_possibilites(grille, grille_poss);
                    k = 1;
                }
            }
        }

        // On vérifie si un n ne peut aller qu'à une case de la ligne / colonne / région
        for(n = 0; n < 9; n++)
        {
            for(i = 0; i < 9; i++)
            {
                nb_poss = 0;
                nb_poss2 = 0;
                for(j = 0; j < 9; j++)
                {
                    if(grille_poss[i][j].nombre[n]) // On parcourt la ligne
                    {
                        nb_poss++;
                        posn = n;
                        posi = i;
                        posj = j;
                    }
                    if(grille_poss[j][i].nombre[n]) // On parcourt la colonne
                    {
                        nb_poss2++;
                        posn2 = n;
                        posi2 = j;
                        posj2 = i;
                    }
                }
                if(nb_poss == 1) // Si n ne peut aller qu'à un endroit sur la ligne on le place
                {
                    grille[posi][posj] = posn + 1;
                    verif_possibilites(grille, grille_poss);
                    k = 1;
                }
                if(nb_poss2 == 1) // Si n ne peut aller que dans une case de la colonne on le place
                {
                    grille[posi2][posj2] = posn2 + 1;
                    verif_possibilites(grille, grille_poss);
                    k = 1;
                }
            }
        }

    verif_fini(grille, &fini);
    }

    if(fini) {return 1;} // retourne 1 si la grille est résolue, et 0 si elle ne peut pas être résolue complètement de maière logique
    else {return 0;}
}




int resoudre_avec_supposition(struct grille_supp *dernier_element, int impossible)
{
    int i, j, n, p;
    grille_supp supp;

    supp.suiv = dernier_element; // On se place entre l'avant dernier et le dernier élément
    supp.prec = supp.suiv->prec;



    i = supp.suiv->i; // On récupère les coordonnées pour si on doit changer une supposition
    j = supp.suiv->j;
    n = supp.suiv->n;



    if(impossible) // Si impossible il faut changer une supposition
    {
        copier_grille(supp.suiv->prec->grille, supp.suiv->grille); // On réinitialise la grille

        if(n < 9) // On teste un autre n (si on peut)
        {
            n++;
            p = verif_case(supp.suiv->grille, n, i, j); // On cherche un n qui peut aller dans la case
            while(n < 9 && !p)
            {
                n++;
                p = verif_case(supp.suiv->grille, n, i, j);
            }
            if(p) // Si on a trouvé un coup possible on le teste
            {
                supp.suiv->grille[i][j] = n; // On change le n dans la grille
                supp.suiv->n = n; // On sauvegarde la supposition (pour pouvoir y revenir)
            }
            else
            {
                impossible = 1;
                supp.suiv->n = 9; // Aucun n ne convient, on veut donc passer à la partie qui suit
                resoudre_avec_supposition(supp.suiv, impossible); // on ré appelle la fonction pour reculer dans la liste (on execute donc la partie qui suit)
            }
        }


        else // Ou si on les a tous testés on revient en arrière pour changer une supposition plus ancienne
        {
            if(supp.prec != NULL) // On ne supprime pas la première case
            {
                free(supp.suiv); // On revient en arrière, donc la case suivante n'est plus nécéssaire
                supp.suiv = supp.prec; // On recule
                supp.prec = supp.prec->prec;
                supp.suiv->suiv = NULL; // Il n'y a rien après
                impossible = 1;
                resoudre_avec_supposition(supp.suiv, impossible); // on ré appelle la fonction pour changer de supposition (on execute donc la partie juste au dessus)
            }
            else // Cela veut dire qu'on est au premier maillon, donc cela ne devrait pas être impossible, donc il y a une erreur
            {
                return 0;
            }
        }
    }


    else // Si non on fait une nouvelle supposition et on continue
    {
        supp.suiv->suiv = malloc(sizeof(grille_supp)); // On ajoute un nouvel élément dans la liste
        supp.suiv->suiv->prec = supp.suiv; // On raccorde dans les deux sens
        supp.prec = supp.suiv; // On déplace supp
        supp.suiv = supp.suiv->suiv;
        supp.suiv->suiv = NULL;
        copier_grille(supp.prec->grille, supp.suiv->grille); // On initialise la grille

        i=0;
        j=0;
        p = 0; // p = 1 si la case est vide, 0 si non.

        if(supp.suiv->grille[i][j] == 0) {p = 1;}

        while(i <= 8 && !p) // On cherche la première case vide de la grille
        {
            while(j < 8 && !p)
            {
                j++;
                if(supp.suiv->grille[i][j] == 0) {p = 1;}
            }
            if(!p) // Si la case ne convient pas on teste une autre ligne
            {
                j = 0;
                i++;
            }
        }
        // Une fois qu'on sort de la boucle les coordonnées i,j correspondent à une case vide

        n=1;
        p = verif_case(supp.suiv->grille, n, i, j);

        while(!p && n < 9) // On cherche le premier n qui peut aller dans la case (il y en a forcément un, sinon impossible = 1 et on arrive pas jusqu'ici
        {
            n++;
            p = verif_case(supp.suiv->grille, n, i, j);
        }

        if(n <= 9) // On vérifie tout de même
        {
            supp.suiv->grille[i][j] = n; // On place n dans la grille
            supp.suiv->i = i; // On enregistre la supposition pour pouvoir la changer si besoin
            supp.suiv->j = j;
            supp.suiv->n = n;
        }
    }
    return 1;
}




int resoudre(int grille[9][9])
{
    int fini, impossible, prov = 1;
    grille_supp prem, aux;

    fini = resoudre_sans_supposition(grille); // On essaye de resoudre sans supposition
    impossible = verif_impossible(grille);
    if(impossible) {return 0;} // La grille n'est pas résolvable

    if(!fini)
    {
        prem.suiv = NULL; // Initialisation de toutes les variables de prem
        prem.prec = NULL;
        prem.i = -1; // Aucune case n'a étée modifiée
        prem.j = -1;
        prem.n = 0;
        copier_grille(grille, prem.grille); // On copie la grille

        aux.suiv = &prem; // Aux (qui nous permet de parcourir la liste) pointe sur le premier élément (puis il sera déplacé si nécéssaire)

        do
        {
            impossible = verif_impossible(aux.suiv->grille);
            prov = resoudre_avec_supposition(aux.suiv, impossible);
            aux.suiv = &prem; // On se replace au début de la liste
            while(aux.suiv->suiv != NULL) {aux.suiv = aux.suiv->suiv;} // On veut se placer au dernier maillon (il a peut être changé)
            fini = resoudre_sans_supposition(aux.suiv->grille); // Après chaque supposition on regarde si cela nous a permis de trouver d'autres cases
        } while(!fini && prov); // On continue tant que la grille n'est pas complète


        copier_grille(aux.suiv->grille, grille); // On copie la grille

        // On libère toute la chaine
        aux.suiv = prem.suiv;
        while(aux.suiv->suiv != NULL)
        {
            aux.prec = aux.suiv;
            aux.suiv = aux.suiv->suiv;
            free(aux.prec);
        }
        free(aux.suiv); // On libère le dernier élément de la chaine
    }
    return 1;
}































