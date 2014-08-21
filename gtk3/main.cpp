#include <gtk/gtk.h>
#include "../core/Frontend.hpp"

#include <string>

#define GAMEBOY_WIDTH  160
#define GAMEBOY_HEIGHT 144

#define JAMES_STOPPED 0
#define JAMES_PAUSED  1
#define JAMES_RUNNING 2

guchar image[GAMEBOY_WIDTH * GAMEBOY_HEIGHT * 3] = { 0, };

class GtkJames : public james::core::Frontend
{
	public:
	int rightPressed;
	int leftPressed;
	int upPressed;
	int downPressed;
	int buttonAPressed;
	int buttonBPressed;
	int selectPressed;
	int startPressed;

	void DrawFrame (Frame& frame)
	{
		for (int y = 0; y < Frame::HEIGHT; y++)
		{
			for (int x = 0; x < Frame::WIDTH; x++)
			{
				image[(y * GAMEBOY_WIDTH + x) * 3] = static_cast<guchar>(frame.data[x][y].GetRed());
				image[(y * GAMEBOY_WIDTH + x) * 3 + 1] = static_cast<guchar>(frame.data[x][y].GetGreen());
				image[(y * GAMEBOY_WIDTH + x) * 3 + 2] = static_cast<guchar>(frame.data[x][y].GetBlue());
			}
		}
	}

	int GetRight()
	{
		return rightPressed;
	}

	int GetLeft()
	{
		return leftPressed;
	}

	int GetUp()
	{
		return upPressed;
	}

	int GetDown()
	{
		return downPressed;
	}

	int GetButtonA()
	{
		return buttonAPressed;
	}

	int GetButtonB()
	{
		return buttonBPressed;
	}

	int GetSelect()
	{
		return selectPressed;
	}

	int GetStart()
	{
		return startPressed;
	}
};

/* VARIABLES */
// james
GtkJames *gtk_james;
GThread *thread_james;

// gtk3
GtkApplication *app;

GMenu *menu;
GMenu *menu_io;
GMenu *menu_preferences;
GMenu *menu_quit;

GtkWidget *header_bar;
//GtkWidget *button_open;
//GtkWidget *button_save_state;
GtkWidget *button_run;

GtkWidget *window;
GtkWidget *box;

GtkWidget *screen;
int screen_width = GAMEBOY_WIDTH;
int screen_height = GAMEBOY_HEIGHT;

GtkWidget *info_bar;
GtkWidget *info_label;

/* JAMES BINDINGS */
	static void *
james_loop(gpointer data)
{
	while (!gtk_james->IsPaused())
	{
		gtk_james->RenderFrame();
		gtk_widget_queue_draw_area(screen, 0, 0, screen_width, screen_height);

		g_thread_yield();
	}
}

void
james_open(const char *file_path)
{
	using namespace james::cartridges;

	gtk_james->LoadRom(std::string(file_path));
	gtk_james->Reset();

	Array<int, Header::NEW_TITLE_LENGTH> newTitle;
	newTitle = gtk_james->GetDevice().GetCartridge().GetHeader().newTitle;

	char newTitleString[Header::NEW_TITLE_LENGTH];

	for (int i = 0; i < Header::NEW_TITLE_LENGTH; i++)
	{
		newTitleString[i] = (char) newTitle[i];
	}

	gtk_header_bar_set_subtitle(GTK_HEADER_BAR(header_bar), newTitleString);
}

void
james_save_state(const char *file_path)
{
}

void
james_run()
{
	GtkWidget *pause_icon = gtk_image_new_from_icon_name("media-playback-pause",
	                                                     GTK_ICON_SIZE_BUTTON);
	gtk_button_set_image(GTK_BUTTON(button_run), pause_icon);

	gtk_james->Start();

	if (thread_james == NULL)
	{
		thread_james = g_thread_new("james", &james_loop, NULL);
	}
}

void
james_pause()
{
	GtkWidget *play_icon = gtk_image_new_from_icon_name("media-playback-start",
	                                                    GTK_ICON_SIZE_BUTTON);
	gtk_button_set_image(GTK_BUTTON(button_run), play_icon);

	gtk_james->Pause();

	if (thread_james != NULL)
	{
		g_thread_join(thread_james);
		g_thread_unref(thread_james);

		thread_james = NULL;
	}
}

