#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QFileDevice>
#include <QDir>
#include <QFileInfo>
#include <QFileDialog>
#include <QDebug>
#include <QDateTime>
#include <QDirIterator>
#include <QtConcurrent/QtConcurrent>

#include <map>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , m_copyManager(nullptr)
{
    ui->setupUi(this);

    ui->progressBar->setValue(0);
    ui->progressBar->setTextVisible(false);

    connect(ui->tbSource, &QToolButton::clicked, this, &MainWindow::openSourceDir);
    connect(ui->tbTarget, &QToolButton::clicked, this, &MainWindow::openTargetDir);
    connect(ui->btnStart, &QPushButton::clicked, this, &MainWindow::runCopy);
    connect(ui->btnInterrupt, &QPushButton::clicked, this, &MainWindow::interruptCopy, Qt::QueuedConnection);
    connect(this, &MainWindow::appendMessage, ui->textBrowser, &QTextBrowser::append);

    m_timer = new QTimer(this);
    m_timer->setInterval(0);

    connect(m_timer, &QTimer::timeout, this, &MainWindow::copyProcessHandler, Qt::QueuedConnection);

    connect(this, SIGNAL(timerCanStart()), m_timer, SLOT(start()), Qt::QueuedConnection);

    connect(ui->btnQuit, &QPushButton::clicked, this, [](){
        qApp->quit();
    });
}

MainWindow::~MainWindow()
{
    if (m_copyManager) delete m_copyManager;
    delete ui;
}

void MainWindow::openSourceDir()
{
    const QString& path = QFileDialog::getExistingDirectory(Q_NULLPTR, tr("Выбор директории"), ".");

    if (path.isEmpty()) return;

    ui->leSource->setText(path);
}

void MainWindow::openTargetDir()
{
    const QString& path =  QFileDialog::getExistingDirectory();

    ui->leTarget->setText(path);
}

void MainWindow::runCopy()
{   
    m_future = QtConcurrent::run([this]() {
        emit appendMessage("Идет подготовка к копированию...");
        QDirIterator iter(ui->leSource->text(), QDirIterator::Subdirectories);

        std::list<CMGlobal::FileInfo> list_files;

        const QString& pathTo = ui->leTarget->text();
        const QString& pathFrom = ui->leSource->text();

        QDir dir(ui->leSource->text());

        int i = 0;

        while(iter.hasNext()) {
            QFileInfo finfo = iter.fileInfo();

            if (finfo.isDir()) {
                QString newPath = QString("%1/%2").arg(pathTo, dir.relativeFilePath(finfo.filePath()));
                QDir().mkdir(newPath);
            }

            CMGlobal::FileInfo file_info;

            file_info.filename = dir.relativeFilePath(finfo.filePath()).toStdString();
            file_info.file_size = finfo.size();
            file_info.pathFrom = pathFrom.toStdString();
            file_info.pathTo = pathTo.toStdString();
            file_info.type = !finfo.isDir() ? CMGlobal::FileInfo::File : CMGlobal::FileInfo::Directory;

            if (file_info.filename.empty()) {
                iter.next();
                continue;
            }

            if (file_info.type & CMGlobal::FileInfo::File) {
                i++;
            }

            list_files.push_back(file_info);

            iter.next();
        }

        emit appendMessage(QString("Файлов для копирования: %1").arg(i));

        ui->progressBar->setMaximum(i);

        m_copyManager = new CopyManager(list_files);

        m_copyManager->runCopy();

        m_time.start();
        emit timerCanStart();
    });
}

void MainWindow::interruptCopy()
{
    if (m_copyManager) {
        if (!m_copyManager->isInterrupted())
            m_copyManager->interrupt();
    }
}

void MainWindow::copyProcessHandler()
{
    auto i = m_copyManager->progress();
    ui->progressBar->setTextVisible(true);
    ui->progressBar->setValue(i);

    QFile logFile(qApp->applicationDirPath() + "/log.txt");
    if(logFile.open(QFile::Append | QFile::Text)) {
        QString currTime = "[" +
                QDateTime::currentDateTime().toString("dd.MM.yyyy hh:mm:ss.zzz") + "]";

        QTextStream stream(&logFile);
        while (!m_copyManager->getErrorQueue().empty()) {
            stream << QString("%1 Error - %2\n").arg(currTime, m_copyManager->getErrorQueue().front().c_str());
            m_copyManager->getErrorQueue().pop();
        }

        stream.flush();
        logFile.flush();
        logFile.close();
    }

    if (i == ui->progressBar->maximum() || m_copyManager->isInterrupted()) {
        int msecs = m_time.elapsed();
        QTime time(0,0,0);
        emit appendMessage(QString("Затрачено времени: %1").arg(time.addMSecs(msecs).toString("mm:ss:zzz")));

        m_copyManager->wait();
        m_future.waitForFinished();

        if (m_copyManager) {
            delete m_copyManager;
            m_copyManager = nullptr;
        }
        m_timer->stop();
    }
}
