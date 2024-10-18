#include <gtk/gtk.h>
#include <vte/vte.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

GtkWidget *package_list;
GtkWidget *details_label;
GtkWidget *terminal;
GtkWidget *toggle_terminal_button;
GtkWidget *search_entry;
GtkListStore *list_store;
gboolean terminal_visible = FALSE;

void on_package_selected(GtkTreeView *tree_view, GtkTreePath *path, GtkTreeViewColumn *col, gpointer user_data);
void on_install_clicked(GtkWidget *widget, gpointer data);
void on_uninstall_clicked(GtkWidget *widget, gpointer data);
void on_toggle_terminal_clicked(GtkWidget *widget, gpointer data);
void on_search_clicked(GtkWidget *widget, gpointer data);

void perform_search(const char *query) {
    char command[256];
    FILE *fp;
    char line[1024];
    GtkTreeIter iter;
    
    snprintf(command, sizeof(command), "yay -Ss %s", query);
    
    gtk_list_store_clear(list_store);
    
    fp = popen(command, "r");
    if (fp == NULL) {
        perror("Failed to run yay command");
        return;
    }

    // Read all lines and store them
    GArray *lines = g_array_new(FALSE, FALSE, sizeof(char*));
    while (fgets(line, sizeof(line), fp) != NULL) {
        char *line_copy = g_strdup(line);
        g_array_append_val(lines, line_copy);
    }
    pclose(fp);

    // Reverse the order and add to list store
    for (int i = lines->len - 1; i >= 0; i--) {
        char *current_line = g_array_index(lines, char*, i);
        if (current_line[0] != ' ') {  // This is a package name line
            gtk_list_store_append(list_store, &iter);
            gtk_list_store_set(list_store, &iter, 0, current_line, -1);
        }
        g_free(current_line);
    }
    g_array_free(lines, TRUE);
}

void on_search_clicked(GtkWidget *widget, gpointer data) {
    const char *query = gtk_entry_get_text(GTK_ENTRY(search_entry));
    perform_search(query);
}

void on_package_selected(GtkTreeView *tree_view, GtkTreePath *path, GtkTreeViewColumn *col, gpointer user_data) {
    GtkTreeModel *model;
    GtkTreeIter iter;
    char *selected_package;

    model = gtk_tree_view_get_model(tree_view);
    if (gtk_tree_model_get_iter(model, &iter, path)) {
        gtk_tree_model_get(model, &iter, 0, &selected_package, -1);
        
        // Extract package name from the line
        char *package_name = strtok(selected_package, "/");
        if (package_name != NULL) {
            char command[256];
            char details[1024] = "";
            FILE *fp;

            snprintf(command, sizeof(command), "yay -Si %s", package_name);
            fp = popen(command, "r");
            if (fp != NULL) {
                while (fgets (details, sizeof(details), fp) != NULL) {
                    gtk_label_set_text(GTK_LABEL(details_label), details);
                }
                pclose(fp);
            }
        }
        g_free(selected_package);
    }
}

void on_install_clicked(GtkWidget *widget, gpointer data) {
    GtkTreeModel *model;
    GtkTreeIter iter;
    char *selected_package;

    model = gtk_tree_view_get_model(GTK_TREE_VIEW(package_list));
    if (gtk_tree_model_get_iter(model, &iter, gtk_tree_view_get_cursor(GTK_TREE_VIEW(package_list)))) {
        gtk_tree_model_get(model, &iter, 0, &selected_package, -1);
        
        // Extract package name from the line
        char *package_name = strtok(selected_package, "/");
        if (package_name != NULL) {
            char command[256];
            snprintf(command, sizeof(command), "yay -S %s", package_name);
            gtk_terminal_feed_child(GTK_TERMINAL(terminal), command, -1);
            g_free(package_name);
        }
        g_free(selected_package);
    }
}

