// window.h

#ifndef WINDOW_H
#define WINDOW_H

#include <QWidget>
#include <QSlider>
#include <QTimer>
#include <QTime>

namespace Ui {
class Window;
}

class Window : public QWidget
{
    Q_OBJECT

private slots:
    void ButtonsetColor();
    void ButtonPlast();
    void ButtonMinus();
    void ButtonSetRadius();
    void ButtonStopTimerRotate();
    void ButtonUpVer();
    void ButtonDownVer();
    void ButtonUpHor();
    void ButtonDownHor();
    void setThickLinePoint(int thick);
    void setShowHor(bool show);
    void setShowVer(bool show);

    void setTimerSpeed(int speed);
    void slotTimerRotate();

    void setTChooseFigure(int item);

    void setParam1(int a);
    void setParam2(int b);
    void setParam3(int c);

    void scrollXRotation(int angle);
    void scrollYRotation(int angle);
    void scrollZRotation(int angle);

    void resizeEvent(QResizeEvent *event);

public:
    explicit Window(QWidget *parent = 0);
    ~Window();

protected:
    void keyPressEvent(QKeyEvent *event);

private:
    Ui::Window *ui;
    QTimer *timerRotate;
    bool sideRotate = true;
};

#endif // WINDOW_H
