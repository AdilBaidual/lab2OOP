#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , facade(new Work_with_file(), new Json_checker())
{
    //facade = Facade(new Work_with_file(), new Json_checker());
    ui->setupUi(this);
    connect(ui->open_file_btn, &QPushButton::clicked, this, &MainWindow::on_open_file_clicked);
    connect(ui->check_file_btn, &QPushButton::clicked, this, &MainWindow::on_check_file_clicked);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_open_file_clicked() {
    QString file_path = QFileDialog::getOpenFileName(this, " ", "D:\sem2\lab2oop\tests", "*.json");
    if (file_path.isEmpty()) {
        ui->file_path_label->setText("File was not opened");
    }
    else {
        facade.fp = file_path.toStdString();
        ui->file_path_label->setText(file_path);
        std::string res = facade.read_operation(facade.fp);
        ui->json_label->setText(QString::fromStdString(res));

        QLabel *outputForm = ui->json_label;
        QWidget *sawc = ui->scrollAreaWidgetContents;
        int cal = outputForm->text().count("\n")*29;
        sawc->setMinimumHeight(cal);
        outputForm->setGeometry(outputForm->x(), outputForm->y(), outputForm->width(), cal);
    }
}

void MainWindow::on_check_file_clicked() {
    try {
        std::string res_new = facade.read_operation(facade.fp);
        facade.check_operation(res_new);
        std::string message = "File is correct!";
        ui->check_result_label->setText(QString::fromStdString(message));
    }  catch (Json_errors& errors) {
        const char* json_error = errors.what();
        ui->check_result_label->setText(json_error);
    } catch (System_errors& errors) {
        const char* system_error = errors.what();
        ui->check_result_label->setText(system_error);
    }
    facade.rollback();
}

