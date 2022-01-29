#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>
#include <QComboBox>
#include <QPushButton>
#include <QFormLayout>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QFile>
#include <QProcess>
#include "version.h"
#include <QDragEnterEvent>

class MainWindow : public QWidget
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void initUI();
    void initData();
    bool generate();
    void testApp();
    void dropEvent(QDropEvent *event) override;
    void dragEnterEvent(QDragEnterEvent *event) override;
    static QString qtenvpath(const QString& envPath);
signals:
    void testAppfinished(bool isok);
private:
    QComboBox* qtVersionCMB;
    QComboBox* buildVersionCMB;
    QPushButton* user_ExeFile_Btn;
    QPushButton* generateBtn;
    QPushButton* testBtn;
    QPushButton* aboutBtn;
    QProcess* m_process;

    Version* version;
};
#endif // MAINWINDOW_H
