#include "client.h"

void uchat_stickerbar_sticker_new(t_main_struct *main_struct, gint column, gint row) {
    GtkWidget *stickerbar_inner_button;
    GtkWidget *stickerbar_inner_fixed;
    GtkWidget *stickerbar_inner_label;
    GtkWidget *stickerbar_inner_image;

    stickerbar_inner_button = gtk_button_new();
    gtk_widget_set_name(stickerbar_inner_button, "stickerbar_inner_button");

    stickerbar_inner_fixed = gtk_fixed_new();
    gtk_widget_set_name(stickerbar_inner_fixed, "stickerbar_inner_fixed");

    gchar *path = NULL;
    gchar *filename = NULL;
    gchar *codename = NULL;

    codename = strjoin(codename, itoa(column));
    codename = strjoin(codename, "_");
    codename = strjoin(codename, itoa(row));

    filename = strjoin(codename, ".png");

    path = strjoin(path, "resource/images/stickers/");
    path = strjoin(path, filename);

    stickerbar_inner_label = gtk_label_new(codename);
    gtk_widget_set_name(stickerbar_inner_label, "stickerbar_inner_label");

    stickerbar_inner_image = gtk_image_new_from_pixbuf(gdk_pixbuf_new_from_file_at_scale(path, 52, 52, FALSE, NULL));
    gtk_widget_set_name(stickerbar_inner_image, "stickerbar_inner_image");

    g_signal_connect(stickerbar_inner_button, "clicked", G_CALLBACK(uchat_send_sticker_message), main_struct);

    gtk_fixed_put(GTK_FIXED(stickerbar_inner_fixed), stickerbar_inner_label, 0, 0);
    gtk_fixed_put(GTK_FIXED(stickerbar_inner_fixed), stickerbar_inner_image, 0, 0);

    gtk_container_add(GTK_CONTAINER(stickerbar_inner_button), stickerbar_inner_fixed);
    gtk_grid_attach(GTK_GRID(main_struct->stickerbar_scrolled_grid), stickerbar_inner_button, column, row, 1, 1);

    strdel(&codename);
    strdel(&filename);
    strdel(&path);
}
