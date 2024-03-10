#include "qlwidget.h"

QLWidget::QLWidget(QWidget *parent) : QOpenGLWidget(parent) {
  my_init_data(&my_data);
}

QLWidget::~QLWidget() { my_remove_data(&my_data); }

void QLWidget::paintGL() {
  glClearColor(allColor.colorBGRed, allColor.colorBGGreen, allColor.colorBGBlue,
               1.0);

  glEnable(GL_DEPTH_TEST);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  if (index_projection) {
    glFrustum(-1, 1, -1, 1, 1, 4);
  }

  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  if (index_projection) {
    glTranslated(0, 0, -2);
  }

  glColor3d(allColor.colorLineRed, allColor.colorLineGreen,
            allColor.colorLineBlue);

  line_preferences(index_line);
  double x = 0, y = 0, z = 0;
  glLineWidth((GLfloat)size_of_line);
  if (my_data.vertices_count != 0) {
    for (int i = 0; i < my_data.facets_count; i++) {
      glBegin(GL_LINE_LOOP);
      for (int j = 0; j < my_data.facets[i].count_of_vertices_in_facet;
           j++) {
        if (my_data.facets[i].vertices[j] <= my_data.vertices_count) {
          x = my_data.coordinate_matrix
                  .matrix[my_data.facets[i].vertices[j] - 1][0];
          y = my_data.coordinate_matrix
                  .matrix[my_data.facets[i].vertices[j] - 1][1];
          z = my_data.coordinate_matrix
                  .matrix[my_data.facets[i].vertices[j] - 1][2];

          glVertex3d(x, y, z);
        }
      }
      glEnd();
    }

    dot_preferences(index_dot);
    for (int i = 0; i < my_data.facets_count && index_dot != 2; i++) {
      glEnable(GL_BLEND);
      glPointSize(size_of_dot);
      glBegin(GL_POINTS);
      for (int j = 0; j < my_data.facets[i].count_of_vertices_in_facet;
           j++) {
        if (my_data.facets[i].vertices[j] <= my_data.vertices_count) {
          x = my_data.coordinate_matrix
                  .matrix[my_data.facets[i].vertices[j] - 1][0];
          y = my_data.coordinate_matrix
                  .matrix[my_data.facets[i].vertices[j] - 1][1];
          z = my_data.coordinate_matrix
                  .matrix[my_data.facets[i].vertices[j] - 1][2];
          glColor3d(allColor.colorVertexRed, allColor.colorVertexGreen,
                    allColor.colorVertexBlue);
          glVertex3d(x, y, z);
        }
      }
      glEnd();
    }
  }
}

/* Rotation functions */

void QLWidget::model_rotate() {
  double rotation_step_angles[3]{0};
  double tmp_translation[3]{0};
  double deg_to_rad_multiplier = M_PI / 180;
  int status = OK;
  for (int axis = 0; axis < 3; axis++) {
    rotation_step_angles[axis] =
        (form_angles[axis] - model_angles[axis]) * deg_to_rad_multiplier;
    tmp_translation[axis] = -form_centers[axis];
  }
  status = my_data.vertices_count > 0 ? OK : ERROR;
  status =
      (status == OK) ? affine_translate(&my_data, tmp_translation) : status;
  status = (status == OK) ? affine_rotate(&my_data, rotation_step_angles, 0)
                          : status;
  status = (status == OK) ? affine_translate(&my_data, form_centers) : status;
  for (int axis = 0; axis < 3; axis++) {
    model_angles[axis] = form_angles[axis];
  }
  if (status == OK) {
    update();
  }
}

void QLWidget::x_rotate(double angle) {
  form_angles[0] = angle;
  model_rotate();
}

void QLWidget::y_rotate(double angle) {
  form_angles[1] = angle;
  model_rotate();
}

void QLWidget::z_rotate(double angle) {
  form_angles[2] = angle;
  model_rotate();
}

void QLWidget::mousePressEvent(QMouseEvent *mouse) {
  mouse_position = mouse->pos();
}

void QLWidget::mouseMoveEvent(QMouseEvent *mouse) {
  double x_rot = 0, y_rot = 0;
  x_rot = (mouse->pos().y() - mouse_position.y());
  y_rot = (mouse->pos().x() - mouse_position.x());
  form_angles[0] -= mouse_sensitivity * x_rot;
  form_angles[1] -= mouse_sensitivity * y_rot;
  model_rotate();
}

void QLWidget::setSensitivity(double sens) { mouse_sensitivity = sens / 1000; }

/* Translation functions */

void QLWidget::model_move() {
  int status = OK;
  double translation_vector[3]{0};
  for (int axis = 0; axis < 3; axis++) {
    translation_vector[axis] = (form_centers[axis] - model_centers[axis]);
  }
  status = my_data.vertices_count > 0 ? OK : ERROR;
  status =
      (status == OK) ? affine_translate(&my_data, translation_vector) : status;
  for (int axis = 0; axis < 3; axis++) {
    model_centers[axis] = form_centers[axis];
  }
  if (status == OK) {
    // paintGL();
    update();
  }
}

void QLWidget::x_move(double coord) {
  form_centers[0] = coord;
  model_move();
}

void QLWidget::y_move(double coord) {
  form_centers[1] = coord;
  model_move();
}

void QLWidget::z_move(double coord) {
  form_centers[2] = coord;
  model_move();
}

/*Scale functions*/

void QLWidget::model_scale(double factor) {
  int status = OK;
  double tmp_translation[3]{0};
  factor /= 100;
  for (int i = 0; i < 3; i++) {
    tmp_translation[i] = -form_centers[i];
  }
  status = my_data.vertices_count > 0 ? OK : ERROR;
  status =
      (status == OK) ? affine_translate(&my_data, tmp_translation) : status;
  status = (status == OK) ? affine_scale(&my_data, factor / model_scale_factor)
                          : status;
  status = (status == OK) ? affine_translate(&my_data, form_centers) : status;
  model_scale_factor = factor;
  if (status == OK) {
    update();
  }
  model_move();
}

void QLWidget::line_preferences(int index_line) {
  if (!index_line) {
    glDisable(GL_LINE_STIPPLE);
  } else {
    // glLineStipple(3, 0xAAAA);
    glLineStipple(5, 0x00FF);
    glEnable(GL_LINE_STIPPLE);
  }
}

void QLWidget::dot_preferences(int index_dot) {
  if (index_dot == 0) {
    glEnable(GL_POINT_SMOOTH);
  } else if (index_dot == 1) {
    glDisable(GL_POINT_SMOOTH);
  }
}
