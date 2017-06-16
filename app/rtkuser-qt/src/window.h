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

private:
    Ui::_MainWindow *ui;
};
