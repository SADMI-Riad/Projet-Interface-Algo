#include <gtk/gtk.h>


void on_button_clicked(GtkWidget *widget, gpointer data)
{
    g_print("Bouton cliqué !\n");
}

int main(int argc, char *argv[])
{

    gtk_init(&argc, &argv);


    GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "Exemple GTK en C");
    gtk_container_set_border_width(GTK_CONTAINER(window), 10);
    gtk_widget_set_size_request(window, 300, 200);
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);


    GtkWidget *button = gtk_button_new_with_label("Cliquez-moi !");
    g_signal_connect(button, "clicked", G_CALLBACK(on_button_clicked), NULL);


    gtk_container_add(GTK_CONTAINER(window), button);


    gtk_widget_show(button);
    gtk_widget_show(window);


    gtk_main();

    return 0;
}
