#include "mainwindow.h"

#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
  ui->setupUi(this);
  setWindowTitle("3DViewer_v1.0");
  connect_rotation_elements();
  connect_translation_elements();
  connect(ui->scaleSlider, &QAbstractSlider::valueChanged, ui->openGLWidget,
          &QLWidget::model_scale);
  //    timer = new QTimer(this);
  connect_preferences_buttons();
  setting = new QSettings("3D_Team", "3D_V_S", this);
  settingsLoader();
  // ui->dot_size_slider->setValue(5);
  // ui->dot_size_spinbox->setValue(5);
}

MainWindow::~MainWindow() {
  settingsSaver();
  delete ui;
}

void MainWindow::settingsSaver() {
  setting->setValue("fileName", ui->file_name->text());
  setting->setValue("path", pathToFile);

  setting->beginGroup("Transition");
  setting->setValue("x_spin_box", ui->xTranslationSpinBox->value());
  setting->setValue("y_spin_box", ui->yTranslationSpinBox->value());
  setting->setValue("z_spin_box", ui->zTranslationSpinBox->value());
  setting->endGroup();

  setting->beginGroup("Rotation");
  setting->setValue("x_rotation_box", ui->xRotationSpinBox->value());
  setting->setValue("y_rotation_box", ui->yRotationSpinBox->value());
  setting->setValue("z_rotation_box", ui->zRotationSpinBox->value());
  setting->endGroup();

  setting->setValue("scale", ui->scaleSlider->value());

  setting->setValue("projection", ui->comboBox_projection->currentIndex());

  setting->beginGroup("Line");
  setting->setValue("line_type", ui->comboBox_line->currentIndex());
  setting->setValue("line_width_box", ui->line_size_spinbox->value());
  // setting->setValue("line_width_slider", ui->line_size_slider->value());
  setting->setValue("line_color_red", ui->openGLWidget->allColor.colorLineRed);
  setting->setValue("line_color_green",
                    ui->openGLWidget->allColor.colorLineGreen);
  setting->setValue("line_color_blue",
                    ui->openGLWidget->allColor.colorLineBlue);
  setting->endGroup();

  setting->beginGroup("Vertex");
  setting->setValue("vertex_type", ui->comboBox_dot->currentIndex());
  setting->setValue("vertex_box", ui->dot_size_spinbox->value());
  // setting->setValue("vertex_slider", ui->dot_size_slider->value());
  setting->setValue("vertex_color_red",
                    ui->openGLWidget->allColor.colorVertexRed);
  setting->setValue("vertex_color_green",
                    ui->openGLWidget->allColor.colorVertexGreen);
  setting->setValue("vertex_color_blue",
                    ui->openGLWidget->allColor.colorVertexBlue);
  setting->endGroup();

  setting->beginGroup("Background");
  setting->setValue("background_color_red",
                    ui->openGLWidget->allColor.colorBGRed);
  setting->setValue("background_color_green",
                    ui->openGLWidget->allColor.colorBGGreen);
  setting->setValue("background_color_blue",
                    ui->openGLWidget->allColor.colorBGBlue);
  setting->endGroup();


  // setting->setValue("geometry", geometry())
}

void MainWindow::settingsLoader() {
  ui->file_name->setText(setting->value("fileName", "").toString());
  pathToFile = (setting->value("path", "").toString());
  if (QString(pathToFile).isEmpty() == false) {
    fileRead(pathToFile);
  }
  setting->beginGroup("Transition");
  ui->xTranslationSpinBox->setValue(setting->value("x_spin_box", 0).toDouble());
  ui->yTranslationSpinBox->setValue(setting->value("y_spin_box", 0).toDouble());
  ui->zTranslationSpinBox->setValue(setting->value("z_spin_box", 0).toDouble());
  setting->endGroup();

  setting->beginGroup("Rotation");
  ui->xRotationSpinBox->setValue(
      setting->value("x_rotation_box", 0).toDouble());
  ui->yRotationSpinBox->setValue(
      setting->value("y_rotation_box", 0).toDouble());
  ui->zRotationSpinBox->setValue(
      setting->value("z_rotation_box", 0).toDouble());
  setting->endGroup();

  ui->scaleSlider->setValue(setting->value("scale", 100).toDouble());
  ui->comboBox_projection->setCurrentIndex(
      setting->value("projection", 0).toInt());

  setting->beginGroup("Line");
  ui->comboBox_line->setCurrentIndex(setting->value("line_type", 0).toInt());
  ui->line_size_spinbox->setValue(setting->value("line_width_box", 5).toInt());
  // ui->line_size_slider->setValue(
  //   setting->value("line_width_slider", 5).toInt());
  ui->openGLWidget->allColor.colorLineRed =
      setting->value("line_color_red", 1.0).toDouble();
  ui->openGLWidget->allColor.colorLineGreen =
      setting->value("line_color_green", 0.0).toDouble();
  ui->openGLWidget->allColor.colorLineBlue =
      setting->value("line_color_blue", 0.0).toDouble();
  setting->endGroup();

  setting->beginGroup("Vertex");
  ui->comboBox_dot->setCurrentIndex(setting->value("vertex_type", 0).toInt());
  ui->dot_size_spinbox->setValue(setting->value("vertex_box", 5).toInt());
  // ui->dot_size_slider->setValue(setting->value("vertex_slider", 5).toInt());
  ui->openGLWidget->allColor.colorVertexRed =
      setting->value("vertex_color_red", 0.0).toDouble();
  ui->openGLWidget->allColor.colorVertexGreen =
      setting->value("vertex_color_green", 0.0).toDouble();
  ui->openGLWidget->allColor.colorVertexBlue =
      setting->value("vertex_color_blue", 0.498039).toDouble();
  setting->endGroup();

  setting->beginGroup("Background");
  ui->openGLWidget->allColor.colorBGRed =
      setting->value("background_color_red", 1.0).toDouble();
  ui->openGLWidget->allColor.colorBGGreen =
      setting->value("background_color_green", 1.0).toDouble();
  ui->openGLWidget->allColor.colorBGBlue =
      setting->value("background_color_blue", 1.0).toDouble();
  setting->endGroup();

  // setGeometry(setting->value("geometry", QRect(0, 0, 971, 600)).toRect());
}

