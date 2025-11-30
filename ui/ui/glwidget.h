#ifndef GLWIDGET_H
#define GLWIDGET_H

#pragma once
#include <QOpenGLWidget>
#include <QOpenGLExtraFunctions>
#include <QMatrix4x4>
#include <QTimer>
#include <QKeyEvent>
#include <QMouseEvent>
#include <QElapsedTimer>

#include "mesh.h"
#include "cameracontroller.h"

class GLWidget : public QOpenGLWidget, public QOpenGLExtraFunctions
{
    Q_OBJECT

public:
    GLWidget(QWidget *parent = nullptr);
    ~GLWidget();

    Mesh *addMesh(Mesh *m);
    void setMesh(Mesh *m, int index=0);

public slots:
    void setControlType(controlTypes type);

signals :
    void mouseButtonClicked(Qt::MouseButton button);

protected:
    void initializeGL() override;
    void resizeGL(int w, int h) override;
    void paintGL() override;

    void keyPressEvent(QKeyEvent *e) override;
    void keyReleaseEvent(QKeyEvent *e) override;

    void mouseMoveEvent(QMouseEvent *e) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;

private:
    QVector<Mesh*> planes;
    QVector<Mesh*> meshes;

    // camera
    CameraController cam;
    QMatrix4x4 projection;

    // input
    Qt::MouseButton button;

    // time
    size_t lastTime;
    QTimer timer;

    QElapsedTimer fps_timer;
    void updateCamera(float dt);
};


#endif
