#include <QDebug>
#include <QMessageBox>
#include <QDir>

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

	const QStringList recentFiles = m_recentMgr.getRecentFiles();
	for(int i = 0; i < recentFiles.length(); i++)
	{
		ui->menuRecentStoryFiles->addAction(recentFiles[i]);

	}
	// for (const QString recentFile: recentFiles) {
	// }
	connect(ui->menuRecentStoryFiles, &QMenu::triggered, this, &MainWindow::on_recentFileSelected);
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
	if(!m_vm.loadFromFile(file))
	{
		QString error = m_vm.lastError().isEmpty()?"Unspecified error":m_vm.lastError();
		QMessageBox::critical(this, "Unable to open file", error);
		return;
	}

	if(!addToRecent) return;
	m_recentMgr.addRecentFile(file);
	QList<QAction*> actions = ui->menuRecentStoryFiles->actions();
	QAction* pathAction = new QAction(file);
	if(actions.length() > 1)
	{
		ui->menuRecentStoryFiles->insertAction(actions[1], pathAction);
	}
	else
	{
		ui->menuRecentStoryFiles->addAction(pathAction);
	}
}

void MainWindow::clearRecentFiles()
{
	m_recentMgr.clearRecentFiles();
	QList<QAction*> actions = ui->menuRecentStoryFiles->actions();
	for(int i = 1; i < actions.length(); i++) {
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

void MainWindow::on_fileDialog_accepted(const QString& file)
{
	openFile(file);
}

void MainWindow::on_recentFileSelected(QAction* recentFile) {
	openFile(recentFile->text(), false);
}

void MainWindow::on_actionClearHistory_triggered()
{
	clearRecentFiles();
}
