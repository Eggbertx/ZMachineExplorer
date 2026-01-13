#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileDialog>
#include <QString>
#include <zmachinevm.h>

#include "recentfilesmanager.h"

QT_BEGIN_NAMESPACE
namespace Ui {
    class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
	void openFile(QString path, bool addToRecent = true);
	void clearRecentFiles();

private slots:
    void on_actionQuit_triggered();
    void on_actionOpenStoryFile_triggered();
    void on_fileDialog_accepted(const QString& filePath);
	void on_recentFileSelected(QAction* recentFile);
	void on_actionClearHistory_triggered();

private:
    Ui::MainWindow *ui;
    QFileDialog* m_fileDialog;
    ZMachineCore::ZMachineVM m_vm;
	RecentFilesManager m_recentMgr;
};
#endif // MAINWINDOW_H
