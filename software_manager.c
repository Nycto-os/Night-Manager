#include <gtk/gtk.h>
#include <vte/vte.h>
#include <stdlib.h>

GtkWidget *package_list;
GtkWidget *details_label;
GtkWidget *terminal;
GtkWidget *toggle_terminal_button;
gboolean terminal_visible = FALSE; 

char *packages[] = {"App 1", "App 2", "App 3", "App 4"};
char *details[] = {
    "App 1: A simple app.\nVersion: 1.0.0\nSize: 10MB",
    "App 2: A game app.\nVersion: 2.1.0\nSize: 50MB",
    "App 3: A utility app.\nVersion: 0.9.3\nSize: 5MB",
    "App 4: A multimedia app.\nVersion: 3.0.5\nSize: 100MB"
};

void on_package_selected(GtkTreeView *tree_view, GtkTreePath *path, GtkTreeViewColumn *col, gpointer user_data) {
    GtkTreeModel *model;
    GtkTreeIter iter;
    char *selected_package;

    model = gtk_tree_view_get_model(tree_view);
    if (gtk_tree_model_get_iter(model, &iter, path)) {
        gtk_tree_model_get(model, &iter, 0, &selected_package, -1);

        // Find and display package details
        for (int i = 0; i < 4; i++) {
            if (g_strcmp0(selected_package, packages[i]) == 0) {
                gtk_label_set_text(GTK_LABEL(details_label), details[i]);
                break;
            }
        }
        g_free(selected_package);
    }
}

void on_install_clicked(GtkWidget *widget, gpointer data) {
    g_print("Installing package...\n");
    vte_terminal_feed_child(VTE_TERMINAL(terminal), "echo 'Installing package...'\n", -1);
  // real code to install packages, to be inserted here  ps: look into your gdocs to find more details.
}

// Uninstall button callback
void on_uninstall_clicked(GtkWidget *widget, gpointer data) {
    g_print("Uninstalling package...\n");
    vte_terminal_feed_child(VTE_TERMINAL(terminal), "echo 'Uninstalling package...'\n", -1);
    //likewise but for uninstall
}

// Toggle terminal visibility
void on_toggle_terminal_clicked(GtkWidget *widget, gpointer data) {
    terminal_visible = !terminal_visible;
    if (terminal_visible) {
        gtk_widget_show(terminal);
        gtk_button_set_label(GTK_BUTTON(toggle_terminal_button), "Hide Terminal");
    } else {
        gtk_widget_hide(terminal);
        gtk_button_set_label(GTK_BUTTON(toggle_terminal_button), "Show Terminal");
    }
}

void populate_package_list(GtkListStore *list_store) {
    GtkTreeIter iter;
    for (int i = 0; i < 4; i++) {
        gtk_list_store_append(list_store, &iter);
        gtk_list_store_set(list_store, &iter, 0, packages[i], -1);
    }
}

int main(int argc, char *argv[]) {
    GtkWidget *window, *vbox, *button_box, *install_button, *uninstall_button;
    GtkListStore *list_store;
    GtkCellRenderer *renderer;
    GtkTreeViewColumn *column;
    
    gtk_init(&argc, &argv);

    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "Software Manager");
    gtk_window_set_default_size(GTK_WINDOW(window), 600, 400);
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
    gtk_container_add(GTK_CONTAINER(window), vbox);

    list_store = gtk_list_store_new(1, G_TYPE_STRING);
    populate_package_list(list_store);

    // Create the package list view
    package_list = gtk_tree_view_new_with_model(GTK_TREE_MODEL(list_store));
    renderer = gtk_cell_renderer_text_new();
    column = gtk_tree_view_column_new_with_attributes("Available Packages", renderer, "text", 0, NULL);
    gtk_tree_view_append_column(GTK_TREE_VIEW(package_list), column);
    gtk_box_pack_start(GTK_BOX(vbox), package_list, TRUE, TRUE, 0);

    g_signal_connect(package_list, "row-activated", G_CALLBACK(on_package_selected), NULL);

    details_label = gtk_label_new("Select a package to view details.");
    gtk_box_pack_start(GTK_BOX(vbox), details_label, FALSE, FALSE, 0);

    button_box = gtk_button_box_new(GTK_ORIENTATION_HORIZONTAL);
    gtk_box_pack_start(GTK_BOX(vbox), button_box, FALSE, FALSE, 0);

    install_button = gtk_button_new_with_label("Install");
    g_signal_connect(install_button, "clicked", G_CALLBACK(on_install_clicked), NULL);
    gtk_container_add(GTK_CONTAINER(button_box), install_button);

    uninstall_button = gtk_button_new_with_label("Uninstall");
    g_signal_connect(uninstall_button, "clicked", G_CALLBACK(on_uninstall_clicked), NULL);
    gtk_container_add(GTK_CONTAINER(button_box), uninstall_button);

    terminal = vte_terminal_new();
    vte_terminal_spawn_async(VTE_TERMINAL(terminal), VTE_PTY_DEFAULT, NULL, NULL, NULL, 0, NULL, NULL, NULL, -1, NULL, NULL, NULL);
    gtk_box_pack_start(GTK_BOX(vbox), terminal, TRUE, TRUE, 0);
    gtk_widget_hide(terminal); 

    toggle_terminal_button = gtk_button_new_with_label("Show Terminal");
    g_signal_connect(toggle_terminal_button, "clicked", G_CALLBACK(on_toggle_terminal_clicked), NULL);
    gtk_box_pack_start(GTK_BOX(vbox), toggle_terminal_button, FALSE, FALSE, 0);

    
    gtk_widget_show_all(window);

    
    gtk_main();

    return 0;
}
