// myglwidget.h

#ifndef MYGLWIDGET_H
#define MYGLWIDGET_H

#include <QGLWidget>
#include <QtMath>

class MyGLWidget : public QGLWidget
{
    Q_OBJECT
public:
    explicit MyGLWidget(QWidget *parent = 0);
    ~MyGLWidget();
    void setRadius(double r1, double r2, double r3);
    void setUV(double FromU, double BeforeU, double FromV, double BeforeV);

    QColor colorWall = QColor(95, 169, 169);
    void setColorWall(QColor color);
    void setThickLinePoint(int thick);
    void setChooseFigure(int item);

    void setCountVer(int verCount);
    void setCountHor(int horCount);
    void setShowVer(bool show);
    void setShowHor(bool show);

    void plastScale();
    void minusScale();
    void rotation(bool side, int asix);

protected:
    void initializeGL();
    void paintGL();
    void resizeGL(int width, int height);

    QSize minimumSizeHint() const;
    QSize sizeHint() const;
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);

public slots:
    void setXRotation(int angle);
    void setYRotation(int angle);
    void setZRotation(int angle);

private:
    void getCoordinatesParametricFunc(double v, double u, double &x, double &y, double &z);
    void drowFun();

    GLfloat xScale = 0.5;
    GLfloat yScale = 0.5;
    GLfloat zScale = 0.5;
    GLfloat thickLinesPoints = 1.0;
    QPoint lastPos;

    double argFun1 = 3.0;
    double argFun2 = 3.0;
    double argFun3 = 3.0;

    int xRot;
    int yRot;
    int zRot;

    double vFrom = -M_PI_2;
    double vBefore = M_PI_2;
    double uFrom = 0;
    double uBefore = 2*M_PI;

    bool showHor = true;
    bool showVer = true;
    int vCount = 10;
    int hCount = 20;
    int itemFigure=0;
};

#endif // MYGLWIDGET_H

