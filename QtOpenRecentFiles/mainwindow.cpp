#include <QDebug>
#include "mainwindow.h"
 
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), maxFileNr(4)
{
    imageLabel = new QLabel;
    setCentralWidget(imageLabel);
 
    // createActionsAndConnections
    openAction = new QAction(tr("&Open..."), this);
    openAction->setShortcuts(QKeySequence::Open);
    connect(openAction, &QAction::triggered, this, [=]()
    {
        QString filePath = QFileDialog::getOpenFileName(
                           this, tr("Open File"), "",
                           tr("Images (*.png *.xpm *.jpg *.gif)"));
            if (!filePath.isEmpty())
                loadFile(filePath);
    });
 
    QAction* recentFileAction = nullptr;
    for(auto i = 0; i < maxFileNr; ++i){
        recentFileAction = new QAction(this);
        recentFileAction->setVisible(false);
        connect(recentFileAction, &QAction::triggered, this, [=]()
        {
            loadFile(recentFileAction->data().toString());
        });
        recentFileActionList.append(recentFileAction);
    }
 
    // create menus
    fileMenu = menuBar()->addMenu(tr("&File"));
    fileMenu->addAction(openAction);
 
    recentFilesMenu = fileMenu->addMenu(tr("Open Recent"));
    for(auto i = 0; i < maxFileNr; ++i)
        recentFilesMenu->addAction(recentFileActionList.at(i));
 
    updateRecentActionList();
 
    resize(350, 250);
}
 
void MainWindow::loadFile(const QString &filePath){
    QFile file(filePath);
 
    if (!file.open(QFile::ReadOnly)) {
        QMessageBox::warning(this, tr("Recent Files Example"),
                             tr("This file could not be found:\n%1.")
                             .arg(filePath));
        return;
    }
 
    QPixmap pMap(filePath);
    if (pMap.isNull()) {
        QMessageBox::information(this, tr("Recent Files Example"),
                      tr("Cannot load:\n%1.")
                      .arg(filePath));
        return;
    }
 
    imageLabel->setPixmap(pMap);
    imageLabel->setAlignment(Qt::AlignCenter);
    adjustForCurrentFile(filePath);
}
 
void MainWindow::adjustForCurrentFile(const QString &filePath){
    currentFilePath = filePath;

    qDebug()<<"currentFilePath"<<currentFilePath;

    setWindowFilePath(currentFilePath);
 
    QSettings settings;
    QStringList recentFilePaths = settings.value("recentFiles").toStringList();
    recentFilePaths.removeAll(filePath);
    recentFilePaths.prepend(filePath);
    while (recentFilePaths.size() > maxFileNr)
        recentFilePaths.removeLast();
    settings.setValue("recentFiles", recentFilePaths);
 
    // see note
    updateRecentActionList();
}
 
void MainWindow::updateRecentActionList(){
    QSettings settings;
    QStringList recentFilePaths = settings.value("recentFiles").toStringList();
 
    auto itEnd = 0;
    if(recentFilePaths.size() <= maxFileNr)
        itEnd = recentFilePaths.size();
    else
        itEnd = maxFileNr;
 
    for (auto i = 0; i < itEnd; ++i) {
        QString strippedName = QFileInfo(recentFilePaths.at(i)).fileName();
        recentFileActionList.at(i)->setText(strippedName);
        recentFileActionList.at(i)->setData(recentFilePaths.at(i));
        recentFileActionList.at(i)->setVisible(true);
    }
 
    for (auto i = itEnd; i < maxFileNr; ++i)
        recentFileActionList.at(i)->setVisible(false);
}
