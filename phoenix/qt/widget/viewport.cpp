namespace phoenix {

uintptr_t pViewport::handle() {
  return (uintptr_t)qtViewport->winId();
}

void pViewport::setDroppable(bool droppable) {
  qtViewport->setAcceptDrops(droppable);
}

void pViewport::constructor() {
  qtWidget = qtViewport = new QtViewport(*this);
  qtViewport->setMouseTracking(true);
  qtViewport->setAttribute(Qt::WA_PaintOnScreen, true);
  qtViewport->setStyleSheet("background: #000000");

  pWidget::synchronizeState();
}

void pViewport::destructor() {
  delete qtViewport;
  qtWidget = qtViewport = nullptr;
}

void pViewport::orphan() {
  destructor();
  constructor();
}

void pViewport::QtViewport::dragEnterEvent(QDragEnterEvent* event) {
  if(event->mimeData()->hasUrls()) {
    event->acceptProposedAction();
  }
}

void pViewport::QtViewport::dropEvent(QDropEvent* event) {
  QList<QUrl> urls = event->mimeData()->urls();
  if(urls.size() == 0) return;

  lstring result;
  for(unsigned n = 0; n < urls.size(); n++) {
    string path = urls[n].path().toUtf8().constData();
    if(path.empty()) continue;
    if(directory::exists(path) && !path.endswith("/")) path.append("/");
    result.append(path);
  }

  if(result.size() == 0) return;

  if(self.viewport.onDrop) self.viewport.onDrop(result);
}

void pViewport::QtViewport::leaveEvent(QEvent* event) {
  if(self.viewport.onMouseLeave) self.viewport.onMouseLeave();
}

void pViewport::QtViewport::mouseMoveEvent(QMouseEvent* event) {
  if(self.viewport.onMouseMove) self.viewport.onMouseMove({event->pos().x(), event->pos().y()});
}

void pViewport::QtViewport::mousePressEvent(QMouseEvent* event) {
  if(!self.viewport.onMousePress) return;
  switch(event->button()) {
  case Qt::LeftButton: self.viewport.onMousePress(Mouse::Button::Left); break;
  case Qt::MidButton: self.viewport.onMousePress(Mouse::Button::Middle); break;
  case Qt::RightButton: self.viewport.onMousePress(Mouse::Button::Right); break;
  }
}

void pViewport::QtViewport::mouseReleaseEvent(QMouseEvent* event) {
  if(!self.viewport.onMouseRelease) return;
  switch(event->button()) {
  case Qt::LeftButton: self.viewport.onMouseRelease(Mouse::Button::Left); break;
  case Qt::MidButton: self.viewport.onMouseRelease(Mouse::Button::Middle); break;
  case Qt::RightButton: self.viewport.onMouseRelease(Mouse::Button::Right); break;
  }
}

pViewport::QtViewport::QtViewport(pViewport& self) : self(self) {
}

}
