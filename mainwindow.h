#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <./digitaltimer.h>
#include <./recorder.h>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
  Q_OBJECT

public:
  MainWindow(QWidget *parent = nullptr);
  ~MainWindow();

private slots:
  void on_action_export_triggered();
  void on_btn_add_player_clicked();

  void on_cb_current_player_currentIndexChanged(const QString &arg1);

private:
  void set_connects();
  Ui::MainWindow *ui;
  QString __current;
  Recorder __recorder;
  DigitalTimer __digital_timer;
};
#endif // MAINWINDOW_H
