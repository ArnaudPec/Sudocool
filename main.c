/*Sudoku réalisé dans le cadre de l'UV NF05 par Antoine Faure et Arnaud Pecoraro
2013
*/

// Bibliothéques :
#include <stdio.h>
#include <stdlib.h>
#include <gtk/gtk.h>
#include <string.h>

// Fichiers headers des autres fonctions :
#include "structures.h"
#include "verifications.h"
#include "resoudre.h"
#include "fichiers.h"
#include "generer.h"
#include "aide.h"
#include "impression.h"

// Déclaration des variables
static int niveau = 2;
static int modeEdition =0;
static int grille[9][9];
static int grilleInitiale[9][9];
static int grilleTemp[9][9];

// Déclaration des widgets principaux du programme
static GtkWidget* fenetre;
static GtkWidget* grilleEcran[9][9];

// Focntions de gestion l'interface
static void menuEvenement(GtkWidget* widget, gpointer data);//gestion du menu
static void boutonVerif(GtkWidget* widget, gpointer data);//gestion du bouton vérifier

//fonction permettant synchroniser la partie graphique et la partie algorithmique
void synchroniserGrilleAffichage(int sens, int blocage);

int main (int argc, char *argv[])
{

    /* Création des différents widgets nécessaires */

    GtkWidget *verticalBox;//boétes qui contiendront les différents widgets
    GtkWidget *horizontalBox;
    GtkWidget *separateur;
    GtkWidget *bouton; //bouton résoudre
    GtkWidget *menuFichier, *menuAide, *menuNiveau; // widgets des menus
    GtkWidget *barreDeMenu;
    GtkWidget *menuItem;

    /*Création et initialisations des labels de la barre de menu*/

    const char* fichier[] = {"Générer une grille", "Grille vide", "Ouvrir une grille", "Enregistrer", "Imprimer dans un fichier", "Quitter"};
    const char* aide[] = {"Résoudre la grille courante", "Triche", "A propos"};
    const char* Niveau[] = {"Niveau 1", "Niveau 2", "Niveau 3"};

    const gchar* labelConv; //Nécessaire pour la conversion des labels en utf8 et résoudre le probléme des mots accentués

    int i, j;

    int grille[9][9] = {0}; //on initialise la grille, en la remplissant de 0

     gtk_init(&argc, &argv); // on initialise gtk

    /*Création de la fenétre*/

    fenetre=gtk_window_new(GTK_WINDOW_TOPLEVEL);//on initialise la fenétre principale du programme
    gtk_window_set_title(GTK_WINDOW(fenetre), "Sudocool !");
    gtk_window_set_icon_from_file(GTK_WINDOW(fenetre),"icon.ico",NULL); // on insére une icéne pour le programme
    gtk_window_set_resizable (GTK_WINDOW(fenetre), FALSE); // on interdt le redimensionnement par l'utilisateur

    //verticalBox = gtk_vbox_new(0,10); // on initialise le widget boéte verticale
    verticalBox = gtk_vbox_new(0,10); // on initialise le widget boéte verticale

    /*Création de la barre de menu et des menus qui la composent*/

    barreDeMenu=gtk_menu_bar_new(); //on initialise la barre de menu
    menuFichier=gtk_menu_new(); // et les menus
    menuAide=gtk_menu_new();
    menuNiveau=gtk_menu_new();

    //menu fichier
    menuItem=gtk_menu_item_new_with_label("Fichier");
    gtk_menu_item_set_submenu(GTK_MENU_ITEM(menuItem), menuFichier);//on définit menuFcihier en tant que sous menu
    gtk_menu_shell_append(GTK_MENU_SHELL(barreDeMenu), menuItem);//On ajoute les items de haut en bas
    //on utilise une boucle pour définir les éléments et gagner du temps
    for ( i=0; i <6 ; i++)
        {
            labelConv = g_locale_to_utf8(fichier[i], -1, NULL, NULL, NULL); // Conversion en utf8 des chaénes de caractéres
            menuItem=gtk_menu_item_new_with_label(labelConv); //on initialise l'item du menu et on lui assigne un label
            gtk_menu_shell_append(GTK_MENU_SHELL(menuFichier), menuItem); // on ajoute les éléments dans le menu de haut en bas
            g_signal_connect(menuItem,"activate",G_CALLBACK(menuEvenement), NULL);//on connecte le signal é la fonction callback menuEvenement ( voir plus bas )
        }

    //menu niveau
    menuItem=gtk_menu_item_new_with_label("Niveau");
    gtk_menu_item_set_submenu(GTK_MENU_ITEM(menuItem), menuNiveau);
    gtk_menu_shell_append(GTK_MENU_SHELL(barreDeMenu), menuItem);
    for ( i=0; i <3 ; i++)
        {
           labelConv = g_locale_to_utf8(Niveau[i], -1, NULL, NULL, NULL);
            menuItem=gtk_menu_item_new_with_label(labelConv);
            gtk_menu_shell_append(GTK_MENU_SHELL(menuNiveau), menuItem);
            g_signal_connect(menuItem,"activate",G_CALLBACK(menuEvenement), NULL);
        }

    //menu aide
    menuItem=gtk_menu_item_new_with_label("Aide");
    gtk_menu_item_set_submenu(GTK_MENU_ITEM(menuItem), menuAide);
    gtk_menu_shell_append(GTK_MENU_SHELL(barreDeMenu), menuItem);
    for ( i=0; i <3 ; i++)
        {
           labelConv = g_locale_to_utf8(aide[i], -1, NULL, NULL, NULL);
            menuItem=gtk_menu_item_new_with_label(labelConv);
            gtk_menu_shell_append(GTK_MENU_SHELL(menuAide), menuItem);
            g_signal_connect(menuItem,"activate",G_CALLBACK(menuEvenement), NULL);
        }

    gtk_box_pack_start(GTK_BOX(verticalBox), barreDeMenu, 0,0,0); //on insére la barre de menu dans la boéte verticale par le haut

    /*Création de la grille */
    for (i =0; i<9; i++)
    {
        horizontalBox = gtk_hbox_new(0,10); // on initialise le widget boéte horizontale
        for (j=0;j<9;j++)
        {
                grilleEcran[i][j]=gtk_entry_new();//on initialise les "cases"
                gtk_entry_set_max_length(GTK_ENTRY(grilleEcran[i][j]),1); //on limite la taille des gtk_entry é un seul élément !
                gtk_widget_set_size_request(grilleEcran[i][j],30,30); // définition de la taille des gtk_entry

                /*On va ensuite créer les régions en séparant artificiellement les cases*/
                gtk_box_pack_start(GTK_BOX(horizontalBox),grilleEcran[i][j],0,0,0);
                if((j+1)%3==0 && j < 8) //pour ne pas avoir un 3é séparateur
                        { separateur = gtk_vseparator_new();//separateur vertical
                        gtk_box_pack_start(GTK_BOX(horizontalBox),separateur,0,0,0);}
        }
        gtk_box_pack_start(GTK_BOX(verticalBox),horizontalBox,0,0,0);
        if((i+1)%3==0 && i < 8)
                {separateur = gtk_hseparator_new();
                gtk_box_pack_start(GTK_BOX(verticalBox),separateur,0,0,0);}
    }

    /*Création du bouton de vérification */

    labelConv = g_locale_to_utf8("Vérifier ma grille", -1, NULL, NULL, NULL); // Conversion en utf8 du label
    bouton=gtk_button_new_with_label(labelConv); //on initialise le bouton et on lui assigne un label
    g_signal_connect(bouton, "clicked", G_CALLBACK(boutonVerif), NULL);//on connecte le signal é la fonction qui gére le bouton
    gtk_box_pack_start(GTK_BOX(verticalBox),bouton,0,0,0);

    g_signal_connect(fenetre, "destroy", G_CALLBACK(gtk_main_quit), NULL); // on connecte le signal é la fonction gtk_main_quit pour permettre l'arrét du programme

    gtk_container_add(GTK_CONTAINER(fenetre), verticalBox);//on ajoute la boéte verticale dans la  fenétre
    gtk_widget_show_all(fenetre); // on affiche la fenetre
    gtk_main();//boucle de fonctionnement de gtk

    return EXIT_SUCCESS;
}

