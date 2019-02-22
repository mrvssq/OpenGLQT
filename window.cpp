// window.cpp

#include <QtWidgets>
#include "window.h"
#include "ui_window.h"

#include "myglwidget.h"

Window::Window(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Window)
{
    ui->setupUi(this);

    connect(ui->myGLWidget, SIGNAL(xRotationChanged(int)), ui->rotXSlider, SLOT(setValue(int)));
    connect(ui->myGLWidget, SIGNAL(yRotationChanged(int)), ui->rotYSlider, SLOT(setValue(int)));
    connect(ui->myGLWidget, SIGNAL(zRotationChanged(int)), ui->rotZSlider, SLOT(setValue(int)));

    connect(ui->pushButtonSetColor, SIGNAL(clicked()), this, SLOT(ButtonSetColorWall()));
    connect(ui->pushButtonPlast, SIGNAL(clicked()), this, SLOT(ButtonPlast()));
    connect(ui->pushButtonMinus, SIGNAL(clicked()), this, SLOT(ButtonMinus()));
    connect(ui->pushButtonSetRadius, SIGNAL(clicked()), this, SLOT(ButtonSetRadius()));

    connect(ui->pushButtonUpLatitude, SIGNAL(clicked()), this, SLOT(ButtonUpLatitude()));
    connect(ui->pushButtonDownLatitude, SIGNAL(clicked()), this, SLOT(ButtonDownLatitude()));
    connect(ui->pushButtonUpLongitude, SIGNAL(clicked()), this, SLOT(ButtonUpLongitude()));
    connect(ui->pushButtonDownLongitude, SIGNAL(clicked()), this, SLOT(ButtonDownLongitude()));
    connect(ui->radioButtonFrame, SIGNAL(clicked()), this, SLOT(on_radioButtonFrame_clicked()));
    connect(ui->radioButtonPoints, SIGNAL(clicked()), this, SLOT(on_radioButtonPoints_clicked()));
    connect(ui->radioButtonSerface, SIGNAL(clicked()), this, SLOT(on_radioButtonSerface_clicked()));
    connect(ui->horizontalSliderThick, SIGNAL(sliderMoved(int)), this, SLOT(setThickLinePoint(int)));

    timerRotate = new QTimer();
    connect(timerRotate, SIGNAL(timeout()), this, SLOT(slotTimerRotate()));
    connect(ui->pushButtonStop, SIGNAL(clicked()), this, SLOT(ButtonStopTimerRotate()));
    connect(ui->horizontalSliderRotate, SIGNAL(sliderMoved(int)), this, SLOT(setTimerSpeed(int)));


    connect(ui->horizontalSliderParam1, SIGNAL(sliderMoved(int)), this, SLOT(setParam1(int)));
    connect(ui->horizontalSliderParam2, SIGNAL(sliderMoved(int)), this, SLOT(setParam2(int)));
    connect(ui->horizontalSliderParam3, SIGNAL(sliderMoved(int)), this, SLOT(setParam3(int)));

    connect(ui->comboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(getTChooseFigure(int)));
}

void Window::ButtonSetColorWall(){
    int r = ui->lineEditWallR->text().toInt();
    int g = ui->lineEditWallG->text().toInt();
    int b = ui->lineEditWallB->text().toInt();

    ui->myGLWidget->setColorWall(r,g,b);
}

void Window::ButtonPlast(){
    ui->myGLWidget->plastScale();
}

void Window::ButtonMinus(){
    ui->myGLWidget->minusScale();
}

void Window::ButtonSetRadius(){
    double a = ui->lineEditRadius1->text().toDouble();
    double b = ui->lineEditRadius2->text().toDouble();
    double c = ui->lineEditRadius3->text().toDouble();
    ui->myGLWidget->setRadius(a, b, c);

    ui->myGLWidget->setUV(ui->lineEditFromU->text().toDouble(),
                          ui->lineEditBeforeU->text().toDouble(),
                          ui->lineEditFromV->text().toDouble(),
                          ui->lineEditBeforeV->text().toDouble());
    ui->horizontalSliderParam1->setValue(static_cast<int>(a * 100.0));
    ui->horizontalSliderParam2->setValue(static_cast<int>(b * 100.0));
    ui->horizontalSliderParam3->setValue(static_cast<int>(c * 100.0));
}

void Window::setParam1(int a){
    ui->lineEditRadius1->setText(QString::number(static_cast<double>(a) / 100));
    ButtonSetRadius();
}

void Window::setParam2(int b){
    ui->lineEditRadius2->setText(QString::number(static_cast<double>(b) / 100));
    ButtonSetRadius();
}

void Window::setParam3(int c){
    ui->lineEditRadius3->setText(QString::number(static_cast<double>(c) / 100));
    ButtonSetRadius();
}

void Window::on_radioButtonFrame_clicked(){
    ui->horizontalSliderThick->setEnabled(true);
    ui->pushButtonUpLatitude->setEnabled(true);
    ui->pushButtonUpLongitude->setEnabled(true);
    ui->pushButtonDownLatitude->setEnabled(true);
    ui->pushButtonDownLongitude->setEnabled(true);

    ui->myGLWidget->setDisplayType(0);
}

void Window::on_radioButtonPoints_clicked(){
    ui->horizontalSliderThick->setEnabled(true);
    ui->pushButtonUpLatitude->setEnabled(true);
    ui->pushButtonUpLongitude->setEnabled(true);
    ui->pushButtonDownLatitude->setEnabled(true);
    ui->pushButtonDownLongitude->setEnabled(true);

    ui->myGLWidget->setDisplayType(1);
}

