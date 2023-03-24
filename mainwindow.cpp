#include "mainwindow.h"
#include "./ui_mainwindow.h"

#include "QTimer"

#include <iostream>
#include <memory>

#include "util.h"

// ! Use this
// #define GPIO

void LED_ON(unsigned int pin) {
#ifdef GPIO
    gpio_export(pin);
    gpio_set_dir(pin, "out");
    gpio_set_value(pin, "1");
#endif
}

void LED_OFF(unsigned int pin) {
#ifdef GPIO
    gpio_set_value(pin, "0");
    gpio_unexport(pin);
#endif
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow), interval(1) {
    ui->setupUi(this);
    timer = new QTimer(this);

    connect(timer, SIGNAL(timeout()), this, SLOT(update()));
}

MainWindow::~MainWindow() {
    delete ui;
    delete timer;
}

void MainWindow::on_pushButton_pressed() {
    std::cout << "fuck\n";

    if (led_on[0]) {
        show_image_0();
    } else {
        hide_image_0();
    }
    if (led_on[1]) {
        show_image_1();
    } else {
        hide_image_1();
    }
    if (led_on[2]) {
        show_image_2();
    } else {
        hide_image_2();
    }
    if (led_on[3]) {
        show_image_3();
    } else {
        hide_image_3();
    }
}

void MainWindow::on_pushButton_released() {
    std::cout << "release\n";

    // hide_image_0();
    // hide_image_1();
    // hide_image_2();
    // hide_image_3();
}

void MainWindow::update() {
    if (enable) {
        std::cout << "g1\n";
        ui->checkBox->setCheckState(Qt::Checked);
        ui->checkBox_2->setCheckState(Qt::Checked);
        ui->checkBox_3->setCheckState(Qt::Unchecked);
        ui->checkBox_4->setCheckState(Qt::Unchecked);
        show_image_0();
        show_image_1();
        hide_image_2();
        hide_image_3();
    } else {
        std::cout << "g2\n";
        ui->checkBox_3->setCheckState(Qt::Checked);
        ui->checkBox_4->setCheckState(Qt::Checked);
        ui->checkBox->setCheckState(Qt::Unchecked);
        ui->checkBox_2->setCheckState(Qt::Unchecked);
        hide_image_0();
        hide_image_1();
        show_image_2();
        show_image_3();
    }

    enable = !enable;
}
// timer->stop();

void MainWindow::on_pushButton_2_clicked() {
    timer->start(1000 - interval * 10);
}

void MainWindow::on_pushButton_3_clicked() {
    timer->stop();
}

void MainWindow::on_checkBox_stateChanged(int arg1) {
    led_on[0] = arg1;
}

void MainWindow::on_checkBox_2_stateChanged(int arg1) {
    led_on[1] = arg1;
}

void MainWindow::on_checkBox_3_stateChanged(int arg1) {
    led_on[2] = arg1;
}

void MainWindow::on_checkBox_4_stateChanged(int arg1) {
    led_on[3] = arg1;
}

void MainWindow::on_spinBox_valueChanged(int arg1) {
    // count = arg1;
}

void MainWindow::show_image_0() {
    QLabel *image = ui->label_2;
    image->setPixmap(QPixmap::fromImage(QImage("../res/speaker02.png")));
    image->show();
    LED_ON(PIN0);
}
void MainWindow::show_image_1() {
    QLabel *image = ui->label_3;
    image->setPixmap(QPixmap::fromImage(QImage("../res/speaker02.png")));
    image->show();
    LED_ON(PIN1);
}
void MainWindow::show_image_2() {
    QLabel *image = ui->label_4;
    image->setPixmap(QPixmap::fromImage(QImage("../res/speaker02.png")));
    image->show();
    LED_ON(PIN2);
}
void MainWindow::show_image_3() {
    QLabel *image = ui->label_5;
    image->setPixmap(QPixmap::fromImage(QImage("../res/speaker02.png")));
    image->show();
    LED_ON(PIN3);
}

void MainWindow::hide_image_0() {
    QLabel *image0 = ui->label_2;
    image0->hide();
    LED_OFF(PIN0);
}
void MainWindow::hide_image_1() {
    QLabel *image1 = ui->label_3;
    image1->hide();
    LED_OFF(PIN1);
}
void MainWindow::hide_image_2() {
    QLabel *image2 = ui->label_4;
    image2->hide();
    LED_OFF(PIN2);
}
void MainWindow::hide_image_3() {
    QLabel *image3 = ui->label_5;
    image3->hide();
    LED_OFF(PIN3);
}
void MainWindow::on_horizontalSlider_valueChanged(int value) {
    interval = value;
    timer->start(1000 - interval * 10);
}