/* UI */
static gboolean
on_key_press(GtkWidget *widget,
             GdkEventKey *event)
{
		char *key_name = gdk_keyval_name(event->keyval);

		if (strcmp(key_name, "Right") == 0)
		{
			gtk_james->rightPressed = TRUE;
			gtk_james->GetDevice().GetInterruptHandler().SignalJoypadInterrupt();
		}
		
		if (strcmp(key_name, "Left") == 0)
		{
			gtk_james->leftPressed = TRUE;
			gtk_james->GetDevice().GetInterruptHandler().SignalJoypadInterrupt();
		}
		
		if (strcmp(key_name, "Up") == 0)
		{
			gtk_james->upPressed = TRUE;
			gtk_james->GetDevice().GetInterruptHandler().SignalJoypadInterrupt();
		}

		if (strcmp(key_name, "Down") == 0)
		{
			gtk_james->downPressed = TRUE;
			gtk_james->GetDevice().GetInterruptHandler().SignalJoypadInterrupt();
		}

		if (strcmp(key_name, "period") == 0)
		{
			gtk_james->buttonAPressed = TRUE;
			gtk_james->GetDevice().GetInterruptHandler().SignalJoypadInterrupt();
		}

		if (strcmp(key_name, "comma") == 0)
		{
			gtk_james->buttonBPressed = TRUE;
			gtk_james->GetDevice().GetInterruptHandler().SignalJoypadInterrupt();
		}

		if (strcmp(key_name, "minus") == 0)
		{
			gtk_james->selectPressed = TRUE;
			gtk_james->GetDevice().GetInterruptHandler().SignalJoypadInterrupt();
		}

		if (strcmp(key_name, "a") == 0)
		{
			gtk_james->startPressed = TRUE;
			gtk_james->GetDevice().GetInterruptHandler().SignalJoypadInterrupt();
		}

    return FALSE;
}

static gboolean
on_key_release(GtkWidget *widget,
               GdkEventKey *event)
{
		char *key_name = gdk_keyval_name(event->keyval);

		if (strcmp(key_name, "Right") == 0)
		{
			gtk_james->rightPressed = FALSE;
		}
		
		if (strcmp(key_name, "Left") == 0)
		{
			gtk_james->leftPressed = FALSE;
		}
		
		if (strcmp(key_name, "Up") == 0)
		{
			gtk_james->upPressed = FALSE;
		}

		if (strcmp(key_name, "Down") == 0)
		{
			gtk_james->downPressed = FALSE;
		}

		if (strcmp(key_name, "period") == 0)
		{
			gtk_james->buttonAPressed = FALSE;
		}

		if (strcmp(key_name, "comma") == 0)
		{
			gtk_james->buttonBPressed = FALSE;
		}

		if (strcmp(key_name, "minus") == 0)
		{
			gtk_james->selectPressed = FALSE;
		}

		if (strcmp(key_name, "a") == 0)
		{
			gtk_james->startPressed = FALSE;
		}

    return FALSE;
}

char *
show_open_dialog(gchar *title)
{
	GtkWidget *dialog = gtk_file_chooser_dialog_new (title,
	                                                 GTK_WINDOW(window),
	                                                 GTK_FILE_CHOOSER_ACTION_OPEN,
	                                                 "_Cancel",
	                                                 GTK_RESPONSE_CANCEL,
	                                                 "_Open",
	                                                 GTK_RESPONSE_ACCEPT,
	                                                 NULL);
	
	gint result = gtk_dialog_run(GTK_DIALOG(dialog));

	if (result == GTK_RESPONSE_ACCEPT)
	{
		char *file_path;

		GtkFileChooser *chooser = GTK_FILE_CHOOSER(dialog);
		file_path = gtk_file_chooser_get_filename(chooser);

		gtk_widget_destroy(dialog);

		return file_path;
	}

	gtk_widget_destroy(dialog);

	return NULL;
}

