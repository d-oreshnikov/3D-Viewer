#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <qlwidget.h>

#include <QFileDialog>
#include <QImage>
#include <QMainWindow>
#include <QMovie>
#include <QSettings>
#include <QTimer>

#include "qgifimage.h"
QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
  Q_OBJECT

 public:
  MainWindow(QWidget *parent = nullptr);
  void settingsSaver();
  void settingsLoader();
  void fileRead(QString path);
  ~MainWindow();
 private slots:
  void enable_controls(bool isEnabled);
  void reset_controls();
  void on_reset_button_clicked();

  void on_choose_file_clicked();
  void on_open_button_clicked();
  void connect_rotation_elements();
  void connect_translation_elements();

  void on_xTranslationSlider_valueChanged(int value);
  void on_xTranslationSpinBox_valueChanged(double value);
  void on_yTranslationSlider_valueChanged(int value);
  void on_yTranslationSpinBox_valueChanged(double value);
  void on_zTranslationSlider_valueChanged(int value);
  void on_zTranslationSpinBox_valueChanged(double value);

  void on_save_image_button_clicked();
  void on_gif_button_clicked();
  void take_screenshot();
  void save_gif();

  //  void on_startRecordingButton_clicked();
  //  void on_stopRecordingButton_clicked();
  //  void takeScreenshot();

  void on_comboBox_projection_currentIndexChanged(int index);
  void on_comboBox_line_currentIndexChanged(int index);
  void connect_preferences_buttons();

  void on_line_size_spinbox_valueChanged(int arg1);

  void on_line_size_slider_valueChanged(int value);

  void on_comboBox_dot_currentIndexChanged(int index);

  void on_dot_size_spinbox_valueChanged(int arg1);

  void on_dot_size_slider_valueChanged(int value);

  void on_pushButton_bg_clicked();

  void on_pushButton_vertex_clicked();

  void on_pushButton_line_clicked();

 private:
  Ui::MainWindow *ui;
  QSettings *setting;
  QString pathToFile;
  QGifImage *gif;

  QTimer *timer;

  //  QTimer *timer;
  //  int n_screenshot = 0;
};
#endif  // MAINWINDOW_H
