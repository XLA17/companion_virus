#include <gtk/gtk.h>

// Fonction callback appelée lors du clic sur le bouton
void on_convert_button_clicked(GtkWidget *button, gpointer data) {
    GtkEntry *entry = data;
    const char *entry_text = gtk_entry_get_text(entry);
    double km = atof(entry_text);
    double miles = km * 0.621371;
    char result[256];
    snprintf(result, sizeof(result), "%.2f Kilomètres = %.2f Miles", km, miles);
    gtk_button_set_label(GTK_BUTTON(button), result);
}

int main(int argc, char *argv[]) {
    gtk_init(&argc, &argv);

    GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "MonPG3");
    gtk_container_set_border_width(GTK_CONTAINER(window), 10);
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    GtkWidget *vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
    gtk_container_add(GTK_CONTAINER(window), vbox);

    GtkWidget *entry = gtk_entry_new();
    gtk_box_pack_start(GTK_BOX(vbox), entry, TRUE, TRUE, 0);

    GtkWidget *button = gtk_button_new_with_label("Convertir en Miles");
    g_signal_connect(button, "clicked", G_CALLBACK(on_convert_button_clicked), entry);
    gtk_box_pack_start(GTK_BOX(vbox), button, TRUE, TRUE, 0);

    gtk_widget_show_all(window);
    gtk_main();

    return 0;
}

