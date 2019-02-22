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

MyGLWidget::~MyGLWidget(){
}

QSize MyGLWidget::minimumSizeHint() const{
    return QSize(50, 50);
}

QSize MyGLWidget::sizeHint() const{
    return QSize(800, 800);
}

static void qNormalizeAngle(int &angle){
    while (angle < 0)
        angle += 360 * 16;
    while (angle > 360)
        angle -= 360 * 16;
}

void MyGLWidget::setXRotation(int angle){
    qNormalizeAngle(angle);
    if (angle != xRot) {
        xRot = angle;
        emit xRotationChanged(angle);
        updateGL();
    }
}

void MyGLWidget::setYRotation(int angle){
    qNormalizeAngle(angle);
    if (angle != yRot) {
        yRot = angle;
        emit yRotationChanged(angle);
        updateGL();
    }
}

void MyGLWidget::setZRotation(int angle){
    qNormalizeAngle(angle);
    if (angle != zRot) {
        zRot = angle;
        emit zRotationChanged(angle);
        updateGL();
    }
}

void MyGLWidget::initializeGL(){
    qglClearColor(QColor(95,169,169,0));

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glShadeModel(GL_SMOOTH);
    glDisable(GL_LINE_STIPPLE);

    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    static GLfloat lightPosition[4] = { 0, 0, 10, 1.0 };
    glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);
}

void MyGLWidget::paintGL(){
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    glTranslatef(0.0, 0.0, -10.0);
    glRotatef(xRot / 1.0f, 1.0f, 0.0f, 0.0f);
    glRotatef(yRot / 1.0f, 0.0f, 1.0f, 0.0f);
    glRotatef(zRot / 1.0f, 0.0f, 0.0f, 1.0f);
    glScalef(xScale, yScale, zScale);

    draw();
    //drawPiramid();
}

void MyGLWidget::resizeGL(int width, int height){
    int side = qMin(width, height);
    glViewport((width - side) / 2, (height - side) / 2, side, side);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
#ifdef QT_OPENGL_ES_1
    glOrthof(-2, +2, -2, +2, 1.0, 15.0);
#else
    glOrtho(-2, +2, -2, +2, 1.0, 15.0);
#endif
    glMatrixMode(GL_MODELVIEW);
}

void MyGLWidget::mousePressEvent(QMouseEvent *event){
    lastPos = event->pos();
}

void MyGLWidget::mouseMoveEvent(QMouseEvent *event){
    int dx = event->x() - lastPos.x();
    int dy = event->y() - lastPos.y();

    if (event->buttons() & Qt::LeftButton) {
        setXRotation(xRot + 8 * dy);
        setYRotation(yRot + 8 * dx);
    } else if (event->buttons() & Qt::RightButton) {
        setXRotation(xRot + 8 * dy);
        setZRotation(zRot + 8 * dx);
    }

    lastPos = event->pos();
}

void MyGLWidget::setColorWall(int r, int g, int b){
    qglClearColor(QColor(r,g,b,0));
    updateGL();
}

void MyGLWidget::setRadius(double r1, double r2, double r3){
    argFun1 = r1;
    argFun2 = r2;
    argFun3 = r3;
    updateGL();
}

void MyGLWidget::setUV(double FromU, double BeforeU, double FromV, double BeforeV){
    uFrom = FromU;
    uBefore = BeforeU;
    vFrom = FromV;
    vBefore = BeforeV;
    updateGL();
}

void MyGLWidget::addCountLongitude(double longCount){
    if (longCount > 0){
        vStep -= longCount;
    }
    else {
        vStep -= longCount;
    }
    updateGL();
}

void MyGLWidget::addCountLatitude(double latCount){
    if (latCount > 0){
        uStep -= latCount;
    }
    else {
        uStep -= latCount;
    }
    updateGL();
}

void MyGLWidget::setDisplayType(int modeType){

    switch (modeType) {
    case 0:
        glDisable(GL_LINE_STIPPLE);
        vStep = 0.2;
        uStep = 0.2;
        thickLinesPoints = 1;
      break;
    case 1:
        glEnable(GL_LINE_STIPPLE);
        glLineStipple(1, 0x0101);
        vStep = 0.2;
        uStep = 0.2;
        thickLinesPoints = 1;
      break;
    case 2:
        glDisable(GL_LINE_STIPPLE);
        vStep = 0.01;
        uStep = 0.01;
        thickLinesPoints = 6;
      break;
    default:
        break;
    }

    updateGL();
}

void MyGLWidget::setThickLinePoint(float thick){
    thickLinesPoints = thick;
    updateGL();
}