void MainWindow::fileRead(QString path) {
  QByteArray ba = (path).toLocal8Bit();
  const char *str = ba.constData();
  my_remove_data(&(ui->openGLWidget->my_data));
  int status = my_parser(str, &(ui->openGLWidget->my_data));
  status = (status == OK) ? normalize_model_data(&(ui->openGLWidget->my_data))
                          : status;
  if (status == OK) {
    enable_controls(true);
    ui->label_vertexes->setText(
        "Вершин: " +
        QString::number(ui->openGLWidget->my_data.vertices_count));
    ui->label_edges->setText(
        "Рёбер: " +
        QString::number(ui->openGLWidget->my_data.facets_count +
                        ui->openGLWidget->my_data.vertices_count - 2));
  } else {
    enable_controls(false);
    ui->label_vertexes->setText("Vertexes: 0");
    ui->label_edges->setText("Edges: 0");
  }
}

void MainWindow::on_choose_file_clicked() {
  pathToFile = QFileDialog::getOpenFileName(this, "Choose file", "../..",
                                            "Object Files (*.obj)");

  QString temp = QFileInfo(pathToFile).fileName();
  ui->file_name->setText(temp);
  fileRead(pathToFile);
}

void MainWindow::on_open_button_clicked() {
  // ui->openGLWidget->paintGL();
  ui->openGLWidget->update();
}

void MainWindow::connect_rotation_elements() {
  connect(ui->xRotationSpinBox, &QDoubleSpinBox::valueChanged,
          ui->xRotationSlider, &QAbstractSlider::setValue);
  connect(ui->xRotationSlider, &QAbstractSlider::valueChanged,
          ui->xRotationSpinBox, &QDoubleSpinBox::setValue);
  connect(ui->yRotationSpinBox, &QDoubleSpinBox::valueChanged,
          ui->yRotationSlider, &QAbstractSlider::setValue);
  connect(ui->yRotationSlider, &QAbstractSlider::valueChanged,
          ui->yRotationSpinBox, &QDoubleSpinBox::setValue);
  connect(ui->zRotationSpinBox, &QDoubleSpinBox::valueChanged,
          ui->zRotationSlider, &QAbstractSlider::setValue);
  connect(ui->zRotationSlider, &QAbstractSlider::valueChanged,
          ui->zRotationSpinBox, &QDoubleSpinBox::setValue);


  connect(ui->xRotationSpinBox, &QDoubleSpinBox::valueChanged, ui->openGLWidget,
          &QLWidget::x_rotate);
  connect(ui->yRotationSpinBox, &QDoubleSpinBox::valueChanged, ui->openGLWidget,
          &QLWidget::y_rotate);
  connect(ui->zRotationSpinBox, &QDoubleSpinBox::valueChanged, ui->openGLWidget,
          &QLWidget::z_rotate);
}

void MainWindow::connect_translation_elements() {
  connect(ui->xTranslationSpinBox, &QDoubleSpinBox::valueChanged,
          ui->openGLWidget, &QLWidget::x_move);
  connect(ui->yTranslationSpinBox, &QDoubleSpinBox::valueChanged,
          ui->openGLWidget, &QLWidget::y_move);
  connect(ui->zTranslationSpinBox, &QDoubleSpinBox::valueChanged,
          ui->openGLWidget, &QLWidget::z_move);
}

