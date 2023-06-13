#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "facade.h"
#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    void on_open_file_clicked();
    void on_check_file_clicked();
    Facade facade;
    //std::string fp;
};
#endif // MAINWINDOW_H