void MyGLWidget::plastScale(){
    xScale+=0.1f;
    yScale+=0.1f;
    zScale+=0.1f;
    updateGL();
}

void MyGLWidget::minusScale(){
    xScale-=0.1f;
    yScale-=0.1f;
    zScale-=0.1f;
    updateGL();
}

void MyGLWidget::rotation(bool side){

    int angle;

    if (side) angle = zRot + 1; else angle = zRot - 1;

    qNormalizeAngle(angle);
    if (angle != zRot) {
        zRot = angle;
        emit zRotationChanged(angle);
        updateGL();
    }
}

void MyGLWidget::setChooseFigure(int item){
    itemFigure = item;
    updateGL();
};

double* MyGLWidget::getCoordinatesParametricFunc(double param[], double v, double u){
    double* xyzReturn = new double[3];

    switch ( itemFigure ) {
    case 0: //Ellipsoid
        xyzReturn[0] = param[0] * cos(v) * cos(u);
        xyzReturn[1] = param[2] * cos(v) * sin(u);
        xyzReturn[2] = param[1] * sin(v);
      break;
    case 1://Thor
        xyzReturn[0] = (param[0] + param[1] * cos(v)) * cos(u);
        xyzReturn[1] = (param[0] + param[1] * cos(v)) * sin(u);
        xyzReturn[2] = param[1] * sin(v);
      break;
    case 2://Hyperboloid
        xyzReturn[0] = param[0] * cosh(v) * cos(u);
        xyzReturn[1] = param[1] * cosh(v) * sin(u);
        xyzReturn[2] = param[2] * sinh(v);
      break;
    case 3://Elliptical paraboloid
        xyzReturn[0] = param[0] * u;
        xyzReturn[1] = param[1] * v;
        xyzReturn[2] = (u* u + v * v);
      break;
    case 4://Logarithmic spiral
        xyzReturn[0] = u * cos(u) * (cos(v) + param[0]) * 1/5;
        xyzReturn[1] = u * sin(u) * (cos(v) + param[1]) * 1/5;
        xyzReturn[2] = u * sin(v) * 1/5;
      break;
    case 5://Pseudospere
        xyzReturn[0] = param[0] * sin(u) * cos(v);
        xyzReturn[1] = param[0] * sin(u) * sin(v);
        xyzReturn[2] = param[0] * (log(tan(u/2)) + cos(u));
      break;
    case 6://Spiral
        xyzReturn[0] = cos(u) * (cos(v) + param[0]) * 1/2;
        xyzReturn[1] = sin(u) * (cos(v) + param[1]) * 1/2;
        xyzReturn[2] = (sin(v) + u) * 1/2;
      break;
    case 7://Dini's Surfase
        xyzReturn[0] = param[0] * cos(u) * sin(v) * 1/5;
        xyzReturn[1] = param[0] * sin(u) * sin(v) * 1/5;
        xyzReturn[2] = (param[0] * (cos(v) + log(tan(v / 2))) + param[1] * u) * 1/5;
      break;
    default:

      break;
    }

    return (xyzReturn);
}

void MyGLWidget::drowFun(double argsFun[], double uGap[], double vGap[]){
    double *xyzFun = new double[3];

    for (double u = uGap[0]; u <= uGap[1]; u+=uGap[2]){ //кол-во колец
        glBegin(GL_LINE_STRIP);
        for(double v = vGap[0]; v <= vGap[1]; v+=0.01) //расстояние между точками
        {
          xyzFun = getCoordinatesParametricFunc(argsFun, v, u);

          glVertex3d(xyzFun[0],
                    xyzFun[1],
                    xyzFun[2]);
          delete[] xyzFun;
        }
        glEnd();
    }

    for (double v = vGap[0]; v <= vGap[1]; v+=vGap[2]){ //кол-во колец
        glBegin(GL_LINE_STRIP);
        for(double u = uGap[0]; u <= uGap[1]; u+=0.01) //расстояние между точками
        {
          xyzFun = getCoordinatesParametricFunc(argsFun, v, u);

          glVertex3d(xyzFun[0],
                    xyzFun[1],
                    xyzFun[2]);
          delete[] xyzFun;
        }
        glEnd();
    }

}

void MyGLWidget::draw(){
    double argsFun[3]={argFun1, argFun2, argFun3};
    double vGap[3]={vFrom, vBefore, vStep};
    double uGap[3]={uFrom, uBefore, uStep};

    glLineWidth(thickLinesPoints);

    drowFun(argsFun, uGap, vGap);
}
