#ifndef QLWIDGET_H
#define QLWIDGET_H

#ifdef __APPLE__
#define GL_SILENCE_DEPRECATION
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#else
#include <GL/gl.h>
#include <GL/glu.h>
#endif

#include <QColor>
#include <QColorDialog>
#include <QOpenGLWidget>
#include <QWidget>
#include <QtOpenGL/QtOpenGL>
// #include <QDebug>

#ifdef __cplusplus
extern "C" {
#endif
#include "../backend/my_viewer.h"
#ifdef __cplusplus
};
#endif

typedef struct colorStruct {
  QColor colorVertex;
  QColor colorLine;
  QColor colorBG;

  double colorVertexRed;
  double colorVertexGreen;
  double colorVertexBlue;

  double colorLineRed;
  double colorLineGreen;
  double colorLineBlue;

  double colorBGRed;
  double colorBGGreen;
  double colorBGBlue;
} colorData;

class QLWidget : public QOpenGLWidget {
 public:
  explicit QLWidget(QWidget *parent = nullptr);
  data_t my_data;
  void paintGL() override;
  int index_projection = 0;
  int index_line = 0;
  void line_preferences(int index_line);
  int size_of_line = 1;
  int index_dot = 0;
  void dot_preferences(int index_dot);
  int size_of_dot = 5;
  colorData allColor;

 public slots:
  void model_rotate();
  void model_move();
  void model_scale(double factor);
  void x_rotate(double angle);
  void y_rotate(double angle);
  void z_rotate(double angle);
  void x_move(double coord);
  void y_move(double coord);
  void z_move(double coord);
  void setSensitivity(double sens);

 private:
  ~QLWidget();
  double model_angles[3]{0};
  double form_angles[3]{0};
  double model_centers[3]{0};
  double form_centers[3]{0};
  double model_scale_factor = 1;
  double mouse_sensitivity = 0.05;
  QPoint mouse_position;
  void mousePressEvent(QMouseEvent *) override;
  void mouseMoveEvent(QMouseEvent *) override;
};

#endif  // QLWIDGET_H
