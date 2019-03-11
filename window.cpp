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
    connect(ui->checkBoxHor, SIGNAL(clicked(bool)), this, SLOT(setShowHor(bool)));
    connect(ui->checkBoxVer, SIGNAL(clicked(bool)), this, SLOT(setShowVer(bool)));

    connect(ui->pushButtonUpVer, SIGNAL(clicked()), this, SLOT(ButtonUpVer()));
    connect(ui->pushButtonDownVer, SIGNAL(clicked()), this, SLOT(ButtonDownVer()));
    connect(ui->pushButtonUpHor, SIGNAL(clicked()), this, SLOT(ButtonUpHor()));
    connect(ui->pushButtonDownHor, SIGNAL(clicked()), this, SLOT(ButtonDownHor()));
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

    int count = ui->lineEditCountV->text().toInt();
    if (count >= 0){
        ui->myGLWidget->setCountVer(count);
    }
    count = ui->lineEditCountH->text().toInt();
    if (count >= 0){
        ui->myGLWidget->setCountHor(count);
    }
}

void Window::setShowHor(bool show){
    ui->myGLWidget->setShowHor(show);
    if (show){
        ui->lineEditCountH->setEnabled(true);
        ui->pushButtonUpHor->setEnabled(true);
        ui->pushButtonDownHor->setEnabled(true);
    }else{
        ui->lineEditCountH->setEnabled(false);
        ui->pushButtonUpHor->setEnabled(false);
        ui->pushButtonDownHor->setEnabled(false);
    }
}

void Window::setShowVer(bool show){
    ui->myGLWidget->setShowVer(show);
    if (show){
        ui->lineEditCountV->setEnabled(true);
        ui->pushButtonUpVer->setEnabled(true);
        ui->pushButtonDownVer->setEnabled(true);
    }else{
        ui->lineEditCountV->setEnabled(false);
        ui->pushButtonUpVer->setEnabled(false);
        ui->pushButtonDownVer->setEnabled(false);
    }
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

void Window::setThickLinePoint(int thick){
    ui->myGLWidget->setThickLinePoint(thick);
}

void Window::ButtonUpHor(){
    int count = ui->lineEditCountH->text().toInt();
    count++;
    ui->lineEditCountH->setText(QString::number(count));
    ui->myGLWidget->setCountHor(count);
}

void Window::ButtonDownHor(){
    int count = ui->lineEditCountH->text().toInt();
    count--;
    if (count > 0){
        ui->lineEditCountH->setText(QString::number(count));
        ui->myGLWidget->setCountHor(count);
    }
}

void Window::ButtonUpVer(){
    int count = ui->lineEditCountV->text().toInt();
    count++;
    ui->lineEditCountV->setText(QString::number(count));
    ui->myGLWidget->setCountVer(count);
}

void Window::ButtonDownVer(){
    int count = ui->lineEditCountV->text().toInt();
    count--;
    if (count > 0){
        ui->lineEditCountV->setText(QString::number(count));
        ui->myGLWidget->setCountVer(count);
    }
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
    QString countHor, countVer;

    switch (item) {
    case 0://Ellipsoid
        par1 = "3"; par2 = "3"; par3 = "3";
        fU = "0"; bU = "6.283184";
        fV = "-1.570796"; bV = "1.570796";
        countHor = "20"; countVer = "10";
        a = true; b = true; c = true;
      break;
    case 1://Thor
        par1 = "2"; par2 = "1"; par3 = "0";
        fU = "0"; bU = "6.283184";
        fV = "-3.141592"; bV = "3.141592";
        countHor = "25"; countVer = "25";
        a = true; b = true; c = false;
      break;
    case 2://Hyperboloid
        par1 = "0.5"; par2 = "0.5"; par3 = "1";
        fU = "0"; bU = "6.283184";
        fV = "-2"; bV = "2";
        countHor = "25"; countVer = "10";
        a = true; b = true; c = true;
        break;
    case 3://Elliptical paraboloid
        par1 = "2"; par2 = "2"; par3 = "0";
        fU = "-1"; bU = "1";
        fV = "-1"; bV = "1";
        countHor = "10"; countVer = "10";
        a = true; b = true; c = false;
        break;
    case 4://Logarithmic spiral
        par1 = "1"; par2 = "1"; par3 = "0";
        fU = "0"; bU = "9.424777";
        fV = "-3.131592"; bV = "3.131592";
        countHor = "30"; countVer = "30";
        a = true; b = true; c = false;
        break;
    case 5://Pseudospere
        par1 = "2"; par2 = "0"; par3 = "0";
        fU = "-3"; bU = "3";
        fV = "-6.283184"; bV = "6.283184";
        countHor = "10"; countVer = "100";
        a = true; b = false; c = false;
        break;
    case 6://Spiral
        par1 = "3"; par2 = "3"; par3 = "0";
        fU = "-6.283185"; bU = "6.283185";
        fV = "-3.141592"; bV = "3.141592";
        countHor = "25"; countVer = "40";
        a = true; b = true; c = false;
        break;
    case 7://Dini's Surfase
        par1 = "10"; par2 = "1"; par3 = "0";
        fU = "0"; bU = "18.849555";
        fV = "0"; bV = "2";
        countHor = "150"; countVer = "10";
        a = true; b = true; c = false;
        break;
    default:
        par1 = "error"; par2 = "error"; par3 = "error";
        fU = "error"; bU = "error";
        fV = "error"; bV = "error";
        countHor = "error"; countVer = "error";
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
    ui->lineEditCountH->setText(countHor);
    ui->lineEditCountV->setText(countVer);

    ButtonSetRadius();
    ui->myGLWidget->setChooseFigure(item);
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
