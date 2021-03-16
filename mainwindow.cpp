#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connect(&tracer, &RayTracer::done, this, &MainWindow::onDone);
    connect(&tracer, &RayTracer::updateProgress, this, &MainWindow::updateProgress);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    int threads = ui->lineEdit->text().toInt();
    ui->label->setText("Processing ...");
    ui->pushButton->setEnabled(false);
    ui->lineEdit->setEnabled(false);
    startTime = std::chrono::system_clock::now();
    tracer.run(threads);
}

void MainWindow::onDone() {
    endTime = std::chrono::system_clock::now();
    long time = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime).count();
    ui->label->setText("Done in :");
    ui->label_2->setText(QString::number(time));
    ui->pushButton->setEnabled(true);
    ui->lineEdit->setEnabled(true);
}

void MainWindow::updateProgress(int value)
{
    ui->label_2->setText(QString::number(value) + "%");
}
