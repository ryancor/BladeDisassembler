#include "../inc/gui.h"
#include "../inc/obfuscate.h"

void check_file_size(FILE *file1)
{
  int size;
  char *nSize = (char*)malloc(sizeof(char*)+1);

  if(file1 == NULL)
  {
    fileNotExist();
    exit(-1);
  }

  fseek(file1, 0, SEEK_END);
  size = ftell(file1);
  fseek(file1, 0, SEEK_SET);

  if(size >= 8000)
  {
    snprintf(nSize, sizeof(size),"%d", size);
    fileSizeLarge(nSize);
    exit(-1);
  }
  free(nSize);
}

void modeselect(GtkWidget *widget, gpointer view)
{
  char file_array2[8192];
  FILE *file1, *file2;
  char *line = NULL;
  size_t len = 0;
  ssize_t read;
  char file_array1[8192];
  gchar *gbuf;
  GtkTextBuffer *buffer_from_view;

  buffer_from_view = gtk_text_view_get_buffer(GTK_TEXT_VIEW(view));

  gchar *text = gtk_combo_box_get_active_text(GTK_COMBO_BOX(widget));

  if(strcmp(text, "OLSR MODE") == 0)
  {
    UNHIDE_STRING(filename);
    file1 = fopen(filename, "r");
    HIDE_STRING(filename);

    check_file_size(file1);

    while((read = getline(&line, &len, file1)) != -1)
    {
      strcat(file_array1, line);
    }
    gbuf = g_strdup_printf("%s", (gchar *)file_array1);

    fclose(file1);

    gtk_text_buffer_set_text(buffer_from_view, gbuf, -1);
  }
}

void show_gui_init(int argc, char *argv[])
{
  GtkWidget *window;
  GtkWidget *view;
  GtkWidget *frame;
  GtkWidget *frame2;
  GtkWidget *vbox;
  GtkWidget *vbox2;
  GtkWidget *hbox;
  GtkWidget *label;
  GtkWidget *label2;
  GtkWidget *combo;

  GtkTextBuffer *buffer;

  gtk_init(&argc, &argv);

  window = gtk_window_new(GTK_WINDOW_TOPLEVEL);

  gtk_window_set_default_size(GTK_WINDOW(window), 650, 600);
  gtk_window_set_title(GTK_WINDOW(window), "Blade Disassembler");
  gtk_container_set_border_width(GTK_CONTAINER(window), 5);
  GTK_WINDOW(window)->allow_shrink = TRUE;

  frame = gtk_fixed_new();
  frame2 = gtk_fixed_new();
  vbox = gtk_vbox_new(FALSE, 0);
  vbox2 = gtk_vbox_new(FALSE, 0);
  hbox = gtk_hbox_new(FALSE, 0);

  combo = gtk_combo_box_new_text();
  gtk_combo_box_append_text(GTK_COMBO_BOX(combo), "--Pick One--");
  gtk_combo_box_append_text(GTK_COMBO_BOX(combo), "OLSR MODE");

  view = gtk_text_view_new();
  gtk_widget_set_size_request(view, 0, 550);
  gtk_text_view_set_editable(GTK_TEXT_VIEW (view), FALSE);
  gtk_box_pack_start(GTK_BOX(vbox2), view, TRUE, TRUE, 0);
  buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(view));

  label = gtk_label_new( NULL);
  gtk_widget_set_size_request(label, 650, 500);

  label2 = gtk_label_new("Select the mode");

  gtk_fixed_put(GTK_FIXED(frame), label, 10, 10);

  gtk_fixed_put(GTK_FIXED(frame2), label2, 20, 25);
  gtk_fixed_put(GTK_FIXED(frame2), combo, 200, 20);

  gtk_box_pack_start(GTK_BOX(hbox), frame2, TRUE, TRUE, 0);
  gtk_box_pack_start(GTK_BOX(vbox), vbox2, TRUE, TRUE, 0);
  gtk_box_pack_start(GTK_BOX(vbox), hbox, TRUE, TRUE, 0);

  gtk_container_add(GTK_CONTAINER(window), vbox);

  g_signal_connect_swapped(G_OBJECT(window), "destroy",
                            G_CALLBACK(gtk_main_quit), G_OBJECT(window));

  g_signal_connect(G_OBJECT (combo),"changed",
                            G_CALLBACK(modeselect),(gpointer) view);

  gtk_widget_show_all(window);

  gtk_main();
}
