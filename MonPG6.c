#include <gtk/gtk.h>

// Structure pour regrouper toutes les données nécessaires pour les callbacks
typedef struct {
    GtkEntry *entry1;
    GtkEntry *entry2;
    GtkEntry *entry_result;
} CalculatorData;

// Fonction générique pour les opérations
static void on_operation_clicked(GtkWidget *widget, gpointer data) {
    CalculatorData *calc_data = (CalculatorData *)data;

    // Pas de changement pour la récupération des textes des entrées
    const char *entry_text1 = gtk_entry_get_text(calc_data->entry1);
    const char *entry_text2 = gtk_entry_get_text(calc_data->entry2);

    // Convertir les textes des entrées en nombres
    double value1 = atof(entry_text1);
    double value2 = atof(entry_text2); // Cette ligne n'est plus nécessaire si on n'utilise pas value2 pour les calculs

    double result = 0.0;
    gboolean error = FALSE; // Pour gérer les erreurs, comme la division par zéro

    // Calculer le résultat en fonction de l'opérateur et de la valeur du premier champ
    switch (calc_data->last_operation) {
        case '+':
            result = calc_data->previous_result + value1;
            break;
        case '-':
            result = calc_data->previous_result - value1;
            break;
        case 'x':
            result = calc_data->previous_result * value1;
            break;
        case '/':
            if (value1 != 0) {
                result = calc_data->previous_result / value1;
            } else {
                gtk_entry_set_text(calc_data->entry2, "Error: Division by zero"); // Modifier ici pour afficher l'erreur dans entry2
                return;
            }
            break;
    }

    // Si pas d'erreur, afficher le résultat dans le deuxième champ de saisie
    if (!error) {
        char result_str[128];
        snprintf(result_str, sizeof(result_str), "%.2f", result);
        gtk_entry_set_text(calc_data->entry2, result_str); // Affichage du résultat dans entry2
    }

    // Mettre à jour le résultat précédent avec la nouvelle valeur calculée
    calc_data->previous_result = result;

    // Mise à jour de l'opération pour la prochaine itération
    calc_data->last_operation = gtk_button_get_label(GTK_BUTTON(widget))[0];
}

int main(int argc, char *argv[]) {
    gtk_init(&argc, &argv);

    GtkWidget *window, *grid, *entry1, *entry2, *entry_result;
    GtkWidget *add_button, *sub_button, *mul_button, *div_button;
    CalculatorData calc_data;

    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "MonPG6");
    gtk_window_set_default_size(GTK_WINDOW(window), 250, 150);
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    grid = gtk_grid_new();
    gtk_container_add(GTK_CONTAINER(window), grid);

    entry1 = gtk_entry_new();
    gtk_grid_attach(GTK_GRID(grid), entry1, 0, 0, 1, 1);
    calc_data.entry1 = GTK_ENTRY(entry1);

    entry2 = gtk_entry_new();
    gtk_grid_attach(GTK_GRID(grid), entry2, 0, 1, 1, 1);
    calc_data.entry2 = GTK_ENTRY(entry2);

    entry_result = gtk_entry_new();
    gtk_editable_set_editable(GTK_EDITABLE(entry_result), FALSE);
    gtk_grid_attach(GTK_GRID(grid), entry_result, 0, 4, 2, 1);
    calc_data.entry_result = GTK_ENTRY(entry_result);

    // Ajouter un bouton et le connecter à la fonction de rappel pour l'addition
    add_button = gtk_button_new_with_label("+");
    g_signal_connect(add_button, "clicked", G_CALLBACK(on_operation_clicked), &calc_data);
    gtk_grid_attach(GTK_GRID(grid), add_button, 1, 0, 1, 1);

    // Bouton de soustraction
	sub_button = gtk_button_new_with_label("-");
	g_signal_connect(sub_button, "clicked", G_CALLBACK(on_operation_clicked), &calc_data);
	gtk_grid_attach(GTK_GRID(grid), sub_button, 1, 1, 1, 1);

// Bouton de multiplication
	mul_button = gtk_button_new_with_label("x");
	g_signal_connect(mul_button, "clicked", G_CALLBACK(on_operation_clicked), &calc_data);
	gtk_grid_attach(GTK_GRID(grid), mul_button, 1, 2, 1, 1);

	// Bouton de division
	div_button = gtk_button_new_with_label("/");
	g_signal_connect(div_button, "clicked", G_CALLBACK(on_operation_clicked), &calc_data);
	gtk_grid_attach(GTK_GRID(grid), div_button, 1, 3, 1, 1);

	// Afficher tous les widgets de la fenêtre
	gtk_widget_show_all(window);

	// Lancer la boucle principale
	gtk_main();

	return 0;
}


   