void MainWindow::on_xTranslationSlider_valueChanged(int value) {
  ui->xTranslationSpinBox->setValue(value / 100.);
}

void MainWindow::on_xTranslationSpinBox_valueChanged(double value) {
  ui->xTranslationSlider->setValue(value * 100);
}

void MainWindow::on_yTranslationSlider_valueChanged(int value) {
  ui->yTranslationSpinBox->setValue(value / 100.);
}

void MainWindow::on_yTranslationSpinBox_valueChanged(double value) {
  ui->yTranslationSlider->setValue(value * 100);
}

void MainWindow::on_zTranslationSlider_valueChanged(int value) {
  ui->zTranslationSpinBox->setValue(value / 100.);
}

void MainWindow::on_zTranslationSpinBox_valueChanged(double value) {
  ui->zTranslationSlider->setValue(value * 100);
}

void MainWindow::on_save_image_button_clicked() {
  QImage img = ui->openGLWidget->grabFramebuffer();
  QString selectedFilter;
  QString filePath = QFileDialog::getSaveFileName(
      this, "Save Image", "../..", tr("BMP (*.bmp);;JPEG (*.jpeg)"),
      &selectedFilter);
  if (selectedFilter.compare("JPEG (*.jpeg)", Qt::CaseInsensitive) == 0) {
    filePath += ".jpeg";
  } else if (selectedFilter.compare("BMP (*.bmp)", Qt::CaseInsensitive) == 0) {
    filePath += ".bmp";
  }
  if (!filePath.isEmpty()) {
    img.save(filePath);
  }
}

void MainWindow::on_gif_button_clicked() {
  gif = new QGifImage();
  QGifImage gif(QSize(640, 480));
  timer = new QTimer(this);
  timer->setInterval(100);
  timer->singleShot(5000, this, SLOT(save_gif()));

  connect(timer, SIGNAL(timeout()), this, SLOT(take_screenshot()));
  timer->start();
}

void MainWindow::save_gif() {
  timer->stop();
  ui->timer->setText("5.0");

  QString fileName = QFileDialog::getSaveFileName(this, "Save file", "../..",
                                                  "GIF Files (*.gif)");
#ifdef __APPLE__
  gif->save(fileName);
#else
  gif->save(fileName + ".gif");
#endif
}

void MainWindow::take_screenshot() {
  QImage image = ui->openGLWidget->grabFramebuffer();
  double seconds = ui->timer->text().toDouble() - 0.1;
  ui->timer->setText(QString::number(seconds, 'f', 1));
  image.scaled(640, 480, Qt::IgnoreAspectRatio);  // Qt::SmoothTransformation);s
  gif->addFrame(image, 100);
}

void MainWindow::enable_controls(bool isEnabled) {
  if (!isEnabled) {
    reset_controls();
  }
  // ui->comboBox_projection->setEnabled(isEnabled);
  ui->rotationGroupBox->setEnabled(isEnabled);
  ui->translationGroupBox->setEnabled(isEnabled);
  ui->scaleGroupBox->setEnabled(isEnabled);
  ui->image_group_box->setEnabled(isEnabled);
  ui->model_group_box->setEnabled(isEnabled);
  ui->reset_button->setEnabled(isEnabled);
}

void MainWindow::on_reset_button_clicked() {
  reset_controls();
  fileRead(pathToFile);
  ui->openGLWidget->update();
}

void MainWindow::reset_controls() {

  ui->comboBox_projection->setCurrentIndex(0);
  ui->comboBox_line->setCurrentIndex(0);
  ui->line_size_spinbox->setValue(1);
  ui->openGLWidget->allColor.colorLineRed = 1.0;
  ui->openGLWidget->allColor.colorLineGreen = 0.0;
  ui->openGLWidget->allColor.colorLineBlue = 0.0;

  ui->comboBox_dot->setCurrentIndex(0);
  ui->dot_size_spinbox->setValue(5);
  ui->openGLWidget->allColor.colorVertexRed = 0.0;
  ui->openGLWidget->allColor.colorVertexGreen = 0.0;
  ui->openGLWidget->allColor.colorVertexBlue = 0.498039;

  ui->openGLWidget->allColor.colorBGRed = 1.0;
  ui->openGLWidget->allColor.colorBGGreen = 1.0;
  ui->openGLWidget->allColor.colorBGBlue = 1.0;

  ui->xTranslationSpinBox->setValue(0);
  ui->yTranslationSpinBox->setValue(0);
  ui->zTranslationSpinBox->setValue(0);
  ui->xRotationSpinBox->setValue(0);
  ui->yRotationSpinBox->setValue(0);
  ui->zRotationSpinBox->setValue(0);
  ui->scaleSlider->setValue(100);
}

