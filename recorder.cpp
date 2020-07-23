#include "recorder.h"

Recorder::Recorder()
{

}

void Recorder::record(const QString &name, const QTime &record)
{
  __record.insert(__record.cend(), name, record);
}

void Recorder::remove_name(const QString &name)
{
  record_type::iterator iter;
  if ((iter = __record.find(name)) != __record.cend())
    __record.erase(iter);
}

QTime const& Recorder::get_time(QString const& name)
{
  return __record[name];
}

void Recorder::remove_by_index(size_type index)
{
  if (index < __record.size())
    __record.erase(__record.begin() + index);
}

void Recorder::export_data(std::ofstream& ofs) const
{
  for (auto item = __record.cbegin(); item != __record.cend(); ++item) {
    ofs << item.key().toStdString();
    ofs << "\t";
    ofs << item.value().toString("mm:ss").toStdString();
    ofs << "\n";
  }
}