/*Fonction de gestion du menu */
/*Le fonctionnement est plutét simple, on récupére le signal envoyé par l'item de menu et on compare son label avec différentes chaénes de caractére pour savoir
sur quel item l'utilisateur a cliqué et lancer ainsi la bonne fonction*/
static void menuEvenement(GtkWidget *widget, gpointer data)
{
    /* Générer une grille */

    if(strcmp(gtk_menu_item_get_label(GTK_MENU_ITEM(widget)),g_locale_to_utf8("Générer une grille", -1, NULL, NULL, NULL))==0) //on utilise cette fonction de la librairie string.h pour effectuer la comparaison
    {
        modeEdition=0;
        generer(grille, niveau); // on fait appel é générer grille en envoyant le niveau  souhaité ( de 1 é 3 )
        copier_grille(grille,grilleInitiale);//on sauvegarde la grille initiale
        synchroniserGrilleAffichage(0,1);//on bloque les cases générées
    }

    /*Grille vide*/
    /*Réinitialise la grille é 0 ppour que l'utilisateur puisse créer la sienne */

    else if(strcmp(gtk_menu_item_get_label(GTK_MENU_ITEM(widget)),"Grille vide") == 0 )
    {

                    int i,j;
                    for (i=0;i<9;i++)
                    {
                        for (j=0;j<9;j++)
                        {
                            grille[i][j]=0;
                            gtk_entry_set_editable (GTK_ENTRY(grilleEcran[i][j]),TRUE);//on s'arrure que les cases soient éditables ( ds le cas ou on clique aprés une ouverture de grille)
                            gtk_entry_set_text(GTK_ENTRY(grilleEcran[i][j]), "0");//on affiche le caractére 0
                        }
                    }
                    copier_grille(grille,grilleInitiale);
                    modeEdition=1;
    }

     /*Ouvrir une grille*/
     /*Permet é l'utilisateur d'ouvrir un fichier contenant une grille et l'afficher é l'écran*/
    else if(strcmp(gtk_menu_item_get_label(GTK_MENU_ITEM(widget)),"Ouvrir une grille")==0)
    {
        modeEdition=0;
       GtkWidget *boiteDeDialogue=NULL;//on crée un widget qui sera initialisé en boéte de dialogue de choix de fichier (voire intruction suivante)
       boiteDeDialogue = gtk_file_chooser_dialog_new("Ouvrir une grille", GTK_WINDOW(fenetre),GTK_FILE_CHOOSER_ACTION_OPEN,GTK_STOCK_OK, GTK_RESPONSE_OK,GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL,NULL);
       gtk_widget_show_all(boiteDeDialogue);//on affiche le widget boite de dialogue

       //on teste la réponse de l'utilisateur, si il clique sur ok, on ouvre le fichier demander et on l'affiche é l'écran
       gint reponse=gtk_dialog_run(GTK_DIALOG(boiteDeDialogue));
       if(reponse == GTK_RESPONSE_OK)
            {
                char *nomFichier;//on utilise ce pointeur pour stocker le nom du fichier que l'utilisateur a choisi
                nomFichier = gtk_file_chooser_get_filename (GTK_FILE_CHOOSER (boiteDeDialogue)); // on récupére ce nom

                lecture(grille, nomFichier);//on envoie é la fonction lecture la grille ainsi que le nom. Le fichier est lu et la grille remplie
                synchroniserGrilleAffichage(0,1);//on synchronise l'affichage (grilleEcran) avec la grille . Cette fonction est expliquée plus bas
            }

       gtk_widget_destroy(boiteDeDialogue);//on détruit la boite de dialogue
       copier_grille(grille, grilleInitiale);
    }

     /*Enregistrer une grille*/
     /*Permet é l'utilisateur d'enregistrer une grille affichée é l'écran dans un fichier.*/
    else if(strcmp(gtk_menu_item_get_label(GTK_MENU_ITEM(widget)),"Enregistrer")==0)
    {
         modeEdition=0;
        synchroniserGrilleAffichage(1,0);//on synchronise la grille avec l'affichage
       if (verif_valide(grille)==1) //on vérifie que la grilleest juste avant d'enregistrer
       {
            GtkWidget *boiteDeDialogue=NULL;//le widget sera initialisé en boéte de dialogue de choix de fichier pour sauvergarder
            boiteDeDialogue = gtk_file_chooser_dialog_new ("Sauvegarder le fichier", NULL,GTK_FILE_CHOOSER_ACTION_SAVE, GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL, GTK_STOCK_SAVE, GTK_RESPONSE_ACCEPT, NULL);
            gtk_widget_show_all(boiteDeDialogue);
            gint reponse=gtk_dialog_run(GTK_DIALOG(boiteDeDialogue));//comme avant on teste a réponse
            if(reponse == GTK_RESPONSE_ACCEPT)
            {
                char *nomFichier;
                nomFichier = gtk_file_chooser_get_filename (GTK_FILE_CHOOSER (boiteDeDialogue));
                synchroniserGrilleAffichage(1,0); //on synchronise la grille avec l'affichage
                enregistrer(grille, nomFichier);//on envoie le nom du fichier et la grille é la fonction qui enregistre le contenu de la grille dans un fichier
            }
            gtk_widget_destroy(boiteDeDialogue);
       }
       else //sinon on affiche un widget qui informe l'utilisateur du probléme pour qu'il corrige sa saisie
        {
            GtkWidget *faux;//on crée un widget qui sera initialisé en boéte de dialogue d'information(instru. suivante), pour la conversion des labels, voire plus haut
            faux = gtk_message_dialog_new(GTK_WINDOW(fenetre),GTK_DIALOG_MODAL,GTK_MESSAGE_ERROR,GTK_BUTTONS_OK, g_locale_to_utf8("\nIl y a une erreur dans la grille.\nOn n'enregistre pas une grille fausse !", -1, NULL, NULL, NULL) );
            gtk_widget_show_all(faux);
            gtk_dialog_run(GTK_DIALOG(faux));//on lance le dialogue
            gtk_widget_destroy(faux);
        }
    }

    /*Imprimer dans un fichier*/
    /*Permet é l'utilisateur d'enregistrer une grille affichée é l'écran dans un fichier sous un format adapté é l'impression. Les différences avec la fonction précédente sont
     l'appel é impression au lieu d'enregistrer et l'ajout de l'extension .txt pour améliorer la compatibilité avec les éditeurs de textes sous windows.*/
    else if(strcmp(gtk_menu_item_get_label(GTK_MENU_ITEM(widget)),"Imprimer dans un fichier")==0)
    {
         modeEdition=0;
       GtkWidget *boiteDeDialogue=NULL;
       boiteDeDialogue = gtk_file_chooser_dialog_new ("Imprimer dans un fichier", NULL,GTK_FILE_CHOOSER_ACTION_SAVE, GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL, GTK_STOCK_SAVE, GTK_RESPONSE_ACCEPT, NULL);
       gtk_widget_show_all(boiteDeDialogue);
       gint reponse=gtk_dialog_run(GTK_DIALOG(boiteDeDialogue));
       if(reponse == GTK_RESPONSE_ACCEPT)
         {
             /*La grille doit étre imprimable dans  un éditeur de texte, sous windows il faut une extension .txt*/
                char *nomFichier, extension[]=".txt";
                nomFichier = gtk_file_chooser_get_filename (GTK_FILE_CHOOSER (boiteDeDialogue));
                strcat(nomFichier, extension);// on force donc l'extension .txt en concaténant le nom choisi par l'utilisateur avec l'extension
                synchroniserGrilleAffichage(1,0);
                impression(grille, nomFichier); //on envoie le nom du fichier et la grille é la fonction qui enregistre le contenu de la grille dans un fichier sous un format imprimable
        }
       gtk_widget_destroy(boiteDeDialogue);
    }

    /*Résoudre*/
    /*Permet é l'utilisateur de résoudre la grille en cours, si celle ci ne comporte pas de faute ou de l'indiquer si c'est le cas.*/
    else if(strcmp(gtk_menu_item_get_label(GTK_MENU_ITEM(widget)),g_locale_to_utf8("Résoudre la grille courante", -1, NULL, NULL, NULL))==0)
    {

            synchroniserGrilleAffichage(1,0);//on synchronise la grille avec l'affichage
            if(modeEdition ==1 ) copier_grille(grille, grilleInitiale);//pour ne pas empécher la résolution d'une grille vide
            copier_grille(grille, grilleTemp);
            resoudre(grilleTemp);
            resoudre(grilleInitiale);
            if (verif_valide(grille)==1 && comparer_grille(grilleInitiale, grilleTemp) == 1) //&& resoudre(grilleInitiale)==resoudre(grille))
            {
                resoudre(grille);// si la grille ne comporte pas d'erreur de l'utilisateur et est solvable, on la résoud
                synchroniserGrilleAffichage(0,1);//puis dans l'autre sens pour afficher la grille résolue
            }
            else if (verif_valide(grille)==1 && comparer_grille(grilleInitiale, grilleTemp) ==0)
            {
                GtkWidget *faux;//on crée un widget qui sera initialisé en boéte de dialogue d'information(instru. suivante), pour la conversion des labels, voire plus haut
                faux = gtk_message_dialog_new(GTK_WINDOW(fenetre),GTK_DIALOG_MODAL,GTK_MESSAGE_WARNING,GTK_BUTTONS_OK, g_locale_to_utf8("\nLa grille respecte les règles du sudoku\n mais elle ne correspond pas é la solution, elle n'est donc pas solvable.", -1, NULL, NULL, NULL) );
                gtk_widget_show_all(faux);
                gtk_dialog_run(GTK_DIALOG(faux));//on lance le dialogue
                gtk_widget_destroy(faux);
            }
            else //sinon on affiche un widget qui informe l'utilisateur du probléme pour qu'il corrige sa saisie
            {
                GtkWidget *faux;//on crée un widget qui sera initialisé en boéte de dialogue d'information(instru. suivante), pour la conversion des labels, voire plus haut
                faux = gtk_message_dialog_new(GTK_WINDOW(fenetre),GTK_DIALOG_MODAL,GTK_MESSAGE_ERROR,GTK_BUTTONS_OK, g_locale_to_utf8("\nIl y a une erreur dans la grille.\nAvec ces entrées elle est irrésolvable.", -1, NULL, NULL, NULL) );
                gtk_widget_show_all(faux);
                gtk_dialog_run(GTK_DIALOG(faux));//on lance le dialogue
                gtk_widget_destroy(faux);
            }

    }

    /*Triche*/
    /*Aide l'utilisateur quand il est bloqué.*/
    else if(strcmp(gtk_menu_item_get_label(GTK_MENU_ITEM(widget)),"Triche")==0)
    {

         modeEdition=0;
        synchroniserGrilleAffichage(1,0);
        aide(grille); // on fait un appel é la fonction d'aide qui renvoit une grille avec une case en plus
        synchroniserGrilleAffichage(0,0);//on synchronise avec l'affichage
    }

    /*A propos de sudocool*/
    else if(strcmp(gtk_menu_item_get_label(GTK_MENU_ITEM(widget)),"A propos")==0)
    {
        GtkWidget *Apropos;
        Apropos = gtk_message_dialog_new(GTK_WINDOW(fenetre),GTK_DIALOG_MODAL,GTK_MESSAGE_INFO,GTK_BUTTONS_OK, "Sudocool !\n\n"  "avec Gtk+\n" "par Antoine Faure et Arnaud Pecoraro" );
        gtk_widget_show_all(Apropos);
        gtk_dialog_run(GTK_DIALOG(Apropos));
        gtk_widget_destroy(Apropos);
    }
    /* On récupére ici le choix du niveau de l'utilisateur et on fait varier la valeur de niveau en conséquence.*/
    else if(strcmp(gtk_menu_item_get_label(GTK_MENU_ITEM(widget)),"Niveau 1")==0) niveau=1;
    else if(strcmp(gtk_menu_item_get_label(GTK_MENU_ITEM(widget)),"Niveau 2")==0) niveau=2;
    else if(strcmp(gtk_menu_item_get_label(GTK_MENU_ITEM(widget)),"Niveau 3")==0) niveau=3;
    /*Permet de quitter le programme*/
    else if(strcmp(gtk_menu_item_get_label(GTK_MENU_ITEM(widget)),"Quitter")==0) gtk_main_quit();
}

