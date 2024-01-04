#include <gtk/gtk.h>
#include <stdio.h>
#include <stdlib.h>
// declaration globale psq utilisithm un peu partout dans le code
#define MAX_TAILLE 200

GtkWidget *labels[MAX_TAILLE];
GtkWidget *window;
GtkWidget *vbox;
GtkWidget *grid;
GtkWidget *labelStatus;
GtkCssProvider *provider; // bch n9dr nst3ml css
int taille = 0;
int tableau[MAX_TAILLE];
int currentCase=0;

gboolean effectuerUnePermutationcrois(gpointer data) ;
gboolean effectuerUnePermutationdecrois(gpointer data);
void commencerTri(GtkWidget *widget, gpointer data) ;
void changerCouleurFond(GtkWidget *label, const char *couleur);

void supprimerValeurOuPositionInterface(GtkWidget *widget, gpointer data);
void supprimerValeurOuPosition(int mode, int value) ;
//fonction li tajouti la valeur
void ajoutervaleur(GtkWidget *widget , gpointer data);
// hadi bch yaffichiw gae les cases fiha appel lel fonction li rahi ththa
void afficherTableau(const int *tableau , int taille);
// fonction li tcreeyi grille case par case
void afficherCase();
//donner main a l'utilisateurs pour faire en sorte howa y3mr b les valeurs li raw habhm plus la taille du tableau
void remplirTableau(GtkWidget *widget , gpointer data);
// en cour de traitement et modification
GtkWidget * creer_label_anime(const char *text);
// function poour apply css
void application_css(GtkWidget *widget,GtkStyleProvider *provider);
// bch n'ameliori encore plus
gboolean transitionColor(gpointer box_ptr);
gboolean fadeInLabel(gpointer label_ptr);

void recommencerApplication(GtkWidget *widget, gpointer data);

//nsgm el bug de compilation
static gboolean draw_background(GtkWidget *widget, cairo_t *cr, gpointer data);


int main(int argc, char *argv[])
{

    gtk_init(&argc, &argv);

    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "Simulation Tri par Insertion tableau");
    gtk_window_set_default_size(GTK_WINDOW(window), 700, 500);
    gtk_container_set_border_width(GTK_CONTAINER(window), 15);

    vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    gtk_container_add(GTK_CONTAINER(window), vbox);

    GtkWidget *label = gtk_label_new("Saisir la taille du tableau :");
    GtkWidget *entry = gtk_entry_new();
    gtk_container_add(GTK_CONTAINER(vbox), label);
    gtk_container_add(GTK_CONTAINER(vbox), entry);

    GtkWidget *buttonRemplir = gtk_button_new_with_label("Remplir le tableau");
    g_signal_connect(G_OBJECT(buttonRemplir), "clicked", G_CALLBACK(remplirTableau), entry);
    gtk_container_add(GTK_CONTAINER(vbox), buttonRemplir);
    //nzid btn quitter
    GtkWidget *quitBtn = gtk_button_new_with_label("Quitter");
    gtk_container_add(GTK_CONTAINER(vbox), quitBtn);  
    g_signal_connect(G_OBJECT(quitBtn), "clicked", G_CALLBACK(gtk_main_quit), NULL);
    labelStatus = gtk_label_new("");
    gtk_container_add(GTK_CONTAINER(vbox), labelStatus);

    provider = gtk_css_provider_new();
    gtk_css_provider_load_from_data(provider,
        "window { background-color: #EEE; }"  
        "button, .button { border-radius: 8px; padding: 10px; background: #AED581; color: #FFF; }"
        "button:hover, .button:hover { background: #81C784; cursor:pointer;}", -1, NULL);
    application_css(GTK_WIDGET(window), GTK_STYLE_PROVIDER(provider)); 

    gtk_widget_show_all(window);

    gtk_main();

    return 0;
}