void on_uninstall_clicked(GtkWidget *widget, gpointer data) {
    GtkTreeModel *model;
    GtkTreeIter iter;
    char *selected_package;

    model = gtk_tree_view_get_model(GTK_TREE_VIEW(package_list));
    if (gtk_tree_model_get_iter(model, &iter, gtk_tree_view_get_cursor(GTK_TREE_VIEW(package_list)))) {
        gtk_tree_model_get(model, &iter, 0, &selected_package, -1);
        
        // Extract package name from the line
        char *package_name = strtok(selected_package, "/");
        if (package_name != NULL) {
            char command[256];
            snprintf(command, sizeof(command), "yay -R %s", package_name);
            gtk_terminal_feed_child(GTK_TERMINAL(terminal), command, -1);
            g_free(package_name);
        }
        g_free(selected_package);
    }
}

int main(int argc, char *argv[]) {
    gtk_init(&argc, &argv);

    GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "Package Manager");

    GtkWidget *vbox = gtk_vbox_new(FALSE, 0);
    gtk_container_add(GTK_CONTAINER(window), vbox);

    GtkWidget *hbox = gtk_hbox_new(FALSE, 0);
    gtk_box_pack_start(GTK_BOX(vbox), hbox, FALSE, FALSE, 0);

    search_entry = gtk_entry_new();
    gtk_box_pack_start(GTK_BOX(hbox), search_entry, TRUE, TRUE, 0);

    GtkWidget *search_button = gtk_button_new_with_label("Search");
    gtk_box_pack_start(GTK_BOX(hbox), search_button, FALSE, FALSE, 0);
    g_signal_connect(search_button, "clicked", G_CALLBACK(on_search_clicked), NULL);

    package_list = gtk_tree_view_new();
    gtk_box_pack_start(GTK_BOX(vbox), package_list, TRUE, TRUE, 0);

    GtkCellRenderer *renderer = gtk_cell_renderer_text_new();
    GtkTreeViewColumn *column = gtk_tree_view_column_new_with_attributes("Package", renderer, "text", 0, NULL);
    gtk_tree_view_append_column(GTK_TREE_VIEW(package_list), column);

    list_store = gtk_list_store_new(1, G_TYPE_STRING);
    gtk_tree_view_set_model(GTK_TREE_VIEW(package_list), GTK_TREE_MODEL(list_store));
    g_signal_connect(package_list, "row-activated", G_CALLBACK(on_package_selected), NULL);

    GtkWidget *hbox2 = gtk_hbox_new(FALSE, 0);
    gtk_box_pack_start(GTK_BOX(vbox), hbox2, FALSE, FALSE, 0);

    GtkWidget *install_button = gtk_button_new_with_label("Install");
    gtk_box_pack_start(GTK_BOX(hbox2), install_button, FALSE, FALSE, 0);
    g_signal_connect(install_button, "clicked", G_CALLBACK(on_install_clicked), NULL);

    GtkWidget *uninstall_button = gtk_button_new_with_label("Uninstall");
    gtk_box_pack_start(GTK_BOX(hbox2), uninstall_button, FALSE, FALSE, 0);
    g_signal_connect(uninstall_button, "clicked", G_CALLBACK(on_uninstall_clicked), NULL);

    GtkWidget *toggle_terminal_button = gtk_button_new_with_label("Toggle Terminal");
    gtk_box_pack_start(GTK_BOX(hbox2), toggle_terminal_button, FALSE, FALSE, 0);
    g_signal_connect(toggle_terminal_button, "clicked", G_CALLBACK(on_toggle_terminal_clicked), NULL);

    terminal = gtk_terminal_new();
    gtk_box_pack_start(GTK_BOX(vbox), terminal, TRUE, TRUE, 0);
    gtk_widget_set_visible(terminal, FALSE);

    details_label = gtk_label_new("");
    gtk_box_pack_start(GTK_BOX(vbox), details_label, FALSE, FALSE, 0);

    gtk_widget_show_all(window);

    gtk_main();

    return 0;
}