#include <gtk/gtk.h>
#include <stdio.h>

typedef struct {
    GtkWidget *label;
    guint timer_id;
    unsigned int count;
} Chronometer;

Chronometer chrono = {0};

// Fonction de mise à jour du chronomètre
gboolean update_chrono_label(gpointer data) {
    chrono.count++;
    unsigned int seconds = chrono.count / 10;
    unsigned int milliseconds = chrono.count % 10;
    char text[256];
    snprintf(text, sizeof(text), "%u.%u", seconds, milliseconds);
    gtk_label_set_text(GTK_LABEL(chrono.label), text);
    return TRUE; // Continue the timer
}

// Démarrer le chronomètre
void start_chrono(GtkWidget *widget, gpointer data) {
    if (!chrono.timer_id) {
        chrono.timer_id = g_timeout_add(100, update_chrono_label, NULL); // 100 ms = 0.1 second
    }
}

// Arrêter le chronomètre
void stop_chrono(GtkWidget *widget, gpointer data) {
    if (chrono.timer_id) {
        g_source_remove(chrono.timer_id);
        chrono.timer_id = 0;
    }
}

// Réinitialiser le chronomètre
void reset_chrono(GtkWidget *widget, gpointer data) {
    stop_chrono(widget, data);
    chrono.count = 0;
    gtk_label_set_text(GTK_LABEL(chrono.label), "0.0");
}

int main(int argc, char *argv[]) {
    gtk_init(&argc, &argv);

    GtkWidget *window, *vbox, *start_button, *stop_button, *reset_button;

    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "MonPG1");
    gtk_container_set_border_width(GTK_CONTAINER(window), 10);
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
    gtk_container_add(GTK_CONTAINER(window), vbox);

    chrono.label = gtk_label_new("0.0");
    gtk_box_pack_start(GTK_BOX(vbox), chrono.label, TRUE, TRUE, 0);

    start_button = gtk_button_new_with_label("Démarrer");
    g_signal_connect(start_button, "clicked", G_CALLBACK(start_chrono), NULL);
    gtk_box_pack_start(GTK_BOX(vbox), start_button, TRUE, TRUE, 0);

    stop_button = gtk_button_new_with_label("Arrêter");
    g_signal_connect(stop_button, "clicked", G_CALLBACK(stop_chrono), NULL);
    gtk_box_pack_start(GTK_BOX(vbox), stop_button, TRUE, TRUE, 0);

    reset_button = gtk_button_new_with_label("Réinitialiser");
    g_signal_connect(reset_button, "clicked", G_CALLBACK(reset_chrono), NULL);
    gtk_box_pack_start(GTK_BOX(vbox), reset_button, TRUE, TRUE, 0);

    gtk_widget_show_all(window);
    gtk_main();

    return 0;
}