void remplirTableau(GtkWidget *widget, gpointer data) {
    GtkWidget *entry = GTK_WIDGET(data);
    const char *text = gtk_entry_get_text(GTK_ENTRY(entry));
    taille = atoi(text);

    if (taille <= 0 || taille> MAX_TAILLE) {
        printf("Erreur: la taille du tableau doit être un entier strictement positif.\n");
        return;
    }

    for (int i = 0; i < taille; ++i) {
        GtkWidget *dialog;
        dialog = gtk_dialog_new_with_buttons("Saisie de valeur", GTK_WINDOW(window), GTK_DIALOG_MODAL, "_OK", GTK_RESPONSE_OK, "_Retour", GTK_RESPONSE_CANCEL, NULL);

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
if (currentCase < taille ) {
        char label_text[50];
        snprintf(label_text, sizeof(label_text), "%d", tableau[currentCase]);
        GtkWidget *label = creer_label_anime(label_text);
        //animation
        gtk_widget_set_opacity(label, 0.0);
        g_timeout_add(100, fadeInLabel, label);
        g_timeout_add(50, transitionColor, label);

        GtkWidget *box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
        gtk_box_pack_start(GTK_BOX(box), label, TRUE, TRUE, 0);
        g_signal_connect(G_OBJECT(box), "draw", G_CALLBACK(draw_background), NULL);

        int cell_size = 65;
        gtk_widget_set_size_request(box, cell_size, cell_size);
        labels[currentCase] = label;
        GtkWidget *frame = gtk_frame_new(NULL);
        gtk_frame_set_shadow_type(GTK_FRAME(frame), GTK_SHADOW_OUT);
        gtk_container_add(GTK_CONTAINER(frame), box);
        g_signal_connect(G_OBJECT(frame), "draw", G_CALLBACK(draw_background), NULL);
        gtk_grid_attach(GTK_GRID(grid), frame, currentCase % 10, currentCase / 10, 1, 1);

        gtk_widget_show_all(window);

        
        currentCase++;

    }
    else
    {
        return;
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
    gtk_grid_set_row_spacing(GTK_GRID(grid), 20);
    gtk_grid_set_column_spacing(GTK_GRID(grid), 0);
    gtk_widget_set_halign(grid, GTK_ALIGN_CENTER);
    gtk_widget_set_valign(grid, GTK_ALIGN_CENTER);
    gtk_container_add(GTK_CONTAINER(vbox),grid);

    // nrj3ha men debut
    currentCase = 0;


    //create the div btn
    GtkWidget *buttonBox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);
    gtk_box_set_homogeneous(GTK_BOX(buttonBox), TRUE); 
    // dok n3yt lel fonction bch taffichili case par case
    g_timeout_add(1000, (GSourceFunc)afficherCase, NULL);
    //nnzid le button tae ajouter valeur w nconnectih mea el fonction 
    GtkWidget *addbtn= gtk_button_new_with_label("Ajouter une valeur au Tableau");
    g_signal_connect(G_OBJECT(addbtn), "clicked", G_CALLBACK(ajoutervaleur), NULL);
    gtk_container_add(GTK_CONTAINER(buttonBox),addbtn);
    //nzid btn quitte hna tan 
    GtkWidget *quitBtn = gtk_button_new_with_label("Quitter");
    gtk_container_add(GTK_CONTAINER(buttonBox), quitBtn);  
    g_signal_connect(G_OBJECT(quitBtn), "clicked", G_CALLBACK(gtk_main_quit), NULL);
    //restart btn
    GtkWidget *buttonRecommencer = gtk_button_new_with_label("Recommencer");
    g_signal_connect(G_OBJECT(buttonRecommencer), "clicked", G_CALLBACK(recommencerApplication), NULL);
    gtk_box_pack_start(GTK_BOX(buttonBox), buttonRecommencer, TRUE, TRUE, 0);
    //sup btn
    GtkWidget *supbtn = gtk_button_new_with_label("supprimer valeur");
    gtk_container_add(GTK_CONTAINER(buttonBox), supbtn);  
    g_signal_connect(G_OBJECT(supbtn), "clicked",G_CALLBACK(supprimerValeurOuPositionInterface), NULL);
    //tri btn
    GtkWidget *sortBtn = gtk_button_new_with_label("Trier le tableau");
    g_signal_connect(G_OBJECT(sortBtn), "clicked", G_CALLBACK(commencerTri), NULL);
    gtk_container_add(GTK_CONTAINER(buttonBox), sortBtn);
    //css the btn
    provider=gtk_css_provider_new();
    gtk_css_provider_load_from_data(provider,
    "window { background-color: #EEE; }"  
    "button, .button { border-radius: 8px; padding: 10px; background: #AED581; color: #FFF; }"
    "button:hover, .button:hover { background: #81C784; cursor:pointer;}", -1, NULL);
    
    application_css(GTK_WIDGET(buttonBox), GTK_STYLE_PROVIDER(provider)); 

    gtk_box_pack_end(GTK_BOX(vbox), buttonBox, FALSE, FALSE, 0);
    gtk_widget_show_all(window);
}

GtkWidget * creer_label_anime(const char *text)
{
    GtkWidget *label = gtk_label_new(text);
    g_signal_connect(G_OBJECT(label), "draw", G_CALLBACK(draw_background), NULL);
    return label;
}


void application_css(GtkWidget *widget, GtkStyleProvider *provider) {
    gtk_style_context_add_provider(gtk_widget_get_style_context(widget), provider, G_MAXUINT);
    if (GTK_IS_CONTAINER(widget)) {
        gtk_container_forall(GTK_CONTAINER(widget), (GtkCallback)application_css, provider);
    }
}

void ajoutervaleur(GtkWidget *widget , gpointer data)
{
    if (taille >= MAX_TAILLE)
    {
        gtk_label_set_text(GTK_LABEL(labelStatus),"Le tableau est plein!");
        return;
    }
GtkWidget *dialog,*valeurEntry,*positionEntry,*content_area;
dialog=gtk_dialog_new_with_buttons("Ajouter une valeur",GTK_WINDOW(window),GTK_DIALOG_MODAL,"_OK",GTK_RESPONSE_OK,"_Retour",GTK_RESPONSE_CANCEL,NULL);
content_area = gtk_dialog_get_content_area(GTK_DIALOG(dialog));
    valeurEntry = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(valeurEntry), "Valeur à ajouter");
    gtk_container_add(GTK_CONTAINER(content_area), valeurEntry);
    positionEntry = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(positionEntry), "Position (0-indexé)");
    gtk_container_add(GTK_CONTAINER(content_area), positionEntry);
    gtk_widget_show_all(dialog);
    gint result = gtk_dialog_run(GTK_DIALOG(dialog));
    if (result == GTK_RESPONSE_OK) {
        const char *valeurText = gtk_entry_get_text(GTK_ENTRY(valeurEntry));
        const char *positionText = gtk_entry_get_text(GTK_ENTRY(positionEntry));
        int valeur = atoi(valeurText);
        int position = atoi(positionText);
        if (position < 0 || position > taille) {
            gtk_label_set_text(GTK_LABEL(labelStatus), "Position invalide!");
        } else {
            for (int i = taille; i > position; --i) {
                tableau[i] = tableau[i - 1];
            }
            tableau[position] = valeur;
            taille++;
            currentCase = 0;
            afficherTableau(tableau, taille);
        }
    }
    gtk_widget_destroy(dialog);
    
}
static gboolean draw_background(GtkWidget *widget, cairo_t *cr, gpointer data) {
    GdkRGBA *color = g_object_get_data(G_OBJECT(widget), "custom-bg-color");
    if (color) {
        gdk_cairo_set_source_rgba(cr, color);
    } else {
        GdkRGBA default_color;
        gdk_rgba_parse(&default_color, "#AED581"); 
        gdk_cairo_set_source_rgba(cr, &default_color);
    }

    cairo_rectangle(cr, 0, 0, gtk_widget_get_allocated_width(widget), gtk_widget_get_allocated_height(widget));
    cairo_fill(cr);

    return FALSE;  
}
void recommencerApplication(GtkWidget *widget, gpointer data) {
    
    GList *children, *iter;
    children = gtk_container_get_children(GTK_CONTAINER(vbox));
    for (iter = children; iter != NULL; iter = g_list_next(iter)) {
        gtk_widget_destroy(GTK_WIDGET(iter->data));  // Détruire chaque enfant
    }
    g_list_free(children);

    
    taille = 0;
    currentCase = 0;

   
    GtkWidget *label = gtk_label_new("Entrez la taille du tableau :");
    GtkWidget *entry = gtk_entry_new();
    gtk_container_add(GTK_CONTAINER(vbox), label);
    gtk_container_add(GTK_CONTAINER(vbox), entry);

    GtkWidget *buttonRemplir = gtk_button_new_with_label("Remplir le tableau");
    g_signal_connect(G_OBJECT(buttonRemplir), "clicked", G_CALLBACK(remplirTableau), entry);
    gtk_container_add(GTK_CONTAINER(vbox), buttonRemplir);

    GtkWidget *quitBtn = gtk_button_new_with_label("Quitter");
    gtk_container_add(GTK_CONTAINER(vbox), quitBtn);  
    g_signal_connect(G_OBJECT(quitBtn), "clicked", G_CALLBACK(gtk_main_quit), NULL);

    labelStatus = gtk_label_new("");
    gtk_container_add(GTK_CONTAINER(vbox), labelStatus);
    provider = gtk_css_provider_new();
    gtk_css_provider_load_from_data(provider,
        "window { background-color: #EEE; }"
        "button, .button { border-radius: 8px; padding: 10px; background: #AED581; color: #FFF; }"
        "button:hover, .button:hover { background: #81C784; cursor : pointer; }", -1, NULL);
    application_css(GTK_WIDGET(window), GTK_STYLE_PROVIDER(provider));
    
    gtk_widget_show_all(window);
}
gboolean fadeInLabel(gpointer label_ptr) {
    static const double MAX_OPACITY = 1.0;
    static const double OPACITY_INCREMENT = 0.1;
    
    GtkWidget *label = GTK_WIDGET(label_ptr);
    double opacity = gtk_widget_get_opacity(label);
    
    if (opacity < MAX_OPACITY) {
        opacity += OPACITY_INCREMENT;
        gtk_widget_set_opacity(label, opacity);
        return TRUE; 
    }
    
    return FALSE; 
}
//chercher sur internet
gboolean transitionColor(gpointer label_ptr) {
    const double TARGET_RED = 129.0 / 255.0;
    const double TARGET_GREEN = 199.0 / 255.0;
    const double TARGET_BLUE = 132.0 / 255.0;
    static const double COLOR_INCREMENT = 0.1;

    GtkWidget *label = GTK_WIDGET(label_ptr);
    GdkRGBA *color = g_object_get_data(G_OBJECT(label), "custom-bg-color");

    if (!color) {

        color = g_new(GdkRGBA, 1);
        gdk_rgba_parse(color, "#81C784");  
        g_object_set_data_full(G_OBJECT(label), "custom-bg-color", color, (GDestroyNotify)gdk_rgba_free);
    }

    gboolean color_changed = FALSE;


    #ifndef MIN
    #define MIN(a, b) ((a) < (b) ? (a) : (b))
    #endif

    if (color->red < TARGET_RED) {
        color->red = MIN(color->red + COLOR_INCREMENT, TARGET_RED);
        color_changed = TRUE;
    }

    if (color->green < TARGET_GREEN) {
        color->green = MIN(color->green + COLOR_INCREMENT, TARGET_GREEN);
        color_changed = TRUE;
    }

    if (color->blue < TARGET_BLUE) {
        color->blue = MIN(color->blue + COLOR_INCREMENT, TARGET_BLUE);
        color_changed = TRUE;
    }

    if (color_changed) {
        gtk_widget_queue_draw(label); 
        return TRUE;  
    }

    return FALSE;  
}

