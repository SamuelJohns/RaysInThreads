#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "raytracer.h"
#include <chrono>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

public slots:
    void on_pushButton_clicked();
    void onDone();
    void updateProgress(int value);
private:
    std::chrono::time_point<std::chrono::system_clock> startTime, endTime;

    Ui::MainWindow *ui;
    RayTracer tracer;
};
#endif // MAINWINDOW_H
