#ifndef INPUTVIEW_H
#define INPUTVIEW_H

#include <QObject>
#include <QColor>
#include <QtOpenGL>
#include <QGLWidget>

class InputView : public QGLWidget{
    
    Q_OBJECT
    
    public:
        InputView(QWidget *parent = 0);
        ~InputView();
        QSize minimumSizeHint() const;
        QSize sizeHint() const;
        void drawSpheres();
        void addSphere(float*, float*);
        void clearSpheres();
        
    public slots:
        void setXRotation(int angle);
        void setYRotation(int angle);
        void setZRotation(int angle);
    
    signals:
        void xRotationChanged(int angle);
        void yRotationChanged(int angle);
        void zRotationChanged(int angle);
    
    protected:
        void initializeGL();
        void paintGL();
        void resizeGL(int width, int height);
        void mousePressEvent(QMouseEvent *event);
        void mouseMoveEvent(QMouseEvent *event);
        void keyPressEvent(QKeyEvent *event);
    
    private:
        int xRot;
        int yRot;
        int zRot;
        float xTrans;
        float yTrans;
        float zTrans;
        float zoomFactor;
        QPoint lastPos;
        QColor qtWhite;
        GLUquadricObj *quadratic;
        QVector<QVector3D> colors;
        QVector<QVector3D> coords;
};

#endif
