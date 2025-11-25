#include "glwidget.h"
#include <QKeyEvent>
#include <QMouseEvent>
#include <QtMath>

GLWidget::GLWidget(QWidget *parent)
    : QOpenGLWidget(parent)
{
    setMouseTracking(true);
    setFocusPolicy(Qt::StrongFocus);

    connect(&timer, &QTimer::timeout, this, QOverload<>::of(&GLWidget::update));
    timer.start(16);
}

GLWidget::~GLWidget()
{
}

Mesh* GLWidget::addMesh(Mesh *m)
{
    meshes.push_back(m);
    return m;
}

void GLWidget::initializeGL()
{
    initializeOpenGLFunctions();

    for(Mesh *m:meshes){
        m->setGlFunctions(this);
        m->setShader("/home/drew/Desktop/3DProject/Projet3D/ui/ui/vshader.glsl" , "/home/drew/Desktop/3DProject/Projet3D/ui/ui/fshader.glsl");
        glDisable(GL_CULL_FACE);
        qDebug() << "Shader vertexes : " << m->vertices.count();
    }

    glEnable(GL_DEPTH_TEST);
}

void GLWidget::resizeGL(int w, int h)
{
    projection.setToIdentity();
    projection.perspective(60.0f, float(w)/float(h), 0.1f, 200.0f);
}

void GLWidget::paintGL()
{
    float dt = 0.016f;
    cam.updateCamera(dt);
    qDebug() << "Camera pos:" << cam.getPos() << "forward:" << cam.getForward();
    // qDebug() << "Cam pos:" << cam.getPos();


    glClearColor(0.1, 0.1, 0.15, 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    QMatrix4x4 view;
    view.lookAt(cam.getPos(), cam.getPos() + cam.getForward(), cam.getUp());

    QMatrix4x4 VP = projection * view;

    for (Mesh *m : meshes)
        m->renderForward(VP, cam.getForward(), QMatrix4x4());
}

void GLWidget::keyPressEvent(QKeyEvent *e)
{
    cam.onKeyPressed(e->key());
    update();
}

void GLWidget::keyReleaseEvent(QKeyEvent *e)
{
    cam.onKeyUnpressed(e->key());
}

void GLWidget::mouseMoveEvent(QMouseEvent *e)
{
    cam.onMouseMove(e);
    update();
}

