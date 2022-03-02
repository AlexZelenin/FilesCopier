#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "copymanager.h"
#include <QFileInfoList>
#include <QTimer>
#include <QTime>


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

signals:

private slots:
    void openSourceDir();
    void openTargetDir();
    void runCopy();
    void interruptCopy();

private:
    Ui::MainWindow *ui;

    CopyManager *m_copyManager;
    QFileInfoList m_list;

    QTimer *m_timer;
    QTime m_time;
};
#endif // MAINWINDOW_H
