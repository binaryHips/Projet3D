#include "glwidget.h"
#include <QKeyEvent>
#include <QMouseEvent>
#include <QtMath>
#include <QTimer>
#include "mainwindow.h"
#include "backend.h"
#include <chrono>

inline size_t currentTime()
{
    std::chrono::time_point<std::chrono::system_clock> timestamp =
        std::chrono::system_clock::now();

    const auto ms = std::chrono::duration_cast<std::chrono::microseconds>
                    (timestamp.time_since_epoch()).count();

    return ms; // Convert to seconds
}


GLWidget::GLWidget(QWidget *parent)
    : QOpenGLWidget(parent)
{
    setMouseTracking(true);
    setFocusPolicy(Qt::StrongFocus);

    connect(&timer, &QTimer::timeout, this, QOverload<>::of(&GLWidget::update));
    timer.start(0);

    lastTime = currentTime();

    // backend->context.particleSystem.spawn(512);
}

void GLWidget::setControlType(controlTypes type)
{
    cam.setControlType(type);
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

    backend = static_cast<MainWindow*>(window())->backend;

    for(Mesh *m:meshes){
        m->setGlFunctions(this);
        m->setShader(":/vshader.glsl" , ":/fshader.glsl");
        glDisable(GL_CULL_FACE);
    }

    glEnable(GL_DEPTH_TEST);
}

void GLWidget::setMesh(Mesh *m, int index)
{


    Mesh *old = meshes[index];
    if (old) {
        delete old;
    }

    meshes[index] = m;

    m->setGlFunctions(this);
    m->setShader(":/vshader.glsl" , ":/fshader.glsl");
    m->updatePlaneHeightmap(backend->context);

}


void GLWidget::resizeGL(int w, int h)
{
    projection.setToIdentity();
    projection.perspective(60.0f, float(w)/float(h), 0.1f, 200.0f);
}

// TODO : make this just a public function not a slot
void GLWidget::updateGLSlot()
{
    // FIXME : Unstable blue thing 
    backend->context.update(0);
    meshes[0]->updatePlaneHeightmap(backend->context);
}

void GLWidget::paintGL()
{   
    size_t ct = currentTime();
    float dt = (ct - lastTime) * 0.000001;
    lastTime = ct;

//    qDebug() << dt * simSpeed;

    if(backend->simulating){

        backend->context.update(dt * simSpeed);
        meshes[0]->updatePlaneHeightmap(backend->context);

        // TODO : make this less ugly good god
        QPixmap map = backend->saveImageFromMap(MAP_LAYERS::BEDROCK);
        backend->setHeightmap(map , MAP_LAYERS::BEDROCK);
        map = backend->saveImageFromMap(MAP_LAYERS::STONE);
        backend->setHeightmap(map , MAP_LAYERS::STONE);
        map = backend->saveImageFromMap(MAP_LAYERS::SAND);
        backend->setHeightmap(map , MAP_LAYERS::SAND);
        map = backend->saveImageFromMap(MAP_LAYERS::WATER);
        backend->setHeightmap(map , MAP_LAYERS::WATER);
        
        cam.updateCamera(dt);
    }

    glClearColor(0.5, 0.5, 0.7, 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    QMatrix4x4 view;
    view.lookAt(cam.getPos(), cam.getPos() + cam.getForward(), cam.getUp());

    QMatrix4x4 VP = projection * view;

    for (Mesh *m : meshes)
    {
        m->renderForward(VP, cam.getForward(), QMatrix4x4());
    }
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

void GLWidget::mousePressEvent(QMouseEvent *event){
    button = event->button();
}
void GLWidget::mouseReleaseEvent(QMouseEvent *event){
    button = Qt::NoButton;
}

void GLWidget::mouseMoveEvent(QMouseEvent *e)
{
    cam.onMouseMove(e,button);
    update();
}

void GLWidget::wheelEvent(QWheelEvent *e)
{
    cam.onMouseScroll(e);
    update();
}
