#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <array>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_pushButton_pressed();
    void on_pushButton_released();

    void on_pushButton_2_clicked();

    void on_checkBox_stateChanged(int arg1);
    void on_checkBox_2_stateChanged(int arg1);
    void on_checkBox_3_stateChanged(int arg1);
    void on_checkBox_4_stateChanged(int arg1);

    void on_spinBox_valueChanged(int arg1);

    void update();

    void show_image_0();
    void show_image_1();
    void show_image_2();
    void show_image_3();

    void hide_image_0();
    void hide_image_1();
    void hide_image_2();
    void hide_image_3();

private:
    Ui::MainWindow *ui;

    std::array<int, 4> led_on;

    int count;

    QTimer *timer;
};
#endif // MAINWINDOW_H
