#include <gtk/gtk.h>
#include <glib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>

static GtkWidget *clear, *enter;
static int K = 0;
typedef struct my_data
{
    GtkWidget *window, *entry, *grid;
} data;
typedef struct my_data_1
{
    GtkWidget *grid, *window, *button;
    int n;
} data_1;
typedef struct my_data_2
{
    GtkWidget **FT, **DAT, *grid;
    int n, i, j;
    float *T, max;

} data_2;

static void draw_rectangleRED(GtkDrawingArea *area, cairo_t *cr, int width, int height)
{
    cairo_set_source_rgb(cr, 1, 0, 0);
    cairo_rectangle(cr, 0, 0, width, height);
    cairo_fill(cr);
}

static void draw_rectangle(GtkDrawingArea *area, cairo_t *cr, int width, int height)
{
    cairo_set_source_rgb(cr, 0, 0, 1);
    cairo_rectangle(cr, 0, 0, width, height);
    cairo_fill(cr);
}

static void draw_rectangleB(GtkDrawingArea *area, cairo_t *cr, int width, int height)
{
    cairo_set_source_rgb(cr, 0, 0, 0);
    cairo_rectangle(cr, 0, 0, width, height);
    cairo_fill(cr);
}

static void draw_rectangleGR(GtkDrawingArea *area, cairo_t *cr, int width, int height)
{
    cairo_set_source_rgb(cr, 0, 1, 0);
    cairo_rectangle(cr, 0, 0, width, height);
    cairo_fill(cr);
}

void action5(GtkButton *clear, GtkWidget *grid) // CLEAR
{
    int i;
    for (K; K < 2 || i < 6; K++)
    {
        i = 1;
        for (i; i < 6; i++)
        {
            gtk_grid_remove_row(grid, i);
        }

        gtk_widget_set_sensitive(enter, true);
    }
}

static gboolean on_timeout(data_2 *data2) // timer fonction for button TRI+ the main operation
{
    char *text;
    GtkWidget *entry;
    gtk_drawing_area_set_draw_func(GTK_DRAWING_AREA((data2->DAT)[data2->j]), draw_rectangleRED, NULL, NULL);

    if (data2->i < 0)
    {
        gtk_drawing_area_set_draw_func(GTK_DRAWING_AREA((data2->DAT)[0]), draw_rectangleGR, NULL, NULL);
        gtk_widget_set_sensitive(clear, true);
        return FALSE;
    }
    if (data2->j > data2->i)
    {
        gtk_drawing_area_set_draw_func(GTK_DRAWING_AREA((data2->DAT)[data2->j]), draw_rectangleGR, NULL, NULL);
        data2->i--;
        data2->j = 0;
        if (data2->i < 0)
        {
            gtk_drawing_area_set_draw_func(GTK_DRAWING_AREA((data2->DAT)[0]), draw_rectangleGR, NULL, NULL);
            gtk_widget_set_sensitive(clear, true);
            return FALSE;
        }
    }
    if ((data2->T)[data2->j] > (data2->T)[data2->j + 1])
    {
        int tt = (data2->T)[data2->j];
        (data2->T)[data2->j] = (data2->T)[data2->j + 1];
        (data2->T)[data2->j + 1] = tt;
        // update the entries
        entry = gtk_grid_get_child_at(GTK_GRID(data2->grid), data2->j + 1, 2);
        sprintf(text, "%d", (int)(data2->T)[data2->j]);
        gtk_editable_set_text(GTK_ENTRY(entry), text);
        entry = gtk_grid_get_child_at(GTK_GRID(data2->grid), data2->j + 2, 2);
        sprintf(text, "%d", (int)(data2->T)[data2->j + 1]);
        gtk_editable_set_text(GTK_ENTRY(entry), text);
        // change places
        if ((data2->T)[data2->j] < 0)
        {
            gtk_widget_set_size_request((data2->DAT)[data2->j], 20, abs((data2->T)[data2->j] * 15 * (data2->max)));
            gtk_fixed_move(GTK_FIXED((data2->FT)[data2->j]), (data2->DAT)[data2->j], 10, 225);
        }
        else
        {
            gtk_widget_set_size_request((data2->DAT)[data2->j], 20, abs((data2->T)[data2->j] * 15 * (data2->max)));
            gtk_fixed_move(GTK_FIXED((data2->FT)[data2->j]), (data2->DAT)[data2->j], 10, 225 - (data2->T)[data2->j] * 15 * (data2->max));
        }
        if ((data2->T)[data2->j + 1] < 0)
        {
            gtk_widget_set_size_request((data2->DAT)[data2->j + 1], 20, abs((data2->T)[data2->j + 1] * 15 * (data2->max)));
            gtk_fixed_move(GTK_FIXED((data2->FT)[data2->j + 1]), (data2->DAT)[data2->j + 1], 10, 225);
        }
        else
        {
            gtk_widget_set_size_request((data2->DAT)[data2->j + 1], 20, abs((data2->T)[data2->j + 1] * 15 * (data2->max)));
            gtk_fixed_move(GTK_FIXED((data2->FT)[data2->j + 1]), (data2->DAT)[data2->j + 1], 10, 225 - (data2->T)[data2->j + 1] * 15 * (data2->max));
        }
    }
    gtk_drawing_area_set_draw_func(GTK_DRAWING_AREA((data2->DAT)[data2->j]), draw_rectangle, NULL, NULL);
    // increment the inner loop index
    data2->j++;
    gtk_drawing_area_set_draw_func(GTK_DRAWING_AREA((data2->DAT)[data2->j]), draw_rectangleRED, NULL, NULL);
    return TRUE; // continue the loop
}

