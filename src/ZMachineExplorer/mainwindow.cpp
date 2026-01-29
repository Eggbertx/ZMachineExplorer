#include <QDebug>
#include <QDir>
#include <QMessageBox>

#include "mainwindow.h"
#include "ui_mainwindow.h"

#define STORY_FILE_FILTER \
    "Z-Machine story files (*.z1 *.z2 *.z3 *.z4 *.z5 *.z6 *.z7 *.z8);;" \
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
    ui->actionIBMPC->setChecked(true);
    ui->menuDebug->setEnabled(false);

    const QStringList recentFiles = m_recentMgr.getRecentFiles();
    for (int i = 0; i < recentFiles.length(); i++) {
        ui->menuRecentStoryFiles->addAction(recentFiles[i]);
    }
    connect(ui->menuRecentStoryFiles, &QMenu::triggered, this, &MainWindow::on_recentFileSelected);
    connect(ui->menuInterpreterNumber, &QMenu::triggered, this, &MainWindow::on_interpreterNumberSelected);
    connect(m_fileDialog, &QFileDialog::fileSelected, this, &MainWindow::on_fileDialog_accepted);
}

MainWindow::~MainWindow()
{
    delete m_fileDialog;
    delete ui;
}

void MainWindow::openFile(QString file, bool addToRecent)
{
    qDebug() << "Loading file " << file;
    if (!m_vm.loadFromFile(file)) {
        QString error = m_vm.lastError().isEmpty() ? "Unspecified error" : m_vm.lastError();
        QMessageBox::critical(this, "Unable to open file", error);
        return;
    }

    if (addToRecent) {
        m_recentMgr.addRecentFile(file);
        QList<QAction *> actions = ui->menuRecentStoryFiles->actions();
        QAction *pathAction = new QAction(file);
        if (actions.length() > 1) {
            ui->menuRecentStoryFiles->insertAction(actions[1], pathAction);
        } else {
            ui->menuRecentStoryFiles->addAction(pathAction);
        }
    }

    ui->actionRestart->setEnabled(true);
    ui->menuDebug->setEnabled(true);
}

void MainWindow::clearRecentFiles()
{
    m_recentMgr.clearRecentFiles();
    QList<QAction *> actions = ui->menuRecentStoryFiles->actions();
    for (int i = 1; i < actions.length(); i++) {
        ui->menuRecentStoryFiles->removeAction(actions[i]);
    }
}

void MainWindow::on_actionQuit_triggered()
{
    QApplication::exit(0);
}

void MainWindow::on_actionOpenStoryFile_triggered()
{
    m_fileDialog->show();
}

void MainWindow::on_fileDialog_accepted(const QString &file)
{
    openFile(file);
}

void MainWindow::on_recentFileSelected(QAction *recentFile)
{
    if(recentFile != ui->actionClearHistory)
        openFile(recentFile->text(), false);
}

void MainWindow::on_interpreterNumberSelected(QAction *numberItem)
{
    using namespace ZMachineCore;
    QList<QAction*> actions = ui->menuInterpreterNumber->actions();
    for(int i = 0; i < actions.length(); i++) {
        actions[i]->setChecked(actions[i] == numberItem);
    }
    if(m_vm.zMachineVersion() < 4) {
        return;
    }
    enum InterpreterNum num = m_vm.interpreterNumber();
    if(numberItem == ui->actionDECSystem20) {
        num = InterpreterNum::DECSystem20;
    } else if(numberItem == ui->actionAppleIIe) {
        num = InterpreterNum::AppleIIe;
    } else if (numberItem == ui->actionMacintosh) {
        num = InterpreterNum::Macintosh;
    } else if (numberItem == ui->actionAmiga) {
        num = InterpreterNum::Amiga;
    } else if (numberItem == ui->actionAtariST) {
        num = InterpreterNum::AtariST;
    } else if (numberItem == ui->actionIBMPC) {
        num = InterpreterNum::IBMPC;
    } else if (numberItem == ui->actionCommodore128) {
        num = InterpreterNum::Commodore128;
    } else if (numberItem == ui->actionCommodore64) {
        num = InterpreterNum::Commodore64;
    } else if (numberItem == ui->actionAppleIIc) {
        num = InterpreterNum::AppleIIc;
    } else if (numberItem == ui->actionAppleIIgs) {
        num = InterpreterNum::AppleIIgs;
    } else if (numberItem == ui->actionTandyColor) {
        num = InterpreterNum::TandyColor;
    }
    m_vm.setInterpreterNum(num);
}

void MainWindow::on_actionClearHistory_triggered()
{
    clearRecentFiles();
}

void MainWindow::on_actionRestart_triggered() {}

void MainWindow::on_btnUpdateObjects_clicked() {
    m_vm.getObjectList();
}
