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

    void setDisplayType(int modeType);
    void setColorWall(int r, int g, int b);
    void setThickLinePoint(float thick);
    void setChooseFigure(int item);

    void addCountLongitude(double longCount);
    void addCountLatitude(double latCount);

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
    //void drawPiramid();
    //void drawEllipsoid();
    void draw();
    double* getCoordinatesParametricFunc(double param[], double v, double u);
    void drowFun(double argsFun[], double uGap[], double vGap[]);

    GLfloat xScale = 0.5;
    GLfloat yScale = 0.5;
    GLfloat zScale = 0.5;

    GLfloat thickLinesPoints = 1.0;

    //double longitude = 30.0;
    //double latitude = 30.0;
    double argFun1 = 3.0;
    double argFun2 = 3.0;
    double argFun3 = 3.0;

    int xRot;
    int yRot;
    int zRot;

    double vFrom = -M_PI_2;
    double vBefore = M_PI_2;
    double vStep = 0.2;
    double uFrom = 0;
    double uBefore = 2*M_PI;
    double uStep = 0.2;

    int itemFigure=0;

    QPoint lastPos;
};

#endif // MYGLWIDGET_H

