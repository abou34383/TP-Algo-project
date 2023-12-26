#include <gtk/gtk.h>
#include <glib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>

typedef struct my_data {
  GtkWidget *window,*entry,*grid;
}data;
typedef struct my_data_1 {
  GtkWidget *grid,*window,*button;
  int n;
}data_1;
typedef struct my_data_2 {
  GtkWidget **FT,**DAT,*grid;
  int n;
  float *T,max;

}data_2;
static void draw_rectangle (GtkDrawingArea *area, cairo_t *cr, int width, int height){
  cairo_set_source_rgb (cr, 0, 0, 1);
  cairo_rectangle (cr, 0, 0, width, height);
  cairo_fill (cr);
}

static void draw_rectangleB (GtkDrawingArea *area, cairo_t *cr, int width, int height){
  cairo_set_source_rgb (cr, 0, 0, 0);
  cairo_rectangle (cr, 0, 0, width, height);
  cairo_fill (cr);
}

void action4(GtkButton *button,data_2 *data2){
    bool p;
    int j,tt,i,num;
    char *text,*tempT;
    GtkWidget *fixed,*entry,*temp;
    GtkDrawingArea *t2;

    for(i=(data2->n)-2;i>-1;i--){
        for(j=0;j<=i;j++){

            if((data2->T)[j]>(data2->T)[j+1]){
                tt=(data2->T)[j];
                (data2->T)[j]=(data2->T)[j+1];
                (data2->T)[j+1]=tt;
                printf("%f ",(data2->T)[j]);
                printf("%f ",(data2->T)[j+1]);
                //change displayed numbers

                entry = gtk_grid_get_child_at (GTK_GRID (data2->grid), j+1, 2);
                sprintf (text, "%d", (int)(data2->T)[j]);
                gtk_editable_set_text (GTK_ENTRY (entry), text);
                entry = gtk_grid_get_child_at (GTK_GRID (data2->grid), j+2, 2);
                sprintf (text, "%d", (int)(data2->T)[j+1]);
                gtk_editable_set_text (GTK_ENTRY (entry), text);
                //change places

                gtk_widget_set_size_request ((data2->DAT)[j], 20, ((data2->T)[j]*10*(data2->max)));
                gtk_fixed_move (GTK_FIXED ((data2->FT)[j]), (data2->DAT)[j], 10, 150-(data2->T)[j]*10*(data2->max));
                gtk_widget_set_size_request ((data2->DAT)[j+1], 20, (data2->T)[j+1]*10*(data2->max));
                gtk_fixed_move (GTK_FIXED ((data2->FT)[j+1]), (data2->DAT)[j+1], 10, 150-(data2->T)[j+1]*10*(data2->max));
                usleep(5000);
            }

        }


    }


}

