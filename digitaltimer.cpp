#include "digitaltimer.h"
#include "ui_digitaltimer.h"

#include <QPalette>

DigitalTimer::DigitalTimer(QWidget *parent) :
  QWidget(parent),
  ui(new Ui::DigitalTimer)
{
  ui->setupUi(this);
  add_lcd_palettes();
  add_time_boundaries();
  reset_time();
}

DigitalTimer::~DigitalTimer()
{
  delete ui;
  if (__timer != nullptr)
    delete __timer;
}

void DigitalTimer::set_current_time()
{
  // set current time with the number in spinbox
  __current.setHMS(0, ui->sb_total_time->value(), 0, 0);
  sync_time_to_display();
  set_palette();
}

void DigitalTimer::set_current_time(const QTime &time)
{
  // set current time with a specific time
  __current = time;
  sync_time_to_display();
  set_palette();
}

void DigitalTimer::set_current_time(const QTime& time, QString const& display)
{
  // set current time with a specific time and set the lcd display with a text
  __current = time;
  ui->lcd_timer->display(display);
  set_palette();
}

QTime const& DigitalTimer::current_time()
{
  return __current;
}

void DigitalTimer::add_lcd_palettes()
{
  QPalette palette;
  palette.setColor(QPalette::Foreground, QColor(0, 0, 0));
  palette.setColor(QPalette::Background, QColor(255, 150, 150));
  __lcd__palettes.push_back(palette);
  palette.setColor(QPalette::Background, QColor(255, 255, 150));
  __lcd__palettes.push_back(palette);
  palette.setColor(QPalette::Background, QColor(150, 255, 150));
  __lcd__palettes.push_back(palette);
  palette.setColor(QPalette::Background, QColor(150, 150, 150));
  __lcd__palettes.push_back(palette);
  // Let LCD fill background and set the first palette
  ui->lcd_timer->setAutoFillBackground(true);
  ui->lcd_timer->setPalette(__lcd__palettes[3]);
}

void DigitalTimer::add_time_boundaries()
{
  QTime time;
  time.setHMS(0, 0, 0, 0);
  __time_boundaries.push_back(time);
  time.setHMS(0, 0, 30, 0);
  __time_boundaries.push_back(time);
  time.setHMS(0, 1, 0, 0);
  __time_boundaries.push_back(time);
  time.setHMS(0, 2, 0, 0);
  __time_boundaries.push_back(time);
}

void DigitalTimer::sync_time_to_display()
{
  QString timer_text = __current.toString("mm:ss");
  ui->lcd_timer->display(timer_text);
}

void DigitalTimer::set_palette()
{
  // Set palettes with current time
  // These rules are from Toast Master Timer Rules
  if (ui->sb_total_time->value() <= 3) {
    if (__current < __time_boundaries[1])
      ui->lcd_timer->setPalette(__lcd__palettes[1]);
    else if (__current < __time_boundaries[2])
      ui->lcd_timer->setPalette(__lcd__palettes[2]);
    else
      ui->lcd_timer->setPalette(__lcd__palettes[3]);
  } else {
    if (__current < __time_boundaries[2])
      ui->lcd_timer->setPalette(__lcd__palettes[1]);
    else if (__current < __time_boundaries[3])
      ui->lcd_timer->setPalette(__lcd__palettes[2]);
    else
      ui->lcd_timer->setPalette(__lcd__palettes[3]);
  }
}

void DigitalTimer::set_palette(int index)
{
  // Set a specific palette
  ui->lcd_timer->setPalette(__lcd__palettes[index]);
}

void DigitalTimer::start_time()
{
  // Check if there is a timer
  if (__timer != nullptr) {
    // If the timer is actived, stop the timer and set the button text
    if (__timer->isActive()) {
      __timer->stop();
      ui->btn_start->setText("开始(&Start)");
      return;
    }
    // If the timer is inactived, start the timer
    __timer->start(1000);
    ui->btn_start->setText("暂停(&Pause)");
    return;
  }

  // If there is no timer instance, instant one and connect to heart beat
  __timer = new QTimer(this);
  connect(__timer, &QTimer::timeout, this, &DigitalTimer::on_timer_heart_beat);
  connect(__timer, &QTimer::timeout, this, &DigitalTimer::timer_heart_beat);
  __timer->start(1000);
  ui->btn_start->setText("暂停(&Pause)");
}

void DigitalTimer::pause_time()
{
  if (__timer == nullptr)
    return;

  if (__timer->isActive()) {
    __timer->stop();
    ui->btn_start->setText("开始(&Start)");
    return;
  }
}

void DigitalTimer::reset_time()
{
  if (__timer != nullptr) {
    if (__timer->isActive()) {
      __timer->stop();
      ui->btn_start->setText("开始(&Start)");
    }
    delete __timer;
    __timer = nullptr;
  }

  set_current_time();
  sync_time_to_display();
  set_palette();
}

void DigitalTimer::on_timer_heart_beat()
{
  // First thing is updating the time left
  __current = __current.addSecs(-1);
  sync_time_to_display();
  // Second thing is updating palette
  set_palette();
  // If time is up, display the red background and show "Up"
  if (__current == __time_boundaries[0]) {
    ui->lcd_timer->setPalette(__lcd__palettes[0]);
    ui->lcd_timer->display("Up");
    delete __timer;
    __timer = nullptr;
  }
}

void DigitalTimer::on_btn_start_clicked()
{
  start_time();
}

void DigitalTimer::on_btn_reset_clicked()
{
  reset_time();
}

void DigitalTimer::on_sb_total_time_valueChanged(int)
{
  if (__timer == nullptr) {
    set_current_time();
    emit start_time_changed();
  }
}
