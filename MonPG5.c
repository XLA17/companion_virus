#include <gtk/gtk.h>
#include <math.h>

typedef struct {
    GtkWidget *entry_weight;
    GtkWidget *entry_height;
    GtkWidget *label_result;
} BMIAppWidgets;

static void on_calculate_clicked(GtkWidget *widget, gpointer data) {
    BMIAppWidgets *appWidgets = (BMIAppWidgets *)data;

    const char *weight_text = gtk_entry_get_text(GTK_ENTRY(appWidgets->entry_weight));
    const char *height_text = gtk_entry_get_text(GTK_ENTRY(appWidgets->entry_height));

    double weight = atof(weight_text);
    double height = atof(height_text) / 100.0; // Convertir cm en mètres

    double bmi = 0.0;
    char *category = "";
    char *color = "black"; // Couleur par défaut

    if (height > 0) {
        bmi = weight / (height * height);
        if (bmi < 16.0) {
            category = "Anorexie ou maigreur";
            color = "blue";
        } else if (bmi >= 16.0 && bmi < 18.5) {
            category = "Maigreur";
            color = "lightblue";
        } else if (bmi >= 18.5 && bmi < 25.0) {
            category = "Corpulence normale";
            color = "green";
        } else if (bmi >= 25.0 && bmi < 30.0) {
            category = "Surpoids";
            color = "orange";
        } else if (bmi >= 30.0 && bmi < 35.0) {
            category = "Obésité modérée";
            color = "darkorange";
        } else if (bmi >= 35.0 && bmi < 40.0) {
            category = "Obésité sévère";
            color = "#FF0000"; // rouge foncé pour l'obésité sévère
        } else {
            category = "Obésité morbide ou massive";
            color = "#A00000"; // un rouge encore plus foncé pour l'obésité morbide
        }
    } else {
        category = "Hauteur invalide";
        color = "red";
    }

    char result_str[256];
    snprintf(result_str, sizeof(result_str), "Votre IMC : %.2f\n<span foreground='%s'>Catégorie : %s</span>", bmi, color, category);
    
    // Utiliser des balises Pango pour la couleur
    gtk_label_set_markup(GTK_LABEL(appWidgets->label_result), result_str);
}


int main(int argc, char *argv[]) {
    gtk_init(&argc, &argv);

    GtkWidget *window, *grid, *calculate_button;
    BMIAppWidgets appWidgets;

    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "MonPG5");
    gtk_window_set_default_size(GTK_WINDOW(window), 300, 200);
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    grid = gtk_grid_new();
    gtk_container_add(GTK_CONTAINER(window), grid);

    appWidgets.entry_weight = gtk_entry_new();
    appWidgets.entry_height = gtk_entry_new();
    appWidgets.label_result = gtk_label_new("");
    
    gtk_grid_attach(GTK_GRID(grid), gtk_label_new("Poids (kg):"), 0, 0, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), appWidgets.entry_weight, 1, 0, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), gtk_label_new("Taille (cm):"), 0, 1, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), appWidgets.entry_height, 1, 1, 1, 1);

    calculate_button = gtk_button_new_with_label("Calculer");
    g_signal_connect(calculate_button, "clicked", G_CALLBACK(on_calculate_clicked), 		&appWidgets);
    gtk_grid_attach(GTK_GRID(grid), calculate_button, 0, 2, 2, 1);
    gtk_grid_attach(GTK_GRID(grid), appWidgets.label_result, 0, 3, 2, 1);

    gtk_widget_show_all(window);
    gtk_main();

    return 0;
}

