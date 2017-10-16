#pragma once

#include <QMainWindow>
#include <QWidget>

namespace Ui
{
class _MainWindow;
}

class Window : public QMainWindow
{

public:
    Window(QWidget* parent = nullptr);
    ~Window();

    void countEvents(QString obs);
    int runCmd(QString cmd);
    void runRTKLIB();
private:
    Ui::_MainWindow *ui;

    QString _savedPath;
    QString _folderName;
    QString _findFolder;
};
