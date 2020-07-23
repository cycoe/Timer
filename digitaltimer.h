#ifndef DIGITALTIMER_H
#define DIGITALTIMER_H

#include <QWidget>
#include <QTimer>
#include <QTime>
#include <QList>

namespace Ui {
  class DigitalTimer;
}

class DigitalTimer : public QWidget
{
  Q_OBJECT

public:
  explicit DigitalTimer(QWidget *parent = nullptr);
  ~DigitalTimer();
  void set_current_time();
  void set_current_time(QTime const& time);
  void set_current_time(const QTime& time, QString const& display);
  QTime const& current_time();
  void set_palette();
  void set_palette(int palette_index);
  void start_time();
  void pause_time();
  void reset_time();

  static enum  {
    error,
    warning,
    good,
    normal,
  } palettes_e;

signals:
  void timer_heart_beat();
  void start_time_changed();

private slots:
  void on_btn_start_clicked();
  void on_timer_heart_beat();
  void on_btn_reset_clicked();
  void on_sb_total_time_valueChanged(int);

private:
  void add_lcd_palettes();
  void add_time_boundaries();
  void sync_time_to_display();

  Ui::DigitalTimer *ui;
  QTimer* __timer = nullptr;
  QTime __current;
  QList<QPalette> __lcd__palettes;
  QList<QTime> __time_boundaries;
};

#endif // DIGITALTIMER_H
