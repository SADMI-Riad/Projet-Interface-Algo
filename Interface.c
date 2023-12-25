#include <gtk/gtk.h>
#include <pango/pango.h>
#include <stdio.h>
#include <stdlib.h>
// declaration globale psq utilisithm un peu partout dans le code
GtkWidget *window;
GtkWidget *vbox;
GtkWidget *grid;
int taille = 0;
int tableau[100];
int currentCase=0;

//en cours de traitement psq nzid el button sup 
// typedef struct {
//     int valeur;
//     GtkWidget *label;
// } ElementData;

// hadi bch yaffichiw gae les cases fiha appel lel fonction li rahi ththa
void afficherTableau();
// fonction li tcreeyi grille case par case
void afficherCase();
//donner main a l'utilisateurs pour faire en sorte howa y3mr b les valeurs li raw habhm plus la taille du tableau
void remplirTableau();





int main(int argc, char *argv[])
{

    gtk_init(&argc, &argv);

    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "Tableau GTK");
    gtk_window_set_default_size(GTK_WINDOW(window), 400, 400);
    gtk_container_set_border_width(GTK_CONTAINER(window), 10);

    vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
    gtk_container_add(GTK_CONTAINER(window), vbox);

    GtkWidget *label = gtk_label_new("Saisir la taille du tableau :");
    GtkWidget *entry = gtk_entry_new();
    gtk_container_add(GTK_CONTAINER(vbox), label);
    gtk_container_add(GTK_CONTAINER(vbox), entry);

    GtkWidget *buttonRemplir = gtk_button_new_with_label("Remplir le tableau");
    g_signal_connect(G_OBJECT(buttonRemplir), "clicked", G_CALLBACK(remplirTableau), entry);
    gtk_container_add(GTK_CONTAINER(vbox), buttonRemplir);

    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    gtk_widget_show_all(window);

    gtk_main();

    return 0;
}

void remplirTableau(GtkWidget *widget, gpointer data) {
    GtkWidget *entry = GTK_WIDGET(data);
    const char *text = gtk_entry_get_text(GTK_ENTRY(entry));
    taille = atoi(text);

    if (taille <= 0) {
        printf("Erreur: la taille du tableau doit être un entier strictement positif.\n");
        return;
    }

    for (int i = 0; i < taille; ++i) {
        GtkWidget *dialog;
        dialog = gtk_dialog_new_with_buttons("Saisie de valeur", GTK_WINDOW(window), GTK_DIALOG_MODAL, "_OK", GTK_RESPONSE_OK, "_Cancel", GTK_RESPONSE_CANCEL, NULL);

        GtkWidget *content_area = gtk_dialog_get_content_area(GTK_DIALOG(dialog));

        GtkWidget *entryValue = gtk_entry_new();
        gtk_container_add(GTK_CONTAINER(content_area), entryValue);
        gtk_widget_show(entryValue);

        gint result = gtk_dialog_run(GTK_DIALOG(dialog));

        if (result == GTK_RESPONSE_OK) {
            const char *textValue = gtk_entry_get_text(GTK_ENTRY(entryValue));
            sscanf(textValue, "%d", &tableau[i]);
        }

        gtk_widget_destroy(dialog);
    }

    afficherTableau(tableau, taille);
}

void afficherCase()
{
if (currentCase < taille) {
        char label_text[50];
        snprintf(label_text, sizeof(label_text), "%d", tableau[currentCase]);

        GtkWidget *label = gtk_label_new(label_text);
        PangoAttrList *attr_list = pango_attr_list_new();
        PangoAttribute *attr = pango_attr_weight_new(PANGO_WEIGHT_BOLD);
        pango_attr_list_insert(attr_list, attr);
        gtk_label_set_attributes(GTK_LABEL(label), attr_list);
        pango_attr_list_unref(attr_list);

        GtkWidget *box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
        gtk_box_pack_start(GTK_BOX(box), label, TRUE, TRUE, 0);

        int cell_size = 50;
        gtk_widget_set_size_request(box, cell_size, cell_size);

        GtkWidget *frame = gtk_frame_new(NULL);
        gtk_frame_set_shadow_type(GTK_FRAME(frame), GTK_SHADOW_IN);
        gtk_container_add(GTK_CONTAINER(frame), box);

        gtk_grid_attach(GTK_GRID(grid), frame, currentCase % 5, currentCase / 5, 1, 1);

        GdkRGBA red;
        gdk_rgba_parse(&red, "#FFDDDD");
        gtk_widget_override_background_color(box, GTK_STATE_FLAG_NORMAL, &red);

        gtk_widget_show_all(window);

        
        currentCase++;

        //n3yt lel la meme fonction avec un delai 
        g_timeout_add(1250, (GSourceFunc)afficherCase, NULL);
    }
}

void afficherTableau(const int *tableau, int taille) {
   GList *children, *iter;

    children = gtk_container_get_children(GTK_CONTAINER(vbox));

    for (iter = children; iter != NULL; iter = g_list_next(iter)) {
        gtk_widget_destroy(GTK_WIDGET(iter->data));
    }

    g_list_free(children);

    grid = gtk_grid_new();
    gtk_grid_set_row_spacing(GTK_GRID(grid), 5);
    gtk_grid_set_column_spacing(GTK_GRID(grid), 1);

    // nrj3ha men debut
    currentCase = 0;

    // dok n3yt lel fonction bch taffichili case par case
    afficherCase();

    gtk_container_add(GTK_CONTAINER(vbox), grid);

    gtk_widget_show_all(window);
}