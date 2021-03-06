namespace phoenix {

static gboolean Viewport_drag_drop(GtkWidget* widget, GdkDragContext* context, gint x, gint y, guint timestamp, pViewport* self) {
  return true;
}

static void Viewport_drag_leave(GtkWidget* widget, GdkDragContext* context, guint timestamp, pViewport* self) {
}

static gboolean Viewport_drag_motion(GtkWidget* widget, GdkDragContext* context, gint x, gint y,
GtkSelectionData* data, guint type, guint timestamp, pViewport* self) {
  return true;
}

static void Viewport_drag_received(GtkWidget* widget, GdkDragContext* context, gint x, gint y,
GtkSelectionData* data, guint type, guint timestamp, pViewport* self) {
  if(self->viewport.state.droppable == false) return;

  gchar** uris = gtk_selection_data_get_uris(data);
  if(uris == nullptr) return;

  lstring result;
  for(unsigned n = 0; uris[n] != nullptr; n++) {
    gchar* pathname = g_filename_from_uri(uris[n], nullptr, nullptr);
    if(pathname) {
      string path = pathname;
      g_free(pathname);
      if(directory::exists(path) && !path.endswith("/")) path.append("/");
      result.append(path);
    }
  }

  g_strfreev(uris);
  if(result.empty()) return;

  if(self->viewport.onDrop) self->viewport.onDrop(result);
}

static gboolean Viewport_mouseLeave(GtkWidget* widget, GdkEventButton* event, pViewport* self) {
  if(self->viewport.onMouseLeave) self->viewport.onMouseLeave();
  return true;
}

static gboolean Viewport_mouseMove(GtkWidget* widget, GdkEventButton* event, pViewport* self) {
  if(self->viewport.onMouseMove) self->viewport.onMouseMove({(signed)event->x, (signed)event->y});
  return true;
}

static gboolean Viewport_mousePress(GtkWidget* widget, GdkEventButton* event, pViewport* self) {
  if(self->viewport.onMousePress) switch(event->button) {
  case 1: self->viewport.onMousePress(Mouse::Button::Left); break;
  case 2: self->viewport.onMousePress(Mouse::Button::Middle); break;
  case 3: self->viewport.onMousePress(Mouse::Button::Right); break;
  }
  return true;
}

static gboolean Viewport_mouseRelease(GtkWidget* widget, GdkEventButton* event, pViewport* self) {
  if(self->viewport.onMouseRelease) switch(event->button) {
  case 1: self->viewport.onMouseRelease(Mouse::Button::Left); break;
  case 2: self->viewport.onMouseRelease(Mouse::Button::Middle); break;
  case 3: self->viewport.onMouseRelease(Mouse::Button::Right); break;
  }
  return true;
}

uintptr_t pViewport::handle() {
  return GDK_WINDOW_XID(gtk_widget_get_window(gtkWidget));
}

void pViewport::setDroppable(bool droppable) {
  gtk_drag_dest_set(gtkWidget, GTK_DEST_DEFAULT_ALL, nullptr, 2, GDK_ACTION_COPY);
  if(droppable) gtk_drag_dest_add_uri_targets(gtkWidget);
}

void pViewport::constructor() {
  gtkWidget = gtk_drawing_area_new();
//gtk_widget_set_double_buffered(gtkWidget, false);
  gtk_widget_add_events(gtkWidget,
    GDK_BUTTON_PRESS_MASK | GDK_BUTTON_RELEASE_MASK | GDK_LEAVE_NOTIFY_MASK | GDK_POINTER_MOTION_MASK);
  g_signal_connect(G_OBJECT(gtkWidget), "drag-drop", G_CALLBACK(Viewport_drag_drop), (gpointer)this);
  g_signal_connect(G_OBJECT(gtkWidget), "drag-leave", G_CALLBACK(Viewport_drag_leave), (gpointer)this);
  g_signal_connect(G_OBJECT(gtkWidget), "drag-motion", G_CALLBACK(Viewport_drag_motion), (gpointer)this);
  g_signal_connect(G_OBJECT(gtkWidget), "drag-data-received", G_CALLBACK(Viewport_drag_received), (gpointer)this);
  g_signal_connect(G_OBJECT(gtkWidget), "button_press_event", G_CALLBACK(Viewport_mousePress), (gpointer)this);
  g_signal_connect(G_OBJECT(gtkWidget), "button_release_event", G_CALLBACK(Viewport_mouseRelease), (gpointer)this);
  g_signal_connect(G_OBJECT(gtkWidget), "leave_notify_event", G_CALLBACK(Viewport_mouseLeave), (gpointer)this);
  g_signal_connect(G_OBJECT(gtkWidget), "motion_notify_event", G_CALLBACK(Viewport_mouseMove), (gpointer)this);

  GdkColor color;
  color.pixel = 0;
  color.red = 0;
  color.green = 0;
  color.blue = 0;
  gtk_widget_modify_bg(gtkWidget, GTK_STATE_NORMAL, &color);
}

void pViewport::destructor() {
  gtk_widget_destroy(gtkWidget);
}

void pViewport::orphan() {
  destructor();
  constructor();
}

}
