#include "version.h"
#include <QDir>
#include <QDebug>

const QString Version::prama_path = QDir::homePath() + "/AppData/Roaming/Microsoft/Windows/Start Menu/Programs/";

Version::Version(QObject *parent) : QObject(parent)
{

}

QStringList Version::qtVersion()
{
    QDir* dir = new QDir(prama_path);
    return QStringList(dir->entryList(QStringList()<<"Qt*"));
}

QString Version::get_curqtVersion()
{
    return this->cur_qtversion;
}

void Version::set_curqtVersion(QString cur_qtVersion)
{
    this->cur_qtversion = cur_qtVersion;
}

QStringList Version::bulidVersion()
{
    QDir* dir = new QDir(prama_path + this->cur_qtversion);
    QStringList tlist = dir->entryList(QDir::Filter::Dirs|QDir::Filter::NoDotAndDotDot);
    if(tlist.empty()){
        qWarning()<<"Qt编译套件查找失败";
        return QStringList();
    }
    this->build_dir_name = tlist.first();
    dir->cd(this->build_dir_name);
    return QStringList(dir->entryList(QDir::Filter::AllDirs|QDir::Filter::NoDotAndDotDot));
}

QString Version::get_curbulidVersion()
{
    return this->cur_buildversion;
}

void Version::set_curbulidVersion(QString cur_buildVersion)
{
    this->cur_buildversion = cur_buildVersion;
}

QString Version::get_userExeFile()
{
   return this->user_exe_file;
}

void Version::set_userExeFile(QString user_exe_file)
{
    this->user_exe_file = user_exe_file;
}

QString Version::get_build_dir_name()
{
    return this->build_dir_name;
}