void action3(GtkButton *button,data_1 *data1){
    int num,res;
    float *T,max=1;
    bool p=true;
    GtkWidget *entry,**FT;
    GtkDrawingArea **DAT;
    char *text,message[45]="error n must be a number not a text in case ";


    T=(float *)malloc(sizeof(float )*(data1->n));
    gint response;
    GtkWidget *dialog, *label, *content_area;
    GtkDialogFlags flags;
      flags = GTK_DIALOG_DESTROY_WITH_PARENT;// the dialog box is here
      dialog = gtk_dialog_new_with_buttons ("Message", GTK_WINDOW (data1->window), flags, "_OK", GTK_RESPONSE_NONE, NULL);
      content_area = gtk_dialog_get_content_area (GTK_DIALOG (dialog));
      g_signal_connect_swapped (dialog, "response", G_CALLBACK (gtk_window_destroy), dialog);
    label=gtk_label_new("");
    gtk_grid_attach (GTK_GRID (data1->grid), label, 1, 4, 1, 1);
    for(int i=1;i<=data1->n;i++){// working on 2nd row also the loop is not going to stop if the user got a text !!!! recheck
        entry = gtk_grid_get_child_at (GTK_GRID (data1->grid), i, 2);
        text = gtk_editable_get_text (GTK_EDITABLE (entry));
      res = sscanf (text, "%d", &num);
      if (res == 1){
        if(num>max){
            max=num;
        }
        T[i-1]=num;
        printf("%f ",T[i-1]);

      }else{
        char ch[2];
        printf ("The input is a text: %s\n", text);
        sprintf(ch, "%d", i);
        strcat(message, ch);
        label = gtk_label_new (NULL);
        gtk_label_set_text(GTK_LABEL(label), message);
        message[44]='\0';
        gtk_box_append (GTK_BOX (content_area), label);
        gtk_widget_show (dialog);
        p=false;
      }
    }

    if(p==true){
        if(max>10){
            max=10/max;
            printf("max = %f ",max);
        }else max=1;
        DAT=(GtkDrawingArea **)malloc(sizeof(GtkWidget *)*(data1->n));
        GtkDrawingArea *base;
        FT=( GtkWidget **)malloc(sizeof( GtkWidget *)*(data1->n));
        for(int i=1;i<=data1->n;i++){

            DAT[i-1]=gtk_drawing_area_new ();
            FT[i-1]= gtk_fixed_new ();
            gtk_grid_attach (GTK_GRID (data1->grid), FT[i-1], i, 5, 1, 1);
            gtk_drawing_area_set_draw_func (GTK_DRAWING_AREA (DAT[i-1]), draw_rectangle, NULL, NULL);
            gtk_widget_set_size_request (DAT[i-1], 20, (T[i-1]*10*max));//x is fixed y (the second one must change)
            gtk_fixed_put (GTK_FIXED (FT[i-1]), DAT[i-1], 10, 150-(T[i-1]*10*max));
            base=gtk_drawing_area_new ();
            gtk_drawing_area_set_draw_func (GTK_DRAWING_AREA (base), draw_rectangleB, NULL, NULL);
             gtk_widget_set_size_request (base, 30, 2);
             gtk_fixed_put (GTK_FIXED (FT[i-1]), base, 5, 149);
        }
        data_2 *data2;
        data2=(data_2 *)malloc(sizeof(data_2));
        data2->DAT=DAT;
        data2->FT=FT;
        data2->T=T;
        data2->n=data1->n;
        data2->max=max;
        data2->grid=data1->grid;
        gtk_widget_set_sensitive (button, FALSE);
        g_signal_connect (data1->button, "clicked", G_CALLBACK (action4),data2);
        gtk_widget_set_sensitive (data1->button, true);
    }
}
void   action2(int N , gpointer *window,gpointer *grid ){
  GtkWidget *entry,*label,*box;
  GtkWidget *button,*buttonTR;

  int i;
  data_1 *data1;
  data1=(data_1 *)malloc(sizeof(data_1));
  char ch[2];
    label = gtk_label_new ("");
    gtk_grid_attach (GTK_GRID (grid), label, 0, 1, 1, 1);
    label = gtk_label_new ("please complete the table with numbers :");
    gtk_grid_attach (GTK_GRID (grid), label, 0, 2, 1, 1);
  for (i = 1; i <=N; i++) {
    entry = gtk_entry_new ();
    gtk_grid_attach (GTK_GRID (grid), entry, i, 2, 1, 1);
    sprintf(ch, "%d", i);
    label = gtk_label_new (NULL);
    gtk_label_set_text(GTK_LABEL(label), ch);
    gtk_grid_attach (GTK_GRID (grid), label, i, 3, 1, 1);
  }

   box = gtk_box_new (GTK_ORIENTATION_HORIZONTAL, 10);
   label = gtk_label_new ("                              ");
   gtk_box_append (GTK_BOX (box), label);

    buttonTR = gtk_button_new_with_label ("trie");
   gtk_widget_set_sensitive (buttonTR, FALSE);
   //the button trie action here
   gtk_box_append (GTK_BOX (box), buttonTR);
    button = gtk_button_new_with_label ("Ready");
    data1->grid=grid;
    data1->n=N;
    data1->window=window;
    data1->button=buttonTR;
  g_signal_connect (button, "clicked", G_CALLBACK (action3),data1);
  gtk_box_append (GTK_BOX (box), button);

  gtk_grid_attach (GTK_GRID (grid), box, 0, 3, 1, 1);




  gtk_window_present (GTK_WINDOW (window));
}
void action1 (GtkButton *button, data *data1)
{
        gint response;
        GtkWidget *dialog, *label, *content_area;
        GtkDialogFlags flags;
      flags = GTK_DIALOG_DESTROY_WITH_PARENT;// the dialog box is here
      dialog = gtk_dialog_new_with_buttons ("Message", GTK_WINDOW (data1->window), flags, "_OK", GTK_RESPONSE_NONE, NULL);
      content_area = gtk_dialog_get_content_area (GTK_DIALOG (dialog));
      g_signal_connect_swapped (dialog, "response", G_CALLBACK (gtk_window_destroy), dialog);


      char *text = gtk_editable_get_text (GTK_EDITABLE (data1->entry));// scan the number from the entry
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
      gtk_window_set_default_size (GTK_WINDOW (window), 800, 600);
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
