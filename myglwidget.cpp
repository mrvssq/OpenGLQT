// myglwidget.cpp

#include <QtWidgets>
#include <QtOpenGL>

#include "myglwidget.h"

MyGLWidget::MyGLWidget(QWidget *parent)
    : QGLWidget(QGLFormat(QGL::SampleBuffers), parent)
{
    xRot = 0;
    yRot = 0;
    zRot = 0;
}

MyGLWidget::~MyGLWidget()
{
}

QSize MyGLWidget::minimumSizeHint() const
{
    return QSize(50, 50);
}

QSize MyGLWidget::sizeHint() const
{
    return QSize(800, 800);
}

static void qNormalizeAngle(int &angle)
{
    while (angle < 0)
        angle += 360 * 16;
    while (angle > 360)
        angle -= 360 * 16;
}

void MyGLWidget::setXRotation(int angle)
{
    qNormalizeAngle(angle);
    if (angle != xRot)
    {
        xRot = angle;
        updateGL();
    }
}

void MyGLWidget::setYRotation(int angle)
{
    qNormalizeAngle(angle);
    if (angle != yRot)
    {
        yRot = angle;
        updateGL();
    }
}

void MyGLWidget::setZRotation(int angle)
{
    qNormalizeAngle(angle);
    if (angle != zRot)
    {
        zRot = angle;
        updateGL();
    }
}

void MyGLWidget::initializeGL()
{
    qglClearColor(QColor(95, 169, 169));
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glShadeModel(GL_SMOOTH);
    glDisable(GL_LINE_STIPPLE);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    static GLfloat lightPosition[4] = { 0, 0, 10, 1.0 };
    glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);
}

void MyGLWidget::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glTranslatef(0.0, 0.0, -10.0);
    glRotatef(xRot / 1.0f, 1.0f, 0.0f, 0.0f);
    glRotatef(yRot / 1.0f, 0.0f, 1.0f, 0.0f);
    glRotatef(zRot / 1.0f, 0.0f, 0.0f, 1.0f);
    glScalef(xScale, yScale, zScale);
    draw();
}

void MyGLWidget::resizeGL(int width, int height)
{
    GLdouble ratio = width/static_cast<GLdouble>(height);
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    #ifdef QT_OPENGL_ES_11
        glOrthof(-2 * ratio, 2 * ratio, -2, 2, 1.0, 15.0);
    #else
        glOrtho(-2 * ratio, 2 * ratio, -2, 2, 1.0, 15.0);
    #endif
    glMatrixMode(GL_MODELVIEW);
}

void MyGLWidget::mousePressEvent(QMouseEvent *event)
{
    lastPos = event->pos();
}

void MyGLWidget::mouseMoveEvent(QMouseEvent *event)
{
    int dx = event->x() - lastPos.x();
    int dy = event->y() - lastPos.y();

    if (event->buttons() & Qt::LeftButton)
    {
        setXRotation(xRot + 8 * dy);
        setYRotation(yRot + 8 * dx);
    }
    else if (event->buttons() & Qt::RightButton)
    {
        setXRotation(xRot + 8 * dy);
        setZRotation(zRot + 8 * dx);
    }

    lastPos = event->pos();
}

void MyGLWidget::setColorWall(QColor color)
{
    colorWall = color;
    qglClearColor(color);
    updateGL();
}

void MyGLWidget::setRadius(double r1, double r2, double r3)
{
    argFun1 = r1;
    argFun2 = r2;
    argFun3 = r3;
    updateGL();
}

void MyGLWidget::setUV(double fromU, double beforeU, double fromV, double beforeV)
{
    uFrom = fromU;
    uBefore = beforeU;
    vFrom = fromV;
    vBefore = beforeV;
    updateGL();
}

void MyGLWidget::setCountVer(int verCount)
{
    vCount = verCount;
    updateGL();
}

void MyGLWidget::setCountHor(int horCount)
{
    hCount = horCount;
    updateGL();
}

void MyGLWidget::setThickLinePoint(int thick)
{
    float linethik = thick * 0.4f;
    thickLinesPoints = linethik;
    updateGL();
}

void MyGLWidget::setShowHor(bool show)
{
    showHor = show;
    updateGL();
}

