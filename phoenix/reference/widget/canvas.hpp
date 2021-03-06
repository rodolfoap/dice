namespace phoenix {

struct pCanvas : public pWidget {
  Canvas& canvas;

  void setSize(Size size);
  void update();

  pCanvas(Canvas& canvas) : pWidget(canvas), canvas(canvas) {}
  void constructor();
  void destructor();
};

}
