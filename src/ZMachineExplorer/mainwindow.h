#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileDialog>
#include <QString>
#include <zmachinevm.h>

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

private slots:
    void on_actionQuit_triggered();
    void on_actionOpenStoryFile_triggered();
    void on_fileDialog_accepted(const QString& filePath);

private:
    Ui::MainWindow *ui;
    QFileDialog* m_fileDialog;
    ZMachineCore::ZMachineVM m_vm;
};
#endif // MAINWINDOW_H
