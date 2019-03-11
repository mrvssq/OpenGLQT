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

    void setColorWall(int r, int g, int b);
    void setThickLinePoint(int thick);
    void setChooseFigure(int item);

    void setCountVer(int verCount);
    void setCountHor(int horCount);
    void setShowVer(bool show);
    void setShowHor(bool show);

    void plastScale();
    void minusScale();
    void rotation(bool side);

protected:
    void initializeGL();
    void paintGL();
    void resizeGL(int width, int height);

    QSize minimumSizeHint() const;
    QSize sizeHint() const;
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);

public slots:
    // slots for xyz-rotation slider
    void setXRotation(int angle);
    void setYRotation(int angle);
    void setZRotation(int angle);

signals:
    // signaling rotation from mouse movement
    void xRotationChanged(int angle);
    void yRotationChanged(int angle);
    void zRotationChanged(int angle);


private:
    void draw();
    double* getCoordinatesParametricFunc(double param[], double v, double u);
    void drowFun(double argsFun[], double uGap[], double vGap[]);

    GLfloat xScale = 0.5;
    GLfloat yScale = 0.5;
    GLfloat zScale = 0.5;

    GLfloat thickLinesPoints = 1.0;

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

    QPoint lastPos;
};

#endif // MYGLWIDGET_H