void MyGLWidget::setShowVer(bool show)
{
    showVer = show;
    updateGL();
}

void MyGLWidget::plastScale()
{
    xScale+=0.1f;
    yScale+=0.1f;
    zScale+=0.1f;
    updateGL();
}

void MyGLWidget::minusScale()
{
    xScale-=0.1f;
    yScale-=0.1f;
    zScale-=0.1f;
    updateGL();
}

void MyGLWidget::rotation(bool side, int asix)
{
    int *asixChange = new int(1);
    int angle = 0;

    if (asix == 0)
        asixChange = &xRot;
    if (asix == 1)
        asixChange = &yRot;
    if (asix == 2)
        asixChange = &zRot;

    if (side) angle = *asixChange + 1; else angle = *asixChange - 1;

    qNormalizeAngle(angle);
    if (angle != *asixChange)
    {
        *asixChange = angle;
        updateGL();
    }
}

void MyGLWidget::setChooseFigure(int item)
{
    itemFigure = item;
    updateGL();
}

void MyGLWidget::getCoordinatesParametricFunc(double v, double u, double &x, double &y, double &z)
{
    switch ( itemFigure )
    {
    case 0: //Ellipsoid
        x = argFun1 * cos(v) * cos(u);
        y = argFun3 * cos(v) * sin(u);
        z = argFun2 * sin(v);
      break;
    case 1://Thor
        x = (argFun1 + argFun2 * cos(v)) * cos(u);
        y = (argFun1 + argFun2 * cos(v)) * sin(u);
        z = argFun2 * sin(v);
      break;
    case 2://Hyperboloid
        x = argFun1 * cosh(v) * cos(u);
        y = argFun2 * cosh(v) * sin(u);
        z = argFun3 * sinh(v);
      break;
    case 3://Elliptical paraboloid
        x = argFun1 * u;
        y = argFun2 * v;
        z = (u* u + v * v);
      break;
    case 4://Logarithmic spiral
        x = u * cos(u) * (cos(v) + argFun1) * 1/5;
        y = u * sin(u) * (cos(v) + argFun2) * 1/5;
        z = u * sin(v) * 1/5;
      break;
    case 5://Pseudospere
        x = argFun1 * sin(u) * cos(v);
        y = argFun1 * sin(u) * sin(v);
        z = argFun1 * (log(tan(u/2)) + cos(u));
      break;
    case 6://Spiral
        x = cos(u) * (cos(v) + argFun1) * 1/2;
        y = sin(u) * (cos(v) + argFun2) * 1/2;
        z = (sin(v) + u) * 1/2;
      break;
    case 7://Dini's Surfase
        x = argFun1 * cos(u) * sin(v) * 1/5;
        y = argFun1 * sin(u) * sin(v) * 1/5;
        z = (argFun1 * (cos(v) + log(tan(v / 2))) + argFun2 * u) * 1/5;
      break;
    default:
      break;
    }
}

void MyGLWidget::drowFun()
{
    double stepPoints = 0.1;
    if (showVer)
    {
        double stepVer = fabs(vFrom - vBefore) / (vCount);
        for (double u = uFrom; u <= uBefore; u+=stepVer)//кол-во колец Вертикаль
        {
            glBegin(GL_LINE_STRIP);
            for(double v = vFrom; v <= vBefore + stepPoints; v+=stepPoints)//расстояние между точками
            {
                double x=0, y=0, z=0;
                getCoordinatesParametricFunc(v, u, x, y, z);
                glVertex3d(x, y, z);
            }
            glEnd();
        }
    }

    if (showHor)
    {
        double stepHor = fabs(uFrom - uBefore) / (hCount + 1);
        for (double v = vFrom; v <= vBefore; v+=stepHor) //кол-во колец Горизонталь
        {
            glBegin(GL_LINE_STRIP);
            for(double u = uFrom; u <= uBefore + stepPoints; u+=stepPoints) //расстояние между точками
            {
                double x=0, y=0, z=0;
                getCoordinatesParametricFunc(v, u, x, y, z);
                glVertex3d(x, y, z);
            }
            glEnd();
        }
    }
}

void MyGLWidget::draw()
{
    glLineWidth(thickLinesPoints);
    drowFun();
}
