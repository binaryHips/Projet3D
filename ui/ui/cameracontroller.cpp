#include "cameracontroller.h"

#include <QVector3D>
#include <QMouseEvent>
#include <QKeyEvent>
#include <QtMath>

#include <iostream>

void CameraController::orbitalRotate()
{
    float cy = qCos(qDegreesToRadians(yaw));
    float sy = qSin(qDegreesToRadians(yaw));
    float cp = qCos(qDegreesToRadians(pitch));
    float sp = qSin(qDegreesToRadians(pitch));

    forward = QVector3D(cy * cp, sp, sy * cp).normalized();

    QVector3D target = QVector3D(0.5f, 0.5f, 0.5f);

    float radius = (pos - target).length();
    if (radius < 0.001f) radius = 3.0f; 

    pos = target - forward * radius;

    QVector3D worldUp(0.0f, 1.0f, 0.0f);
    QVector3D right = QVector3D::crossProduct(forward, worldUp).normalized();
    up = QVector3D::crossProduct(right, forward).normalized();
}

void CameraController::fpsRotate()
{
    float cy = qCos(qDegreesToRadians(yaw));
    float sy = qSin(qDegreesToRadians(yaw));
    float cp = qCos(qDegreesToRadians(pitch));
    float sp = qSin(qDegreesToRadians(pitch));

    forward = QVector3D(cy * cp, sp, sy * cp).normalized();
    QVector3D worldUp(0.0f, 1.0f, 0.0f);
    QVector3D right = QVector3D::crossProduct(forward, worldUp).normalized();
    up = QVector3D::crossProduct(right, forward).normalized();
}

CameraController::CameraController(){

    pos = QVector3D(0.5f, 1.0f, 2.5f);

    yaw = -90.0f;
    pitch = 0.0f;

    forward = QVector3D(0.0f, 0.0f, -1.0f);
    up = QVector3D(0.0f, 1.0f, 0.0f);

    camControlType = ORBITAL;

    // Lock into the right angle for orbital view.
    orbitalRotate();

}

QVector3D CameraController::getPos(){
    return pos;
}

QVector3D CameraController::getForward(){
    return forward;
}

QVector3D CameraController::getUp(){
    return up;
}

void CameraController::updateCamera(float dt){
    
    if(camControlType == FPS)
    {

        float speed = 3.0f * dt;

        QVector3D right = QVector3D::crossProduct(forward, up);

        if (keyDown[Qt::Key_Z]) pos += forward * speed;
        if (keyDown[Qt::Key_S]) pos -= forward * speed;
        if (keyDown[Qt::Key_Q]) pos -= right * speed;
        if (keyDown[Qt::Key_D]) pos += right * speed;
        if (keyDown[Qt::Key_E]) pos += up * speed;
        if (keyDown[Qt::Key_A]) pos -= up * speed;

        qDebug() << pos ;
    }
}


void CameraController::onKeyPressed(int key){

    if (key <= 256){
        keyDown[key] = 1;
    }

}

void CameraController::onKeyUnpressed(int key){

    if (key <= 256){
        keyDown[key] = 0;
    }

}

// TODO : Convert this to orbital cam
void CameraController::onMouseMove(QMouseEvent *e, Qt::MouseButton button){

    if (firstMouse) {
        lastMouse = e->pos();
        firstMouse = false;
    }

    float dx = e->x() - lastMouse.x();
    float dy = lastMouse.y() - e->y();
    lastMouse = e->pos();

    float sensitivity = 0.1f;
    dx *= sensitivity;
    dy *= sensitivity;

    if (button == Qt::RightButton){

        yaw   += dx;
        pitch += dy;

        pitch = qBound(-89.0f, pitch, 89.0f);

        switch (camControlType) {
        case ORBITAL:
            orbitalRotate();
            break;
        case FPS:
            fpsRotate();
            break;

        default:
            orbitalRotate();
            break;
        }


    }
}


void CameraController::setControlType(controlTypes type)
{
    camControlType = type;
}
