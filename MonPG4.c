#include <gtk/gtk.h>
#include <stdlib.h>

static void calculer_aire_et_perimetre(GtkWidget *widget, gpointer data) {
    // Conversion du pointeur de données pour accéder aux widgets
    GtkWidget **entrees = (GtkWidget **)data;
    const char *texte_largeur = gtk_entry_get_text(GTK_ENTRY(entrees[0]));
    const char *texte_hauteur = gtk_entry_get_text(GTK_ENTRY(entrees[1]));
    
    double largeur = atof(texte_largeur);
    double hauteur = atof(texte_hauteur);
    
    char resultat[256];
    if (largeur > 0 && hauteur > 0) {
        // Rectangle
        double aire = largeur * hauteur;
        double perimetre = 2 * (largeur + hauteur);
        snprintf(resultat, sizeof(resultat), "Rectangle - Aire : %.2f, Périmètre : %.2f", aire, perimetre);
    } else {
        snprintf(resultat, sizeof(resultat), "Veuillez entrer des dimensions valides !");
    }
    
    GtkWidget *dialogue = gtk_message_dialog_new(NULL, GTK_DIALOG_DESTROY_WITH_PARENT, GTK_MESSAGE_INFO, GTK_BUTTONS_OK, "%s", resultat);
    gtk_dialog_run(GTK_DIALOG(dialogue));
    gtk_widget_destroy(dialogue);
}

int main(int argc, char *argv[]) {
    gtk_init(&argc, &argv);

    GtkWidget *fenetre, *grille, *etiquette_largeur, *etiquette_hauteur, *entree_largeur, *entree_hauteur, *bouton_calculer;
    GtkWidget *entrees[2];

    fenetre = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(fenetre), "MonPG4");
    gtk_container_set_border_width(GTK_CONTAINER(fenetre), 10);
    g_signal_connect(fenetre, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    grille = gtk_grid_new();
    gtk_container_add(GTK_CONTAINER(fenetre), grille);

    etiquette_largeur = gtk_label_new("Largeur :");
    gtk_grid_attach(GTK_GRID(grille), etiquette_largeur, 0, 0, 1, 1);

    entree_largeur = gtk_entry_new();
    gtk_grid_attach(GTK_GRID(grille), entree_largeur, 1, 0, 2, 1);
    entrees[0] = entree_largeur;

    etiquette_hauteur = gtk_label_new("Hauteur :");
    gtk_grid_attach(GTK_GRID(grille), etiquette_hauteur, 0, 1, 1, 1);

    entree_hauteur = gtk_entry_new();
    gtk_grid_attach(GTK_GRID(grille), entree_hauteur, 1, 1, 2, 1);
    entrees[1] = entree_hauteur;

    bouton_calculer = gtk_button_new_with_label("Calculer");
    g_signal_connect(bouton_calculer, "clicked", G_CALLBACK(calculer_aire_et_perimetre), entrees);
    gtk_grid_attach(GTK_GRID(grille), bouton_calculer, 1, 2, 1, 1);

    gtk_widget_show_all(fenetre);
    gtk_main();

    return 0;
}

