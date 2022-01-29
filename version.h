#ifndef VERSION_H
#define VERSION_H

#include <QObject>

class Version : public QObject
{
    Q_OBJECT
public:
    explicit Version(QObject *parent = nullptr);
    QStringList qtVersion();
    QString get_curqtVersion();
    void set_curqtVersion(QString cur_qtVersion);
    QStringList bulidVersion();
    QString get_curbulidVersion();
    void set_curbulidVersion(QString cur_buildversion);
    QString get_userExeFile();
    void set_userExeFile(QString user_exe_file);
    QString get_build_dir_name();
private:
    static const QString prama_path;
    QString cur_qtversion;
    QString cur_buildversion;
    QString build_dir_name;
    QString user_exe_file;
signals:

};

#endif // VERSION_H