void show_alert(const char *message) {
    GtkWidget *dialog = gtk_message_dialog_new(NULL,GTK_DIALOG_MODAL,GTK_MESSAGE_WARNING,GTK_BUTTONS_OK,"%s",message);

    gtk_dialog_run(GTK_DIALOG(dialog));
    gtk_widget_destroy(dialog);
}

void supprimerValeurOuPosition(int mode, int value) {
    if (taille <= 0) {
        g_print("Le tableau est déjà vide!\n");
        return;
    }

    if (mode == 0) {
        if (value < 0 || value >= taille) {
            g_print("Position invalide!\n");
            return;
        }
        for (int i = value; i < taille - 1; i++) {
            tableau[i] = tableau[i + 1];
        }
    } else if (mode == 1) {
        int found = 0; 
        for (int i = 0; i < taille; i++) {
            if (tableau[i] == value) {
                found++;
            } else {
                tableau[i - found] = tableau[i]; 
            }
        }
        taille -= found; 
    }

    afficherTableau(tableau, taille);
}

void supprimerValeurOuPositionInterface(GtkWidget *widget, gpointer data) {
    GtkWidget *dialog, *content_area, *modeComboBox, *valueEntry;
    GtkDialogFlags flags = GTK_DIALOG_MODAL | GTK_DIALOG_DESTROY_WITH_PARENT;

    dialog = gtk_dialog_new_with_buttons("Supprimer valeur ou position", GTK_WINDOW(window),
                                         flags, "_OK", GTK_RESPONSE_OK,
                                         "_Cancel", GTK_RESPONSE_CANCEL, NULL);
    content_area = gtk_dialog_get_content_area(GTK_DIALOG(dialog));

    
    modeComboBox = gtk_combo_box_text_new();
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(modeComboBox), "Position");
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(modeComboBox), "Valeur");
    gtk_combo_box_set_active(GTK_COMBO_BOX(modeComboBox), 0);  
    gtk_container_add(GTK_CONTAINER(content_area), modeComboBox);

    
    valueEntry = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(valueEntry), "Entrez la valeur ou la position");
    gtk_container_add(GTK_CONTAINER(content_area), valueEntry);

    gtk_widget_show_all(dialog);
    gint result = gtk_dialog_run(GTK_DIALOG(dialog));

    if (result == GTK_RESPONSE_OK) {
        int mode = gtk_combo_box_get_active(GTK_COMBO_BOX(modeComboBox));
        const char *valueText = gtk_entry_get_text(GTK_ENTRY(valueEntry));
        int value = atoi(valueText);
        supprimerValeurOuPosition(mode, value);
    }

    gtk_widget_destroy(dialog);
}

