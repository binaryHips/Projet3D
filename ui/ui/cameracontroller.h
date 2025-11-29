#ifndef CAMERACONTROLLER_H
#define CAMERACONTROLLER_H

#include <QMatrix4x4>
#include <QVector3D>
#include <QPoint>
#include <QKeyEvent>
#include <QMouseEvent>

enum controlTypes
{
    ORBITAL,
    FPS
};

class CameraController
{
public:
    CameraController();

    void updateCamera(float dt);
    void onKeyPressed(int key);
    void onKeyUnpressed(int key);
    void onMouseMove(QMouseEvent *e, Qt::MouseButton button);

    QVector3D getPos();
    QVector3D getForward();
    QVector3D getUp();

    controlTypes camControlType; 

public slots:
    void setControlType(controlTypes type);

private:
    QVector3D pos ;
    QVector3D forward;
    QVector3D up;
    float yaw;
    float pitch;

    bool keyDown[1024] = {};
    Qt::MouseButton button;

    // input
    QPoint lastMouse;
    bool firstMouse = true;

    // rotate hlepers 
    void orbitalRotate();
    void fpsRotate();
};

#endif // CAMERACONTROLLER_H
