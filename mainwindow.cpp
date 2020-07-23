#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "./digitaltimer.h"

#include <QFileDialog>

MainWindow::MainWindow(QWidget *parent)
  : QMainWindow(parent)
  , ui(new Ui::MainWindow)
  , __digital_timer(this)
{
  ui->setupUi(this);
  ui->verticalLayout->addWidget(&__digital_timer);
  set_connects();
}

MainWindow::~MainWindow()
{
  delete ui;
}

void MainWindow::set_connects()
{
  // When the timer heart beat, record the time
  connect(&__digital_timer, &DigitalTimer::timer_heart_beat,
          this, [=](void) {
      __recorder.record(__current, __digital_timer.current_time());
    });
  connect(&__digital_timer, &DigitalTimer::start_time_changed,
          this, [=](void) {
      __recorder.record(__current, __digital_timer.current_time());
    });
}

void MainWindow::on_action_export_triggered()
{
  QString file_name;
  file_name = QFileDialog::getSaveFileName(this, "导出到...", "",
                                           "Excel(*.xls);; Text(*.txt)");
  std::ofstream ofs(file_name.toStdString());
  __recorder.export_data(ofs);
  ofs.close();
}

void MainWindow::on_btn_add_player_clicked()
{
  if (ui->le_player->text().isEmpty() || ui->le_section->text().isEmpty()) {
    ui->statusbar->showMessage("角色或环节不能为空！", 3000);
    return;
  }

  __digital_timer.reset_time();
  QString item = ui->le_section->text() + " - " + ui->le_player->text();
  ui->cb_current_player->addItem(item);
  ui->cb_current_player->setCurrentIndex(ui->cb_current_player->count() - 1);
  ui->le_player->setText("");
  __digital_timer.reset_time();
  // Record current time when player added, because currently the timer heart
  // beat is not start yet.
  __recorder.record(ui->cb_current_player->currentText(),
                    __digital_timer.current_time());
}

void MainWindow::on_cb_current_player_currentIndexChanged(const QString &text)
{
  __digital_timer.pause_time();
  __current = text;
  __digital_timer.set_current_time(__recorder.get_time(__current));
}