gboolean effectuerUnePermutationcrois(gpointer data) {
    static int i = 1, j = 0, phase = 0;

    if (i <= taille) {
        if (phase == 0) {
            
            if (j < 0 || tableau[j] <= tableau[j + 1]) {
                j = i - 1; 
                i++;       
            }

            if (j >= 0 && j<taille-1 && tableau[j] > tableau[j + 1]) {
                
                changerCouleurFond(labels[j], "#FFA07A");   
                changerCouleurFond(labels[j + 1], "#FFA07A"); 

                phase = 1; 
                g_timeout_add(500, effectuerUnePermutationcrois, NULL);
                return G_SOURCE_REMOVE; 
            }
        } else {
           
            int temp = tableau[j];
            tableau[j] = tableau[j + 1];
            tableau[j + 1] = temp;

            
            gchar *text = g_strdup_printf("%d", tableau[j]);
            gtk_label_set_text(GTK_LABEL(labels[j]), text);
            g_free(text);

            text = g_strdup_printf("%d", tableau[j + 1]);
            gtk_label_set_text(GTK_LABEL(labels[j + 1]), text);
            g_free(text);

            
            changerCouleurFond(labels[j], "#FFFFFF");  
            changerCouleurFond(labels[j + 1], "#FFFFFF"); 

            phase = 0; 
            j--; 
        }
    } else {
        
        gtk_label_set_text(GTK_LABEL(labelStatus), "Tri terminé!");
    for (int i = 0; i < taille; i++)
    {
        changerCouleurFond(labels[i], "#81C784");
    }
        i = 1; 
        j = 0; 
        phase = 0; 
        return G_SOURCE_REMOVE; 
    }
    
    return G_SOURCE_CONTINUE; 
    
}
gboolean effectuerUnePermutationdecrois(gpointer data) {
    static int i = 1, j = 0, phase = 0;

    if (i <= taille) {
        if (phase == 0) {
            
            if (j < 0 || tableau[j] >= tableau[j + 1]) { 
                j = i - 1; 
                i++;       
            }

            if (j >= 0 && j < taille - 1 && tableau[j] < tableau[j + 1]) { 
                
                changerCouleurFond(labels[j], "#FFA07A");   
                changerCouleurFond(labels[j + 1], "#FFA07A"); 

                phase = 1; 
                g_timeout_add(500, effectuerUnePermutationdecrois, NULL);
                return G_SOURCE_REMOVE; 
            }
        } else {
           
            int temp = tableau[j];
            tableau[j] = tableau[j + 1];
            tableau[j + 1] = temp;

            
            gchar *text = g_strdup_printf("%d", tableau[j]);
            gtk_label_set_text(GTK_LABEL(labels[j]), text);
            g_free(text);

            text = g_strdup_printf("%d", tableau[j + 1]);
            gtk_label_set_text(GTK_LABEL(labels[j + 1]), text);
            g_free(text);

            
            changerCouleurFond(labels[j], "#FFFFFF");  
            changerCouleurFond(labels[j + 1], "#FFFFFF"); 

            phase = 0; 
            j--; 
        }
    } else {
        
        gtk_label_set_text(GTK_LABEL(labelStatus), "Tri terminé!");
        for (int i = 0; i < taille; i++)
        {
            changerCouleurFond(labels[i], "#81C784");
        }
        i = 1; 
        j = 0; 
        phase = 0; 
        return G_SOURCE_REMOVE; 
    }
    
    return G_SOURCE_CONTINUE; 
    
}
void changerCouleurFond(GtkWidget *label, const char *couleur) {
    
    gchar *css = g_strdup_printf("label {background-color: %s;}", couleur);
    GtkCssProvider *provider = gtk_css_provider_new();
    gtk_css_provider_load_from_data(provider, css, -1, NULL);
    gtk_style_context_add_provider(gtk_widget_get_style_context(label),
                                   GTK_STYLE_PROVIDER(provider),
                                   GTK_STYLE_PROVIDER_PRIORITY_USER);
    g_free(css);
}
void commencerTri(GtkWidget *widget, gpointer data) {

    GtkWidget *dialog = gtk_dialog_new_with_buttons("Choix du Tri",
                                                    GTK_WINDOW(data),
                                                    GTK_DIALOG_MODAL,
                                                    "Tri Croissant",
                                                    GTK_RESPONSE_ACCEPT,
                                                    "Tri Décroissant",
                                                    GTK_RESPONSE_REJECT,
                                                    NULL);
    gint result = gtk_dialog_run(GTK_DIALOG(dialog));
    gtk_widget_destroy(dialog);
    if (result == GTK_RESPONSE_ACCEPT) {
        g_timeout_add(1000, (GSourceFunc)effectuerUnePermutationcrois, NULL);
    } else if (result == GTK_RESPONSE_REJECT) {
        g_timeout_add(1000, (GSourceFunc)effectuerUnePermutationdecrois, NULL);
    }
}
