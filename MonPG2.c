#include <stdio.h>
#include <gtk/gtk.h>
#include <math.h>

typedef struct {
    int annee;
    int chomeurs;
} DonneesChomage;

DonneesChomage donnees_2008_2012[] = {
    {2008, 2600000},
    {2009, 3500000},
    {2010, 3750000},
    {2011, 4200000},
    {2012, 4500000}
};

DonneesChomage donnees_2013_2017[] = {
    {2013, 4600000},
    {2014, 4600000},
    {2015, 4200000},
    {2016, 3800000},
    {2017, 3500000}
};

DonneesChomage donnees_chomage[] = {
    {2018, 3000000},
    {2019, 3100000},
    {2020, 3400000},
    {2021, 2600000},
    {2022, 2100000}
};

int current_dataset_index = 0;
DonneesChomage *datasets[] = {donnees_2008_2012, donnees_2013_2017, donnees_chomage};
int dataset_sizes[] = {5, 5, 5};
GtkWidget *drawing_area;

#define COLOR_RED 1.0
#define COLOR_GREEN 0.0
#define COLOR_BLUE 0.0

void change_dataset(GtkWidget *widget, gpointer data) {
    int index = GPOINTER_TO_INT(data); // Convertit le pointeur en entier
    current_dataset_index = index; // Met à jour l'index de l'ensemble de données courant
    gtk_widget_queue_draw(GTK_WIDGET(drawing_area)); // Redessine la zone de dessin
}

static void draw_chart(GtkWidget *widget, cairo_t *cr, gpointer data) {
    DonneesChomage *current_dataset = datasets[current_dataset_index];
    int nb_annees = dataset_sizes[current_dataset_index];
    int i;
    int width, height, usable_height;
    double max_chomeurs = 0.0;
    double bar_width, bar_height, percentage_change;
    double x, y;

    const int top_margin = 30;
    const int bottom_margin = 60;
    const int percentage_label_height = 15;

    width = gtk_widget_get_allocated_width(widget);
    height = gtk_widget_get_allocated_height(widget);
    usable_height = height - (top_margin + bottom_margin + percentage_label_height);

    for (i = 0; i < nb_annees; ++i) {
        if (current_dataset[i].chomeurs > max_chomeurs) {
            max_chomeurs = current_dataset[i].chomeurs;
        }
    }

    bar_width = width / (2 * nb_annees);

    cairo_text_extents_t extents;

    for (i = 0; i < nb_annees; ++i) {
        bar_height = ((double)current_dataset[i].chomeurs / max_chomeurs) * usable_height;
        x = (2 * i + 1) * (width / (2 * nb_annees)) - bar_width / 2;
        y = height - bottom_margin - bar_height;

        cairo_set_source_rgb(cr, COLOR_RED, COLOR_GREEN, COLOR_BLUE);
        cairo_rectangle(cr, x, y, bar_width, bar_height);
        cairo_fill(cr);

        char year_text[5];
        sprintf(year_text, "%d", current_dataset[i].annee);
        cairo_set_source_rgb(cr, 0, 0, 0);
        cairo_text_extents(cr, year_text, &extents);
        cairo_move_to(cr, x + (bar_width - extents.width) / 2, height - bottom_margin + 20);
        cairo_show_text(cr, year_text);

        char chomeurs_text[10];
        sprintf(chomeurs_text, "%d", current_dataset[i].chomeurs);
        cairo_text_extents(cr, chomeurs_text, &extents);
        cairo_move_to(cr, x + (bar_width - extents.width) / 2, height - bottom_margin + 35);
        cairo_show_text(cr, chomeurs_text);

        if (i > 0) {
            percentage_change = 100.0 * (current_dataset[i].chomeurs - current_dataset[i - 			1].chomeurs) / (double)current_dataset[i - 1].chomeurs;
                        cairo_text_extents(cr, year_text, &extents);
            char percentage_text[16];
            snprintf(percentage_text, sizeof(percentage_text), "%.1f%%", 			   fabs(percentage_change));
            cairo_move_to(cr, x + (bar_width - extents.width) / 2, y - 10);
            if (percentage_change > 0) {
                cairo_set_source_rgb(cr, 0, 0.5, 0); // Vert pour les augmentations
                cairo_show_text(cr, g_strdup_printf("+%s", percentage_text));
            } else {
                cairo_set_source_rgb(cr, 0.5, 0, 0); // Rouge pour les diminutions
                cairo_show_text(cr, percentage_text);
            }
        }
    }

    // Dessiner le titre du graphique
    cairo_set_source_rgb(cr, 0.0, 0.0, 0.0); // Texte en noir
    cairo_select_font_face(cr, "Sans", CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_BOLD);
    cairo_set_font_size(cr, 18);
    cairo_text_extents(cr, "Nombre de chômeurs en France au fil des années", &extents);
    cairo_move_to(cr, (width - extents.width) / 2, top_margin - 10);
    cairo_show_text(cr, "Nombre de chômeurs en France au fil des années");
}

int main(int argc, char *argv[]) {
    GtkWidget *window, *button_2008_2012, *button_2013_2017, *button_2018_2022, *box;

    gtk_init(&argc, &argv);

    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "monpg2");
    gtk_window_set_default_size(GTK_WINDOW(window), 800, 600);
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
    gtk_container_add(GTK_CONTAINER(window), box);

    // Création et configuration des boutons pour chaque période
    button_2008_2012 = gtk_button_new_with_label("2008-2012");
    g_signal_connect(button_2008_2012, "clicked", G_CALLBACK(change_dataset), GINT_TO_POINTER(0));
    gtk_box_pack_start(GTK_BOX(box), button_2008_2012, FALSE, FALSE, 0);

    button_2013_2017 = gtk_button_new_with_label("2013-2017");
    g_signal_connect(button_2013_2017, "clicked", G_CALLBACK(change_dataset), GINT_TO_POINTER(1));
    gtk_box_pack_start(GTK_BOX(box), button_2013_2017, FALSE, FALSE, 0);

    button_2018_2022 = gtk_button_new_with_label("2018-2022");
    g_signal_connect(button_2018_2022, "clicked", G_CALLBACK(change_dataset), GINT_TO_POINTER(2));
    gtk_box_pack_start(GTK_BOX(box), button_2018_2022, FALSE, FALSE, 0);

    drawing_area = gtk_drawing_area_new();
    gtk_box_pack_start(GTK_BOX(box), drawing_area, TRUE, TRUE, 0);
    g_signal_connect(G_OBJECT(drawing_area), "draw", G_CALLBACK(draw_chart), NULL);
    gtk_widget_set_size_request(drawing_area, 800, 550);

    gtk_widget_show_all(window);
    gtk_main();

    return 0;
}