void Window::on_radioButtonSerface_clicked()
{
    ui->horizontalSliderThick->setEnabled(false);
    ui->pushButtonUpLatitude->setEnabled(false);
    ui->pushButtonUpLongitude->setEnabled(false);
    ui->pushButtonDownLatitude->setEnabled(false);
    ui->pushButtonDownLongitude->setEnabled(false);
    ui->horizontalSliderThick->setValue(0);

    ui->myGLWidget->setDisplayType(2);
}

void Window::setThickLinePoint(int thick){
    ui->myGLWidget->setThickLinePoint(thick);
}

void Window::ButtonUpLatitude(){
    double step = ui->lineEditStepU->text().toDouble();
    double oldStep = ui->labelCountLatitude->text().toDouble();
    ui->labelCountLatitude->setText(QString::number(oldStep + step));
    ui->myGLWidget->addCountLatitude(step);
}

void Window::ButtonDownLatitude(){
    double step = 0 - ui->lineEditStepU->text().toDouble();
    double oldStep = ui->labelCountLatitude->text().toDouble();
    ui->labelCountLatitude->setText(QString::number(oldStep+step));
    ui->myGLWidget->addCountLatitude(step);
}

void Window::ButtonUpLongitude(){
    double step = ui->lineEditStepV->text().toDouble();
    double oldStep = ui->labelCountLongitude->text().toDouble();
    ui->labelCountLongitude->setText(QString::number(oldStep + step));
    ui->myGLWidget->addCountLongitude(step);
}

void Window::ButtonDownLongitude(){
    double step = 0 - ui->lineEditStepV->text().toDouble();
    double oldStep = ui->labelCountLongitude->text().toDouble();
    ui->labelCountLongitude->setText(QString::number(oldStep+step));
    ui->myGLWidget->addCountLongitude(step);
}

void Window::setTimerSpeed(int speed){
    int newSpeed = 0;
    if (timerRotate->isActive() == true)
        timerRotate->stop();
    if (speed != 0){
        newSpeed = int(70/speed);
        if (newSpeed > 0) sideRotate = true;
            else sideRotate = false;
        timerRotate->start(abs(newSpeed));
    }
}

void Window::slotTimerRotate(){
    ui->myGLWidget->rotation(sideRotate);
}

void Window::ButtonStopTimerRotate(){
    timerRotate->stop();
    ui->horizontalSliderRotate->setValue(0);
}

void Window::getTChooseFigure(int item){
    bool a, b, c;
    QString fU, bU, fV, bV;
    QString par1, par2, par3;

    switch (item) {
    case 0://Ellipsoid
        par1 = "3"; par2 = "3"; par3 = "3";
        fU = "0"; bU = "6.283184";
        fV = "-1.570796"; bV = "1.570796";
        a = true; b = true; c = true;
      break;
    case 1://Thor
        par1 = "2"; par2 = "1"; par3 = "0";
        fU = "0"; bU = "6.283184";
        fV = "-3.141592"; bV = "3.141592";
        a = true; b = true; c = false;
      break;
    case 2://Hyperboloid
        par1 = "0.5"; par2 = "0.5"; par3 = "1";
        fU = "0"; bU = "6.283184";
        fV = "-2"; bV = "2";
        a = true; b = true; c = true;
        break;
    case 3://Elliptical paraboloid
        par1 = "2"; par2 = "2"; par3 = "0";
        fU = "-1"; bU = "1";
        fV = "-1"; bV = "1";
        a = true; b = true; c = false;
        break;
    case 4://Logarithmic spiral
        par1 = "1"; par2 = "1"; par3 = "0";
        fU = "0"; bU = "9.424777";
        fV = "-3.131592"; bV = "3.131592";
        a = true; b = true; c = false;
        break;
    case 5://Pseudospere
        par1 = "2"; par2 = "0"; par3 = "0";
        fU = "-3"; bU = "3";
        fV = "-6.283184"; bV = "6.283184";
        a = true; b = false; c = false;
        break;
    case 6://Spiral
        par1 = "3"; par2 = "3"; par3 = "0";
        fU = "-6.283185"; bU = "6.283185";
        fV = "-3.141592"; bV = "3.141592";
        a = true; b = true; c = false;
        break;
    case 7://Dini's Surfase
        par1 = "10"; par2 = "1"; par3 = "0";
        fU = "0"; bU = "18.849555";
        fV = "0"; bV = "2";
        a = true; b = true; c = false;
        break;
    default:
        par1 = "error"; par2 = "error"; par3 = "error";
        fU = "error"; bU = "error";
        fV = "error"; bV = "error";
        a = false; b = false; c = false;
      break;
    }

    ui->lineEditRadius1->setText(par1);
    ui->lineEditRadius2->setText(par2);
    ui->lineEditRadius3->setText(par3);
    ui->horizontalSliderParam1->setValue(par1.toInt());
    ui->horizontalSliderParam2->setValue(par2.toInt());
    ui->horizontalSliderParam3->setValue(par3.toInt());
    ui->lineEditFromU->setText(fU);
    ui->lineEditBeforeU->setText(bU);
    ui->lineEditFromV->setText(fV);
    ui->lineEditBeforeV->setText(bV);
    ui->lineEditRadius1->setEnabled(a);
    ui->lineEditRadius2->setEnabled(b);
    ui->lineEditRadius3->setEnabled(c);
    ui->horizontalSliderParam1->setEnabled(a);
    ui->horizontalSliderParam2->setEnabled(b);
    ui->horizontalSliderParam3->setEnabled(c);

    ui->myGLWidget->setChooseFigure(item);

    ButtonSetRadius();
};

Window::~Window(){
    delete ui;
}

void Window::keyPressEvent(QKeyEvent *e){
    if (e->key() == Qt::Key_Escape)
        close();
    else
        QWidget::keyPressEvent(e);
}