char *
show_save_dialog(gchar *title)
{
	GtkWidget *dialog = gtk_file_chooser_dialog_new (title,
	                                                 GTK_WINDOW(window),
	                                                 GTK_FILE_CHOOSER_ACTION_SAVE,
	                                                 "_Cancel",
	                                                 GTK_RESPONSE_CANCEL,
	                                                 "_Save",
	                                                 GTK_RESPONSE_ACCEPT,
	                                                 NULL);
	
	gint result = gtk_dialog_run(GTK_DIALOG(dialog));

	if (result == GTK_RESPONSE_ACCEPT)
	{
		char *file_path;

		GtkFileChooser *chooser = GTK_FILE_CHOOSER(dialog);
		file_path = gtk_file_chooser_get_filename(chooser);

		gtk_widget_destroy(dialog);

		return file_path;
	}

	gtk_widget_destroy(dialog);

	return NULL;
}

void
on_button_run_clicked(GtkWidget *self,
                      gpointer   data)
{
	if (gtk_james->IsPaused())
	{
		james_run();
	}
	else
	{
		james_pause();
	}
}

void
on_button_clicked(GtkWidget *self,
                  gpointer   data)
{
	gtk_button_set_label(GTK_BUTTON(self), "Ha! You clicked me!");

	gtk_widget_show_all(info_bar);
}

static void
on_menu_open_clicked(GSimpleAction *action,
                     GVariant      *parameter,
                     gpointer       user_data)
{
	char *file_path = show_open_dialog("Open ROM");
	
	if (file_path != NULL)
	{
		james_open(file_path);

		james_pause();
	}
}

static void
on_menu_save_state_clicked(GSimpleAction *action,
                           GVariant      *parameter,
                           gpointer       user_data)
{
	char *file_path = show_save_dialog("Save state");

	if (file_path != NULL)
	{
		james_save_state(file_path);
	}
}

static void
on_menu_preferences_clicked(GSimpleAction *action,
                            GVariant      *parameter,
                            gpointer       user_data)
{
}

static void
on_menu_quit_clicked(GSimpleAction *action,
                     GVariant      *parameter,
                     gpointer       user_data)
{
  g_application_quit(G_APPLICATION(app));
}

/* CAIRO */
static gboolean
on_screen_configure(GtkWidget         *self,
	                  GdkEventConfigure *event,
	                  gpointer           data)
{
	return TRUE;
}

static gboolean
on_screen_draw(GtkWidget *self,
	             cairo_t   *cr,
	             gpointer   data)
{
	screen_width = gtk_widget_get_allocated_width(self);
	screen_height = gtk_widget_get_allocated_height(self);

	GdkPixbuf *pixbuf;

	pixbuf = gdk_pixbuf_new_from_data(image,
	                                  GDK_COLORSPACE_RGB,
	                                  FALSE,
	                                  8,
	                                  GAMEBOY_WIDTH,
	                                  GAMEBOY_HEIGHT,
	                                  GAMEBOY_WIDTH * 3,
	                                  NULL,
	                                  NULL);

	GdkPixbuf *pixbuf_scaled;
	
	pixbuf_scaled = gdk_pixbuf_scale_simple(pixbuf,
	                                        screen_width,
	                                        screen_height,
	                                        GDK_INTERP_NEAREST);

	gdk_cairo_set_source_pixbuf(cr, pixbuf_scaled, 0, 0);
	cairo_paint(cr);

	gdk_pixbuf_unref(pixbuf);
	gdk_pixbuf_unref(pixbuf_scaled);

	return FALSE;
}