void action4(GtkButton *button, data_2 *data2)
{
    GtkWidget *entry;
    for (int q = 1; q <= data2->n; q++)
    {
        entry = gtk_grid_get_child_at(GTK_GRID(data2->grid), q, 2);
        gtk_widget_set_sensitive(entry, false);
    }
    gtk_widget_set_sensitive(button, false);
    gtk_widget_set_sensitive(clear, false);
    data2->i = (data2->n) - 2;
    data2->j = 0;
    g_timeout_add(400, on_timeout, data2);
}

void action3(GtkButton *button, data_1 *data1) // for button ready
{
    int num, res;
    float *T, max = 1;
    bool p = true;
    GtkWidget *entry, **FT, *label;
    GtkDrawingArea **DAT;
    gint response;
    GtkWidget *dialog, *content_area;
    GtkDialogFlags flags;
    char *text, scale[7] = "15px->", message[41] = "   Error there must be a number in case ", ch[5]; // text needed
    T = (float *)malloc(sizeof(float) * (data1->n));
    flags = GTK_DIALOG_DESTROY_WITH_PARENT; // the dialog box is here
    dialog = gtk_dialog_new_with_buttons("Alert", GTK_WINDOW(data1->window), flags, "_OK", GTK_RESPONSE_NONE, NULL);
    content_area = gtk_dialog_get_content_area(GTK_DIALOG(dialog));
    g_signal_connect_swapped(dialog, "response", G_CALLBACK(gtk_window_destroy), dialog);
    label = gtk_label_new("");
    gtk_grid_attach(GTK_GRID(data1->grid), label, 1, 4, 1, 1);
    for (int i = 1; i <= data1->n; i++)
    { // working on 2nd row also the loop is not going to stop if the user got a text (update it works now !!!)
        entry = gtk_grid_get_child_at(GTK_GRID(data1->grid), i, 2);
        text = gtk_editable_get_text(GTK_EDITABLE(entry));
        res = sscanf(text, "%d", &num);
        if (res == 1) // if its not a text
        {
            if (abs(num) > max)
            {
                max = abs(num);
            }
            T[i - 1] = num;
        }
        else
        {
            // send an error message of a missing value or its a text
            sprintf(ch, "%d", i);
            strcat(message, ch);
            label = gtk_label_new(NULL);
            gtk_label_set_text(GTK_LABEL(label), message);
            message[40] = '\0';
            gtk_box_append(GTK_BOX(content_area), label);
            gtk_widget_show(dialog);
            p = false;
        }
    }

    if (p == true)
    {

        max = 15 / max;                                                    // scaling
        DAT = (GtkDrawingArea **)malloc(sizeof(GtkWidget *) * (data1->n)); // table of drawing areas
        GtkDrawingArea *base;                                              // drawing base (black)
        FT = (GtkWidget **)malloc(sizeof(GtkWidget *) * (data1->n));       // table of fixed areas
        for (int i = 1; i <= data1->n; i++)                                // draw the ready state
        {
            float p;
            if (T[i - 1] < 0)
            {
                p = 0;
            }
            else
                p = (T[i - 1] * 15 * max);
            DAT[i - 1] = gtk_drawing_area_new();
            FT[i - 1] = gtk_fixed_new();

            gtk_grid_attach(GTK_GRID(data1->grid), FT[i - 1], i, 5, 1, 1);
            gtk_drawing_area_set_draw_func(GTK_DRAWING_AREA(DAT[i - 1]), draw_rectangle, NULL, NULL);
            gtk_widget_set_size_request(DAT[i - 1], 20, abs(T[i - 1] * 15 * max)); // x is fixed y aka(the second one must change)
            gtk_fixed_put(GTK_FIXED(FT[i - 1]), DAT[i - 1], 10, 225 - p);
            base = gtk_drawing_area_new();
            gtk_drawing_area_set_draw_func(GTK_DRAWING_AREA(base), draw_rectangleB, NULL, NULL);
            gtk_widget_set_size_request(base, 30, 2);
            gtk_fixed_put(GTK_FIXED(FT[i - 1]), base, 5, 224);
        }
        gtk_drawing_area_set_draw_func(GTK_DRAWING_AREA(DAT[0]), draw_rectangleRED, NULL, NULL);
        data_2 *data2; // prepare the data for the tri button
        data2 = (data_2 *)malloc(sizeof(data_2));
        data2->DAT = DAT;
        data2->FT = FT;
        data2->T = T;
        data2->n = data1->n;
        data2->max = max;
        data2->grid = data1->grid;
        gtk_widget_set_sensitive(button, FALSE);
        content_area = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
        gtk_box_set_homogeneous(GTK_BOX(content_area), TRUE);
        // fill the empty area kinda
        label = gtk_label_new("");
        gtk_label_set_markup(GTK_LABEL(label), "<span font='Arial' foreground='black' weight='bold' size='12000'>Now the app is going to do the work for you !</span>");

        gtk_box_append(GTK_BOX(content_area), label);

        label = gtk_label_new("");
        gtk_label_set_markup(GTK_LABEL(label), "<span font='Arial' foreground='black' weight='bold' size='12000'>The Scaling of the bars is on :</span>");

        gtk_box_append(GTK_BOX(content_area), label);

        sprintf(text, "%f", 1 / max);
        strcat(scale, text);
        label = gtk_label_new(scale);

        GtkCssProvider *provider = gtk_css_provider_new();
        gtk_css_provider_load_from_data(provider, "label { font-size: 20pt;font-family: 'Arial'; font-weight: bold; }", -1);
        GtkStyleContext *context = gtk_widget_get_style_context(label);
        gtk_style_context_add_provider(context, GTK_STYLE_PROVIDER(provider), GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);

        gtk_box_append(GTK_BOX(content_area), label);

        gtk_grid_attach(GTK_GRID(data1->grid), content_area, 0, 5, 1, 1); // put the box in the main grid
        // now for the tri button action
        g_signal_connect(data1->button, "clicked", G_CALLBACK(action4), data2);
        gtk_widget_set_sensitive(data1->button, true);
    }
}
void action2(int N, gpointer *window, gpointer *grid) // create the empty table + the buttons
{
    GtkWidget *entry, *label, *box;
    GtkWidget *button, *buttonTR;

    int i;
    data_1 *data1;
    data1 = (data_1 *)malloc(sizeof(data_1));
    char ch[2];
    label = gtk_label_new("");
    gtk_grid_attach(GTK_GRID(grid), label, 0, 1, 1, 1);
    label = gtk_label_new("");
    gtk_label_set_markup(GTK_LABEL(label), "<span font='Arial' foreground='black' weight='bold'>Please complete the table with numbers :</span>");
    gtk_grid_attach(GTK_GRID(grid), label, 0, 2, 1, 1);
    for (i = 1; i <= N; i++)
    {
        entry = gtk_entry_new();
        gtk_grid_attach(GTK_GRID(grid), entry, i, 2, 1, 1);
        sprintf(ch, "%d", i);
        label = gtk_label_new(NULL);
        gtk_label_set_text(GTK_LABEL(label), ch);
        gtk_grid_attach(GTK_GRID(grid), label, i, 3, 1, 1);
    }

    box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 10);
    label = gtk_label_new("                              ");
    gtk_box_append(GTK_BOX(box), label);

    buttonTR = gtk_button_new_with_label("Tri");
    gtk_widget_set_sensitive(buttonTR, FALSE);
    gtk_box_append(GTK_BOX(box), buttonTR);
    button = gtk_button_new_with_label("Ready");
    data1->grid = grid;
    data1->n = N;
    data1->window = window;
    data1->button = buttonTR;
    g_signal_connect(button, "clicked", G_CALLBACK(action3), data1);
    gtk_box_append(GTK_BOX(box), button);

    gtk_grid_attach(GTK_GRID(grid), box, 0, 3, 1, 1);

    gtk_window_present(GTK_WINDOW(window));
}
void action1(GtkButton *button, data *data1) // enter button action
{
    gint response;
    GtkWidget *dialog, *label, *content_area;
    GtkDialogFlags flags;

    flags = GTK_DIALOG_DESTROY_WITH_PARENT; // the dialog box is here
    dialog = gtk_dialog_new_with_buttons("Alert", GTK_WINDOW(data1->window), flags, "_OK", GTK_RESPONSE_NONE, NULL);
    content_area = gtk_dialog_get_content_area(GTK_DIALOG(dialog));
    g_signal_connect_swapped(dialog, "response", G_CALLBACK(gtk_window_destroy), dialog);

    char *text = gtk_editable_get_text(GTK_EDITABLE(data1->entry)); // scan the number from the entry
    int num;
    int result = sscanf(text, "%d", &num);
    if (result == 1)
    {
        if (num < 2 || num > 28)
        {
            label = gtk_label_new("    please enter a valid number from 2 to 28    ");
            gtk_box_append(GTK_BOX(content_area), label);
            gtk_widget_show(dialog);
        }
        else
        {
            gtk_widget_set_sensitive(enter, false);
            gtk_widget_set_sensitive(clear, true);
            action2(num, data1->window, data1->grid); // start the second state (creating the table and the new buttons)
            g_signal_connect(clear, "clicked", G_CALLBACK(action5), data1->grid);
        }
    }
    else
    {
        label = gtk_label_new("error n must be a positive number");
        gtk_box_append(GTK_BOX(content_area), label);
        gtk_widget_show(dialog);
    }
}
static void activate(GtkApplication *app, gpointer user_data)
{
    GtkWidget *window;
    GtkWidget *grid, *grid1;
    GtkWidget *label;
    GtkWidget *entry;
    data *data1;
    data1 = (data *)malloc(sizeof(data));
    window = gtk_application_window_new(app);
    gtk_window_set_title(GTK_WINDOW(window), "Tri a bulles"); // the window is here
    gtk_window_set_default_size(GTK_WINDOW(window), 1280, 720);
    grid = gtk_grid_new();
    gtk_window_set_child(GTK_WINDOW(window), grid);
    grid1 = gtk_grid_new();
    label = gtk_label_new("");
    gtk_label_set_markup(GTK_LABEL(label), "<span font='Arial' foreground='black' weight='bold'>Please enter a valid Number between 2 to 28  :</span>");
    entry = gtk_entry_new();
    gtk_entry_set_max_length(GTK_ENTRY(entry), 5);
    enter = gtk_button_new_with_label("Enter");
    clear = gtk_button_new_with_label("Clear");
    gtk_widget_set_sensitive(clear, false);
    gtk_grid_attach(GTK_GRID(grid1), label, 0, 0, 1, 1);
    gtk_grid_attach(GTK_GRID(grid1), entry, 1, 0, 1, 1);
    gtk_grid_attach(GTK_GRID(grid1), enter, 2, 0, 1, 1);
    gtk_grid_attach(GTK_GRID(grid1), clear, 3, 0, 1, 1);

    gtk_grid_attach(GTK_GRID(grid), grid1, 0, 0, 1, 1);

    data1->window = window;
    data1->entry = entry;
    data1->grid = grid;

    g_signal_connect(enter, "clicked", G_CALLBACK(action1), data1);

    gtk_window_present(GTK_WINDOW(window));
}

int main(int argc, char **argv)
{
    GtkApplication *app;
    int status;

    app = gtk_application_new("org.gtk.Mainapp", G_APPLICATION_DEFAULT_FLAGS);
    g_signal_connect(app, "activate", G_CALLBACK(activate), NULL);
    status = g_application_run(G_APPLICATION(app), argc, argv);
    g_object_unref(app);

    return status;
}
