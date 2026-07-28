#include <QDebug>
#include <QDir>
#include <QMessageBox>

#include "headermodel.h"
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
    m_headerModel = new HeaderModel(&m_vm, HeaderModel::TableMode::Header);
    m_flags1Model = new HeaderModel(&m_vm, HeaderModel::TableMode::Flags1);
    m_flags2Model = new HeaderModel(&m_vm, HeaderModel::TableMode::Flags2);
    ui->headerTable->setModel(m_headerModel);
    ui->flags1Table->setModel(m_flags1Model);
    ui->flags2Table->setModel(m_flags2Model);
    const QStringList recentFiles = m_recentMgr.getRecentFiles();
    for (int i = 0; i < recentFiles.length(); i++) {
        ui->menuRecentStoryFiles->addAction(recentFiles[i]);
    }
    connectSignals();
    for(int i = 1; i <= 3; i++) {
        ui->tabWidget->setTabEnabled(i, false);
    }
}

MainWindow::~MainWindow()
{
    disconnectSignals();
    delete m_fileDialog;
    delete m_headerModel;
    delete m_flags1Model;
    delete m_flags2Model;
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
    ui->btnUpdateObjects->setEnabled(true);
    for(int i = 1; i <= 3; i++) {
        ui->tabWidget->setTabEnabled(i, true);
    }
    m_headerModel->refresh();
    m_flags1Model->refresh();
    m_flags2Model->refresh();
}

void MainWindow::clearRecentFiles()
{
    m_recentMgr.clearRecentFiles();
    QList<QAction *> actions = ui->menuRecentStoryFiles->actions();
    for (int i = 1; i < actions.length(); i++) {
        ui->menuRecentStoryFiles->removeAction(actions[i]);
    }
}

void MainWindow::connectSignals()
{
    connect(ui->actionQuit, &QAction::triggered, this, &MainWindow::onActionQuitTriggered);
    connect(ui->actionOpenStoryFile, &QAction::triggered, this, &MainWindow::onActionOpenStoryFileTriggered);
    connect(ui->menuRecentStoryFiles, &QMenu::triggered, this, &MainWindow::onRecentFileSelected);
    connect(ui->menuInterpreterNumber, &QMenu::triggered, this, &MainWindow::onInterpreterNumberSelected);
    connect(ui->actionClearHistory, &QAction::triggered, this, &MainWindow::onActionClearHistoryTriggered);
    connect(ui->actionRestart, &QAction::triggered, this, &MainWindow::onActionRestartTriggered);
    connect(ui->btnUpdateObjects, &QPushButton::pressed, this, &MainWindow::onBtnUpdateObjectsClicked);
    connect(m_fileDialog, &QFileDialog::fileSelected, this, &MainWindow::onFileDialogAccepted);
}

void MainWindow::disconnectSignals()
{
    disconnect(ui->actionQuit, &QAction::triggered, this, &MainWindow::onActionQuitTriggered);
    disconnect(ui->actionOpenStoryFile, &QAction::triggered, this, &MainWindow::onActionOpenStoryFileTriggered);
    disconnect(ui->menuRecentStoryFiles, &QMenu::triggered, this, &MainWindow::onRecentFileSelected);
    disconnect(ui->menuInterpreterNumber, &QMenu::triggered, this, &MainWindow::onInterpreterNumberSelected);
    disconnect(ui->actionClearHistory, &QAction::triggered, this, &MainWindow::onActionClearHistoryTriggered);
    disconnect(ui->actionRestart, &QAction::triggered, this, &MainWindow::onActionRestartTriggered);
    disconnect(ui->btnUpdateObjects, &QPushButton::pressed, this, &MainWindow::onBtnUpdateObjectsClicked);
    disconnect(m_fileDialog, &QFileDialog::fileSelected, this, &MainWindow::onFileDialogAccepted);
}

void MainWindow::onActionQuitTriggered()
{
    QApplication::exit(0);
}

void MainWindow::onActionOpenStoryFileTriggered()
{
    m_fileDialog->show();
}

void MainWindow::onFileDialogAccepted(const QString &file)
{
    openFile(file);
}

void MainWindow::onRecentFileSelected(QAction *recentFile)
{
    if(recentFile != ui->actionClearHistory)
        openFile(recentFile->text(), false);
}

void MainWindow::onInterpreterNumberSelected(QAction *numberItem)
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

void MainWindow::onActionClearHistoryTriggered()
{
    clearRecentFiles();
}

void MainWindow::onActionRestartTriggered() {}

void MainWindow::onBtnUpdateObjectsClicked()
{
    m_vm.getObjectList();
}
