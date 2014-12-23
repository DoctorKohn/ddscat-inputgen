#include <QtGui>
#include <QtOpenGL>
#include <iostream>

#include <math.h>
#include <GL/gl.h>
#include <GL/glu.h>

#include "inputview.h"

#ifndef GL_MULTISAMPLE
#define GL_MULTISAMPLE  0x809D
#endif

InputView::InputView(QWidget *parent)
    : QGLWidget(QGLFormat(QGL::SampleBuffers), parent){
        
    xRot = 0;
    yRot = 0;
    zRot = 0;
    xTrans = 0.0f;
    yTrans = 0.0f;
    zTrans = -10.0f;
    zoomFactor = 1.0;
    qtWhite = QColor::fromRgbF(1.0, 1.0, 1.0, 1.0);
}

InputView::~InputView(){
    // Destructor
}

QSize InputView::minimumSizeHint() const {
    
    return QSize(50, 50);
}
    
QSize InputView::sizeHint() const {
    
    return QSize(600, 600);
}

static void qNormalizeAngle(int &angle){
    
    while (angle < 0)
        angle += 360 * 16;
    while (angle > 360 * 16)
        angle -= 360 * 16;
}

void InputView::setXRotation(int angle){
    
    qNormalizeAngle(angle);
    if (angle != xRot) {
        xRot = angle;
        emit xRotationChanged(angle);
        updateGL();
    }
}

void InputView::setYRotation(int angle){
    
    qNormalizeAngle(angle);
    if (angle != yRot) {
        yRot = angle;
        emit yRotationChanged(angle);
        updateGL();
    }
}

void InputView::setZRotation(int angle){
    
    qNormalizeAngle(angle);
    if (angle != zRot) {
        zRot = angle;
        emit zRotationChanged(angle);
        updateGL();
    }
}

void InputView::initializeGL(){
    
    qglClearColor(qtWhite); // Sets background to white

    glEnable(GL_DEPTH_TEST); // Enables depth for mouse location
    glEnable(GL_CULL_FACE); // Disables rendering of faces not visible by the camera
    glShadeModel(GL_SMOOTH); // Smooth vertex model
    glEnable(GL_LIGHTING); // Enables lighting
    glEnable(GL_LIGHT0); // Turns on light 0
    glEnable(GL_MULTISAMPLE); // Enables multisample anti-alising for smoother edges
    static GLfloat lightPosition[4] = { 0.5, 5.0, 7.0, 0.0 }; // Coordinates for the light source
    glLightfv(GL_LIGHT0, GL_POSITION, lightPosition); // Sets the postion of the light source
    glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE); // These two lines allow coloring due to lighting
    glEnable(GL_COLOR_MATERIAL); // Paired with top line to allow colors with lighting
    quadratic = gluNewQuadric();			// Create A Pointer To The Quadric Object ( NEW )
	gluQuadricNormals(quadratic, GLU_SMOOTH);	// Create Smooth Normals ( NEW )
	gluQuadricTexture(quadratic, GL_TRUE);		// Create Texture Coords ( NEW )
    setFocus();
}

void InputView::paintGL(){
    
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    glTranslatef(xTrans, yTrans, zTrans);
    glRotatef(xRot / 16.0f, 1.0f, 0.0f, 0.0f);
    glRotatef(yRot / 16.0f, 0.0f, 1.0f, 0.0f);
    glRotatef(zRot / 16.0f, 0.0f, 0.0f, 1.0f);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(10.0*zoomFactor, 1, 5, 100);
    glMatrixMode(GL_MODELVIEW);
    drawSpheres();
}

void InputView::drawSpheres(){
    
    for (int i = 0; i < coords.size(); i++){
        glColor3f(colors[i].x(), colors[i].y(), colors[i].z());
        glTranslatef(coords[i].x(), coords[i].y(), coords[i].z());
        gluSphere(quadratic, .02, 6, 6);
        glTranslatef(-coords[i].x(), -coords[i].y(), -coords[i].z());
    }
}    

void InputView::addSphere(float coord[3], float color[3]){
    
    coords << QVector3D(coord[0], coord[1], coord[2]);
    colors << QVector3D(color[0], color[1], color[2]);  
}

void InputView::clearSpheres(){
    
    coords.clear();
    colors.clear();
}

void InputView::resizeGL(int width, int height){
    
    int side = qMin(width, height);
    glViewport(0, 0, side, side);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(10.0*zoomFactor, (float)width/(float)height, 5, 100);
    glMatrixMode(GL_MODELVIEW);
}

void InputView::mousePressEvent(QMouseEvent *event){
    
    lastPos = event->pos();
    setFocus();
}

void InputView::keyPressEvent(QKeyEvent *event){
    
    if(event->key() == Qt::Key_Up){
        zoomFactor = zoomFactor - 0.1;
    } else if (event->key() == Qt::Key_Down){
        zoomFactor = zoomFactor + 0.1;
    }
    updateGL();
}

void InputView::mouseMoveEvent(QMouseEvent *event){
    
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