/*Fonction de gestion du bouton de vérification*/
/*Fonctionnement : on remplit la grille avec les éléments affichés é l'écran et on teste ensuite si cette nouvelle grille
est cohérente avec les régles du sudoku en appellant la fonction verif_valide. On ouvre ensuite une boéte de dialogue en fonction du résultat.*/
static void boutonVerif(GtkWidget* widget, gpointer data)
{
     modeEdition=0;

         GtkWidget *fin;
    int fini, valide;

    synchroniserGrilleAffichage(1,0);//on synchonise la grille avec l'affichage
    valide = verif_valide(grille); //on teste si la saisie est valide avec les régles du sudoku
    verif_fini(grille, &fini); //on vérifie si la grille est terminée

    /*Selon les situations, trois messages sont possibles*/
    if (fini == 1 && valide == 1) fin = gtk_message_dialog_new(GTK_WINDOW(fenetre),GTK_DIALOG_MODAL,GTK_MESSAGE_INFO,GTK_BUTTONS_OK, g_locale_to_utf8("\nBravo, la grille est résolue !", -1, NULL, NULL, NULL) );
    else if ( valide == 1 ) fin = gtk_message_dialog_new(GTK_WINDOW(fenetre),GTK_DIALOG_MODAL,GTK_MESSAGE_INFO,GTK_BUTTONS_OK, g_locale_to_utf8("\nPour l'instant c'est juste, continue !", -1, NULL, NULL, NULL) );
    else fin = gtk_message_dialog_new(GTK_WINDOW(fenetre),GTK_DIALOG_MODAL,GTK_MESSAGE_WARNING,GTK_BUTTONS_OK, "\nLa grille actuelle est fausse, encore un effort !" );

    gtk_widget_show_all(fin);
    gtk_dialog_run(GTK_DIALOG(fin));
    gtk_widget_destroy(fin);
}

