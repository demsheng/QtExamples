#ifndef MAINWINDOW_H
#define MAINWINDOW_H
 
#include <QMainWindow>
#include <QMenuBar>
#include <QMenu>
#include <QSettings>
#include <QFileDialog>
#include <QMessageBox>
#include <QApplication>
#include <QLabel>
#include <QActionEvent>
 
class MainWindow : public QMainWindow
{
    Q_OBJECT
 
public:
    MainWindow(QWidget *parent = Q_NULLPTR);
 
private:
    QMenu* fileMenu;
    QMenu* recentFilesMenu;
 
    QAction* openAction;
    QList<QAction*> recentFileActionList;
    const int maxFileNr;
 
    QString currentFilePath;
    QLabel *imageLabel;
 
    void loadFile(const QString& filePath);
    void adjustForCurrentFile(const QString& filePath);
    void updateRecentActionList();
};
 
#endif // MAINWINDOW_H