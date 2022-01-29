#include "mainwindow.h"
#include <QDebug>
#include <QFile>
#include <QFileDialog>
#include <QStandardPaths>
#include <QMessageBox>
#include <QDragEnterEvent>
#include <QMimeData>


MainWindow::MainWindow(QWidget *parent)
    : QWidget(parent),version(new Version)
{
    this->setAcceptDrops(true);
    setFixedSize(640,480);
    initUI();
    initData();
}

MainWindow::~MainWindow()
{
}

void MainWindow::initUI()
{
    qtVersionCMB = new QComboBox;
    buildVersionCMB = new QComboBox;
    user_ExeFile_Btn = new QPushButton("拖拽/选择文件");
    user_ExeFile_Btn->setSizePolicy(QSizePolicy::Policy::Expanding,QSizePolicy::Policy::Expanding);
    user_ExeFile_Btn->setObjectName("user_ExeFile_Btn");
    generateBtn = new QPushButton("生成");
    testBtn = new QPushButton("测试");
    aboutBtn = new QPushButton("关于");

    auto* flayout = new QFormLayout();
    flayout->addRow("选择Qt版本", qtVersionCMB);
    flayout->addRow("选择编译器版本", buildVersionCMB);
    flayout->setContentsMargins(20,0,20,0);

    auto* hlayout = new QHBoxLayout();
    hlayout->addWidget(testBtn);
    hlayout->addWidget(aboutBtn);

    auto* vlayout = new QVBoxLayout();
    vlayout->setSpacing(20);
    vlayout->addLayout(flayout);
    vlayout->addWidget(user_ExeFile_Btn);
    vlayout->addWidget(generateBtn);
    vlayout->addLayout(hlayout);

    this->setLayout(vlayout);

    QFile stylefile("://Resource/style.css");
    if(stylefile.open(QIODevice::ReadOnly|QIODevice::Text)){
        this->setStyleSheet(stylefile.readAll());
    }
    stylefile.close();

    connect(qtVersionCMB, &QComboBox::currentTextChanged, this->version, &Version::set_curqtVersion);
    connect(qtVersionCMB, &QComboBox::currentTextChanged, this, [=](const QString& version){
        this->buildVersionCMB->clear();
        this->buildVersionCMB->addItems(this->version->bulidVersion());
    });
    connect(buildVersionCMB, &QComboBox::currentTextChanged, this->version, &Version::set_curbulidVersion);
    connect(user_ExeFile_Btn,&QPushButton::released, this, [=](){
        QString userfilepath = QFileDialog::getOpenFileName(this, "选择exe", QStandardPaths::writableLocation(QStandardPaths::DesktopLocation), "exe(*.exe)");
        if(userfilepath.isEmpty()){
            return;
        }
        this->version->set_userExeFile(userfilepath);
        QFileInfo finfo = QFileInfo(userfilepath);
        this->user_ExeFile_Btn->setText(finfo.fileName());
    });
    connect(generateBtn, &QPushButton::released, this, &MainWindow::generate);
    connect(testBtn, &QPushButton::released, this, &MainWindow::testApp);
}

void MainWindow::initData()
{
    this->qtVersionCMB->addItems(version->qtVersion());
}

bool MainWindow::generate()
{
    qDebug()<<"info [qt version]"<<this->version->get_curqtVersion();
    qDebug()<<"info [build version]"<<this->version->get_curbulidVersion();
    qDebug()<<"info [user file]"<<this->version->get_userExeFile();

    QDir dir = QDir(QDir::homePath() + "/AppData/Roaming/Microsoft/Windows/Start Menu/Programs/"
                    + this->version->get_curqtVersion() + "/" + this->version->get_build_dir_name()
                    + "/" + this->version->get_curbulidVersion());
    QStringList flist = dir.entryList(QDir::Filter::Files);
    if(flist.empty()){
        qWarning()<<"qt编译套件的链接获取失败";
        return false;
    }
    QFileInfo finfo(dir.path() + "/" + flist.first());
    QString qtBinPath = QFileInfo(finfo.symLinkTarget()).path();
    //设置环境变量
    QString path = qtenvpath(qtBinPath) + qgetenv("PATH");
    qputenv("PATH", path.toLocal8Bit());
    // 1
    // system(QString(qtBinPath + "/" + "windeployqt.exe " + this->version->get_userExeFile()).toStdString().c_str());
    // 2
    QProcess process;
    process.start(QString(qtBinPath + "/" + "windeployqt.exe " + this->version->get_userExeFile()).toStdString().c_str());
    process.waitForStarted();
    process.waitForFinished();
    qDebug()<<"info [Process OutPut Info]" << process.readAllStandardOutput();
    qDebug()<<"info [Process Error Info]" << process.readAllStandardError();
    qDebug()<<"info [Process Exit Status]"<< process.exitCode()<<" "<<process.exitStatus();
    if(process.exitCode()!= 0||process.exitStatus()!=QProcess::NormalExit){
        return false;
    }
    return true;
}

void MainWindow::testApp()
{
    m_process = new QProcess();
    m_process->start(this->version->get_userExeFile());
    connect(m_process, &QProcess::readAllStandardOutput, this ,[=](){
        qDebug()<<m_process->readAllStandardOutput();
    });
    connect(m_process, &QProcess::readAllStandardError, this ,[=](){
        qDebug()<<m_process->readAllStandardError();
    });
    connect(m_process,QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished),this, [=](int exitcode, QProcess::ExitStatus exitstatus){
        emit testAppfinished(exitcode==0&&exitstatus==QProcess::NormalExit);
    });
    connect(this,&MainWindow::testAppfinished, this,[=](bool ok){
        if(ok){
           qDebug()<<"ok";
        }
        else{
            qWarning()<<"NO";
        }
    });
}

void MainWindow::dropEvent(QDropEvent *event)
{
    const QMimeData* data = event->mimeData();
    if(data->hasUrls()){
        QString drag_exe_file = data->urls().first().path().mid(1);
        this->version->set_userExeFile(drag_exe_file);
        this->user_ExeFile_Btn->setText(QFileInfo(drag_exe_file).fileName());
    }
}

void MainWindow::dragEnterEvent(QDragEnterEvent *event)
{
    event->accept();
}

QString MainWindow::qtenvpath(const QString& envPath)
{
     QFile file(envPath + "/qtenv2.bat");
     if(!file.open(QIODevice::ReadOnly | QIODevice::Text)){
         qWarning("qtenv2.bat open failed");
         return QString();
     }
     QString res;
     while(!file.atEnd()){
         QString lineData = file.readLine();
         if(lineData.indexOf("set PATH=")!= -1){
             res = lineData.mid(9).remove("%PATH%").trimmed();
         }
     }
     file.close();
     return res;
}








