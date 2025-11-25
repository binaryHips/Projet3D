#ifndef CAMERACONTROLLER_H
#define CAMERACONTROLLER_H

#include <QMatrix4x4>
#include <QVector3D>
#include <QPoint>
#include <QKeyEvent>
#include <QMouseEvent>

class CameraController
{
public:
    CameraController();

    void updateCamera(float dt);
    void onKeyPressed(int key);
    void onKeyUnpressed(int key);
    void onMouseMove(QMouseEvent *e);

    QVector3D getPos();
    QVector3D getForward();
    QVector3D getUp();

private:
    QVector3D pos = {0, 0, 3};
    QVector3D forward = {0, 0, -1};
    QVector3D up = {0, 1, 0};
    float yaw = -90.0f;
    float pitch = 0.0f;

    bool keyDown[1024] = {};

    // input
    QPoint lastMouse;
    bool firstMouse = true;
};

#endif // CAMERACONTROLLER_H