/*Cette fonction permet le transfert des chiffres saisies par l'utilisateur dans le widget *grilleEcran[][] é la grille[][] d'entiers qui servira dans les algorithmes
ainsi que la possibilité de bloquer les cases de la grille*/
void synchroniserGrilleAffichage(int sens, int blocage) //elle prend comme paramétre le sens de la conversion et le blocage des cases quand c'est nécessaire
{
    if(sens==1) //affichage vers grille
    {
        int i,j;
        gchar *p;//on ne peut afficher que des caractéres é l'écran, une conversion est donc nécessaire
        for (i=0;i<9;i++)
        {
            for (j=0;j<9;j++)
            {
                int n;
                p = (char*) gtk_entry_get_text(GTK_ENTRY(grilleEcran[i][j])); // on récupére dans  p la chaéne de caractére affichée
               // if (p==NULL)n=0;
                /*else */n = atoi(p);//on utilise atoi pour la convertir en entier
                grille[i][j]=n; // on insére l'entier dans la grille
            }
        }
    }
    else if (sens == 0) //grille vers affichage
    {
        int i,j;
        char nombreCara[2];//on ne peut afficher que des caractéres é l'écran, une conversion est donc nécessaire
        for (i=0;i<9;i++)
        {
            for (j=0;j<9;j++)
            {
                gtk_entry_set_editable (GTK_ENTRY(grilleEcran[i][j]),TRUE);//on débloque
                snprintf(nombreCara, 2, "%i", grille[i][j]);//on convertit les entiers en caractéres
                gtk_entry_set_text(GTK_ENTRY(grilleEcran[i][j]), nombreCara);//on affiche les caractéres
                if((blocage==1) && (grille[i][j]!=0 )){gtk_entry_set_editable (GTK_ENTRY(grilleEcran[i][j]),FALSE);}//tout ce qui n'est pas 0 est bloqué
            }
        }
    }

}
