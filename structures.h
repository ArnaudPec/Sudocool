// Constantes pour la difficulté des niveaux (nombre de cases non directes à trouver)
#define BORNE_FACILE 25
#define BORNE_MOYEN 35
#define BORNE_DIFFICILE 45


// Structure pour garder en mémoire les possibilités de chaque case
typedef struct nb
{
    int nombre[9];
}nb;

// Structure de liste doublement chaînée pour les résolution (donc aussi les générations) de grilles avec supposition
typedef struct grille_supp
{
    struct grille_supp *prec;
    int grille[9][9]; // On fait une sauvegarde de la grille à chaque supposition pour pouvoir revenir en arrière si la supposition n'est pas bonne
    int n, i, j; // Pour savoir quelle supposition est faite sur cette grille
    struct grille_supp *suiv;
}grille_supp;

