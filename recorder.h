#ifndef RECORDER_H
#define RECORDER_H

#include <QString>
#include <QTime>
#include <QMap>
#include <fstream>

class Recorder
{
public:
  using record_type = QMap<QString, QTime>;
  using size_type = record_type::size_type;
  Recorder();
  void record(QString const& name, QTime const& record);
  void remove_name(QString const& name);
  void remove_by_index(size_type index);
  QTime const& get_time(QString const& name);
  void export_data(std::ofstream& ofs) const;

private:
  record_type __record;
};

#endif // RECORDER_H
