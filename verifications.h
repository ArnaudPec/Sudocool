// Cette fonction vérifie si un chiffre n peut aller dans une case de coordonnées (i;j).
int verif_case(int grille[9][9], int n, int i, int j);

// Cette fonction vérifie toutes les possibilités de toutes les cases du jeu.
void verif_possibilites (int grille[9][9], nb grille_poss[9][9]);

// Cette fonction vérifie si le jeu est fini, donc si toutes les cases sont remplies.
void verif_fini(int grille[9][9], int *fini);

// Cette fonction vérifie si une grille est résovable
int verif_impossible(int grille[9][9]);

int verif_valide(int grille[9][9]);
