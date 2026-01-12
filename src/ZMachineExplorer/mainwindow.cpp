#include <QDebug>
#include <QMessageBox>

#include "mainwindow.h"
#include "ui_mainwindow.h"

#define STORY_FILE_FILTER "Z-Machine story files (*.z1 *.z2 *.z3 *.z4 *.z5 *.z6 *.z7 *.z8);;" \
    "All files (*.*)"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->tabWidget->setCurrentIndex(0);
    m_fileDialog = new QFileDialog(this, "Open Story File", "", STORY_FILE_FILTER);
    m_fileDialog->setFileMode(QFileDialog::ExistingFile);
    m_fileDialog->setAcceptMode(QFileDialog::AcceptOpen);
    connect(m_fileDialog, &QFileDialog::fileSelected, this, &MainWindow::on_fileDialog_accepted);
}

MainWindow::~MainWindow()
{
    delete m_fileDialog;
    delete ui;
}

void MainWindow::on_actionQuit_triggered()
{
    QApplication::exit(0);
}

void MainWindow::on_actionOpenStoryFile_triggered()
{
    m_fileDialog->show();
}

void MainWindow::on_fileDialog_accepted(const QString& file)
{
    qDebug() << "Loading file " << file;
    if(!m_vm.loadFromFile(file))
    {
        QString error = m_vm.lastError().isEmpty()?"Unspecified error":m_vm.lastError();
        QMessageBox::critical(this, "Unable to open file", error);
    }
}