void MainWindow::on_comboBox_projection_currentIndexChanged(int index) {
  ui->openGLWidget->index_projection = index;
  // ui->openGLWidget->paintGL();
  ui->openGLWidget->update();
}

void MainWindow::on_comboBox_line_currentIndexChanged(int index) {
  ui->openGLWidget->index_line = index;
  // ui->openGLWidget->paintGL();
  ui->openGLWidget->update();
}

void MainWindow::connect_preferences_buttons() {
  ui->comboBox_projection->addItem("Параллельная");
  ui->comboBox_projection->addItem("Центральная");
  connect(ui->comboBox_projection,
          QOverload<int>::of(&QComboBox::currentIndexChanged), this,
          &MainWindow::on_comboBox_projection_currentIndexChanged);

  ui->comboBox_line->addItem("Сплошная");
  ui->comboBox_line->addItem("Пунктирная");
  connect(ui->comboBox_line,
          QOverload<int>::of(&QComboBox::currentIndexChanged), this,
          &MainWindow::on_comboBox_line_currentIndexChanged);

  ui->comboBox_dot->addItem("Круглая");
  ui->comboBox_dot->addItem("Квадратная");
  ui->comboBox_dot->addItem("Отсутствует");
  connect(ui->comboBox_dot, QOverload<int>::of(&QComboBox::currentIndexChanged),
          this, &MainWindow::on_comboBox_dot_currentIndexChanged);
}

void MainWindow::on_line_size_spinbox_valueChanged(int arg1) {
  ui->line_size_slider->setValue(arg1);
  ui->openGLWidget->size_of_line = arg1;
  // ui->openGLWidget->paintGL();
  ui->openGLWidget->update();
}

void MainWindow::on_line_size_slider_valueChanged(int value) {
  ui->line_size_spinbox->setValue(value);
  ui->openGLWidget->size_of_line = value;
  // ui->openGLWidget->paintGL();
  ui->openGLWidget->update();
}

void MainWindow::on_comboBox_dot_currentIndexChanged(int index) {
  ui->openGLWidget->index_dot = index;
  // ui->openGLWidget->paintGL();
  ui->openGLWidget->update();
}

void MainWindow::on_dot_size_spinbox_valueChanged(int arg1) {
  ui->dot_size_slider->setValue(arg1);
  ui->openGLWidget->size_of_dot = arg1;
  ui->openGLWidget->update();
}

void MainWindow::on_dot_size_slider_valueChanged(int value) {
  ui->dot_size_spinbox->setValue(value);
  ui->openGLWidget->size_of_dot = value;
  ui->openGLWidget->update();
}

void MainWindow::on_pushButton_vertex_clicked() {
  ui->openGLWidget->allColor.colorVertex =
      QColorDialog::getColor(QColor(0, 0, 127, 255));
  // qDebug() << "Цвет вершин при нажатии на кнопку: " <<
  // ui->openGLWidget->allColor.colorVertex;
  if (ui->openGLWidget->allColor.colorVertex.isValid()) {
    ui->openGLWidget->allColor.colorVertexRed =
        ui->openGLWidget->allColor.colorVertex.redF();
    ui->openGLWidget->allColor.colorVertexGreen =
        ui->openGLWidget->allColor.colorVertex.greenF();
    ui->openGLWidget->allColor.colorVertexBlue =
        ui->openGLWidget->allColor.colorVertex.blueF();
  }
  ui->openGLWidget->update();
}

void MainWindow::on_pushButton_line_clicked() {
  ui->openGLWidget->allColor.colorLine =
      QColorDialog::getColor(QColor(255, 0, 0, 255));
  if (ui->openGLWidget->allColor.colorLine.isValid()) {
    ui->openGLWidget->allColor.colorLineRed =
        ui->openGLWidget->allColor.colorLine.redF();
    ui->openGLWidget->allColor.colorLineGreen =
        ui->openGLWidget->allColor.colorLine.greenF();
    ui->openGLWidget->allColor.colorLineBlue =
        ui->openGLWidget->allColor.colorLine.blueF();
  }
  ui->openGLWidget->update();
}

void MainWindow::on_pushButton_bg_clicked() {
  ui->openGLWidget->allColor.colorBG =
      QColorDialog::getColor(QColor(255, 255, 255, 255));
  if (ui->openGLWidget->allColor.colorBG.isValid()) {
    ui->openGLWidget->allColor.colorBGRed =
        ui->openGLWidget->allColor.colorBG.redF();
    ui->openGLWidget->allColor.colorBGGreen =
        ui->openGLWidget->allColor.colorBG.greenF();
    ui->openGLWidget->allColor.colorBGBlue =
        ui->openGLWidget->allColor.colorBG.blueF();
  }
  ui->openGLWidget->update();
}
