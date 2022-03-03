#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "copymanager.h"
#include <QFileInfoList>
#include <QTimer>
#include <QTime>
#include <QFuture>


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
    void timerCanStart();
    void appendMessage(const QString&);

private slots:
    void openSourceDir();
    void openTargetDir();
    void runCopy();
    void interruptCopy();
    void copyProcessHandler();

private:
    Ui::MainWindow *ui;

    CopyManager *m_copyManager;

    QTimer *m_timer;
    QTime m_time;
    QFuture<void> m_future;
};
#endif // MAINWINDOW_H
