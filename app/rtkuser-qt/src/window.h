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

    int runCmd(QString cmd);
    void runRTKLIB();
private:
    Ui::_MainWindow *ui;

    QString _savedPath;
    QString _folderName;
};
