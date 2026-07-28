#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileDialog>
#include <QString>
#include <zmachinevm.h>

#include "headermodel.h"
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
    void connectSignals();
    void disconnectSignals();

private slots:
    void onActionQuitTriggered();
    void onActionOpenStoryFileTriggered();
    void onFileDialogAccepted(const QString& filePath);
    void onRecentFileSelected(QAction* recentFile);
    void onInterpreterNumberSelected(QAction* numberItem);
    void onActionClearHistoryTriggered();
    void onActionRestartTriggered();
    void onBtnUpdateObjectsClicked();

private:
    Ui::MainWindow *ui;
    QFileDialog* m_fileDialog;
    ZMachineCore::ZMachineVM m_vm;
	RecentFilesManager m_recentMgr;
    HeaderModel* m_headerModel;
    HeaderModel* m_flags1Model;
    HeaderModel* m_flags2Model;
};
#endif // MAINWINDOW_H
