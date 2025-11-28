#include "cameracontroller.h"

#include <QVector3D>
#include <QMouseEvent>
#include <QKeyEvent>
#include <QtMath>

#include <iostream>



CameraController::CameraController(){

    pos = QVector3D(1.0f, 0.0f, 3.0f);

    yaw = -90.0f;
    pitch = 0.0f;

    forward = QVector3D(0.0f, 0.0f, -1.0f);
    up = QVector3D(0.0f, 1.0f, 0.0f);

    button = Qt::NoButton;

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
    float speed = 3.0f * dt;

    QVector3D right = QVector3D::crossProduct(forward, up);

    if (keyDown[Qt::Key_Z]) pos += forward * speed;
    if (keyDown[Qt::Key_S]) pos -= forward * speed;
    if (keyDown[Qt::Key_Q]) pos -= right * speed;
    if (keyDown[Qt::Key_D]) pos += right * speed;
    if (keyDown[Qt::Key_E]) pos += up * speed;
    if (keyDown[Qt::Key_A]) pos -= up * speed;
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

        float cy = qCos(qDegreesToRadians(yaw));
        float sy = qSin(qDegreesToRadians(yaw));
        float cp = qCos(qDegreesToRadians(pitch));
        float sp = qSin(qDegreesToRadians(pitch));

        forward = QVector3D(cy * cp, sp, sy * cp).normalized();
        QVector3D worldUp(0.0f, 1.0f, 0.0f);
        QVector3D right = QVector3D::crossProduct(forward, worldUp).normalized();
        up = QVector3D::crossProduct(right, forward).normalized();
    }
}