static void
on_activate(GApplication *app,
            gpointer      data)
{
	// set up header bar
	// icons
	GtkWidget *icon_play = gtk_image_new_from_icon_name("media-playback-start",
	                                                    GTK_ICON_SIZE_BUTTON);

	// run button
	button_run = gtk_button_new();
	gtk_button_set_image(GTK_BUTTON(button_run), icon_play);
	gtk_widget_set_valign(button_run, GTK_ALIGN_CENTER);
	gtk_widget_set_size_request(button_run, 20, 20);
	g_signal_connect(button_run,
	                 "clicked",
	                 G_CALLBACK(on_button_run_clicked),
	                 NULL);

	// set up the actual header bar
	header_bar = gtk_header_bar_new();
	gtk_header_bar_set_show_close_button(GTK_HEADER_BAR(header_bar), TRUE);
	gtk_header_bar_set_title(GTK_HEADER_BAR(header_bar), "James");
	gtk_header_bar_set_has_subtitle(GTK_HEADER_BAR(header_bar), TRUE);
	gtk_header_bar_set_subtitle(GTK_HEADER_BAR(header_bar),
	                            "a decadent gameboy emulatior");

	// add things to the header bar
	gtk_header_bar_pack_start(GTK_HEADER_BAR(header_bar), button_run);

	// set up application menu
	menu_io = g_menu_new();
	g_menu_append(menu_io, "_Open", "app.open");
	g_menu_append(menu_io, "Save state", "app.save_state");

	menu_preferences = g_menu_new();
	g_menu_append(menu_preferences, "_Preferences", "app.preferences");;

	menu_quit = g_menu_new();
	g_menu_append(menu_quit, "_Quit", "app.quit");

	menu = g_menu_new();
	g_menu_append_section(menu, NULL, G_MENU_MODEL(menu_io));
	g_menu_append_section(menu, NULL, G_MENU_MODEL(menu_preferences));
	g_menu_append_section(menu, NULL, G_MENU_MODEL(menu_quit));

	gtk_application_set_app_menu(GTK_APPLICATION(app), G_MENU_MODEL(menu));

	g_object_unref(menu_io);
	g_object_unref(menu_quit);
	g_object_unref(menu);

	// set up window
  window = gtk_application_window_new(GTK_APPLICATION(app));
	gtk_application_window_set_show_menubar(GTK_APPLICATION_WINDOW(window), FALSE);
  gtk_window_set_titlebar(GTK_WINDOW(window), header_bar);
	gtk_window_set_wmclass(GTK_WINDOW(window), "James", "James");
	gtk_window_set_title(GTK_WINDOW(window), "James");
	g_signal_connect(window, "key-press-event", G_CALLBACK(on_key_press), NULL);
	g_signal_connect(window, "key-release-event", G_CALLBACK(on_key_release), NULL);

	// set up box
	box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);

	// set up the drawing area
	screen = gtk_drawing_area_new();
	g_signal_connect(screen,
	                 "configure-event",
	                 G_CALLBACK(on_screen_configure),
	                 NULL);
	g_signal_connect(screen,
	                 "draw",
	                 G_CALLBACK(on_screen_draw),
	                 NULL);
	gtk_widget_set_size_request(screen, GAMEBOY_WIDTH * 2, GAMEBOY_HEIGHT * 2);
	gtk_widget_set_can_focus(screen, TRUE);
	
	gtk_box_pack_start(GTK_BOX(box), screen, TRUE, TRUE, 0);

	gtk_container_add(GTK_CONTAINER(window), box);

	// show all widgets
	gtk_widget_show_all(header_bar);
	gtk_widget_show(window);

	gtk_widget_show(box);
	gtk_widget_show(screen);

	// initialize james
	gtk_james = new GtkJames();
	gtk_james->Initialize();

	thread_james = NULL;

	james_pause();
}

static void
on_shutdown(GApplication *app,
            gpointer      data)
{
	gtk_james->Finalize();

	delete gtk_james;
}

int
main(int   argc,
     char *argv[])
{
	int status;

	app = gtk_application_new("james.gtkapp", G_APPLICATION_FLAGS_NONE);
	g_signal_connect(app, "activate", G_CALLBACK(on_activate), NULL);
	g_signal_connect(app, "shutdown", G_CALLBACK(on_shutdown), NULL);
	//g_signal_connect(app, "quit", G_CALLBACK(on_quit), NULL);
	
	static GActionEntry app_entries[] =
	{
	  { "open", on_menu_open_clicked, NULL, NULL, NULL },
	  { "save_state", on_menu_save_state_clicked, NULL, NULL, NULL },
	  { "preferences", on_menu_preferences_clicked, NULL, NULL, NULL },
	  { "quit", on_menu_quit_clicked, NULL, NULL, NULL }
	};
	
	g_action_map_add_action_entries(G_ACTION_MAP(app),
	                                app_entries,
	                                G_N_ELEMENTS(app_entries),
	                                app);

	status = g_application_run(G_APPLICATION(app), argc, argv);

	g_object_unref(app);

	return 0;
}
