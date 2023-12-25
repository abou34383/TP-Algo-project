#include <gtk/gtk.h>
#include <glib.h>
typedef struct my_data {
  GtkWidget *window,*entry,*grid;
}data;
void   action2(int N , gpointer *window,gpointer *grid ){
  GtkWidget *entry,*label,*box;
  GtkWidget *button;

  int i;
    label = gtk_label_new ("");
    gtk_grid_attach (GTK_GRID (grid), label, 0, 1, 1, 1);
    label = gtk_label_new ("please complete the table with numbers :");
    gtk_grid_attach (GTK_GRID (grid), label, 0, 2, 1, 1);
  for (i = 1; i <=N; i++) {
    entry = gtk_entry_new ();
    gtk_grid_attach (GTK_GRID (grid), entry, i, 2, 1, 1);
  }

   box = gtk_box_new (GTK_ORIENTATION_HORIZONTAL, 10);
  button = gtk_button_new_with_label ("Ready");
  gtk_box_append (GTK_BOX (box), button);
   button = gtk_button_new_with_label ("clear");
   gtk_box_append (GTK_BOX (box), button);
  gtk_grid_attach (GTK_GRID (grid), box, 0, 3, 1, 1);
  gtk_window_present (GTK_WINDOW (window));
}
void action1 (GtkButton *button, data *data1)
{
        gint response;
        GtkWidget *dialog, *label, *content_area;
        GtkDialogFlags flags;
        int n;
      flags = GTK_DIALOG_DESTROY_WITH_PARENT;// th dialog box is here
      dialog = gtk_dialog_new_with_buttons ("Message", GTK_WINDOW (data1->window), flags, "_OK", GTK_RESPONSE_NONE, NULL);
      content_area = gtk_dialog_get_content_area (GTK_DIALOG (dialog));
      g_signal_connect_swapped (dialog, "response", G_CALLBACK (gtk_window_destroy), dialog);


      char *text = gtk_editable_get_text (GTK_EDITABLE (data1->entry));// scan the number
      int num;
      int result = sscanf (text, "%d", &num);
      if (result == 1)
      {
        if (num < 0)
        {
            label = gtk_label_new ("error n must be >0");
            printf ("The input is a negative integer: %d\n", num);
            gtk_box_append (GTK_BOX (content_area), label);
            gtk_widget_show (dialog);
        }
        else
        {
            n=num;
            action2(num,data1->window,data1->grid);
        }
      }
      else
      {
          label = gtk_label_new ("error n must be a positive number");
        printf ("The input is a text: %s\n", text);
        gtk_box_append (GTK_BOX (content_area), label);
        gtk_widget_show (dialog);
      }



}
static void activate (GtkApplication *app, gpointer user_data)
{
      GtkWidget *window;
      GtkWidget *grid,*grid1;
      GtkWidget *label;
      GtkWidget *entry;
      GtkWidget *button;
      data *data1;
      data1=(data *)malloc(sizeof(data));
      window = gtk_application_window_new (app);
      gtk_window_set_title (GTK_WINDOW (window), "Window");//the window is here
      grid = gtk_grid_new ();
      gtk_window_set_child (GTK_WINDOW (window), grid);
 grid1 = gtk_grid_new ();
      label = gtk_label_new ("please enter a valide positive number n=");
      entry = gtk_entry_new ();
      gtk_entry_set_max_length (GTK_ENTRY (entry), 5);
      button = gtk_button_new_with_label ("Enter");
      gtk_grid_attach (GTK_GRID (grid1), label, 0, 0, 1, 1);
      gtk_grid_attach (GTK_GRID (grid1), entry, 1, 0, 1, 1);
      gtk_grid_attach (GTK_GRID (grid1), button, 2, 0, 1, 1);

     gtk_grid_attach (GTK_GRID (grid), grid1, 0, 0, 1, 1);


        data1->window=window;
        data1->entry=entry;
        data1->grid=grid;
        g_signal_connect (button, "clicked", G_CALLBACK (action1),data1);


      gtk_window_present (GTK_WINDOW (window));

}

int main (int argc, char **argv)
{
  GtkApplication *app;
  int status;

  app = gtk_application_new ("org.gtk.example", G_APPLICATION_DEFAULT_FLAGS);
  g_signal_connect (app, "activate", G_CALLBACK (activate), NULL);
  status = g_application_run (G_APPLICATION (app), argc, argv);
  g_object_unref (app);

  return status;
}
