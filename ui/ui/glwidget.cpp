#include "glwidget.h"
#include <QKeyEvent>
#include <QMouseEvent>
#include <QtMath>
#include <QTimer>
#include <QPainter>
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
    }

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    glDisable(GL_CULL_FACE);
}

Mesh* GLWidget::getMesh(int index)
{
    return meshes[index];
}

void GLWidget::setMesh(Mesh *m, int index)
{


    Mesh *old = meshes[index];
    if (old) {
        delete old;
    }

    meshes[index] = m;
    pendingSetMeshUpdate = true;
}

void GLWidget::resizeGL(int w, int h)
{
    projection.setToIdentity();
    projection.perspective(60.0f, float(w)/float(h), 0.1f, 200.0f);
}

// TODO : make this just a public function not a slot
void GLWidget::updateGLSlot()
{
    pendingHeightmapUpdate = true;
}

void GLWidget::paintGL()
{   
    size_t ct = currentTime();
    float dt = (ct - lastTime) * 0.000001;
    lastTime = ct;

    float fixedDt = 0.01;

    // FPS calculation
    frameCount++;
    fpsAccumulator += dt;
    if (fpsAccumulator >= fpsUpdateInterval) {
        fps = frameCount / fpsAccumulator;
        frameCount = 0;
        fpsAccumulator = 0.0f;
    }

    // Used when we set the plane by hand
    if (pendingSetMeshUpdate)
    {
        pendingSetMeshUpdate = false;
        meshes[0]->setGlFunctions(this);
        meshes[0]->setShader(":/vshader.glsl" , ":/fshader.glsl");
        meshes[0]->updatePlaneHeightmap(backend->context);
    }

    // Used when we modify/load one of the maps
    if (pendingHeightmapUpdate) {
        pendingHeightmapUpdate = false;
        //backend->context.update(0);
        meshes[0]->updatePlaneHeightmap(backend->context);
    }

    // Used when simulation is running
    if(backend->simulating){

        backend->context.update(fixedDt * simSpeed);
        meshes[0]->updatePlaneHeightmap(backend->context);

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

    // Always render particles if there are any
    if (!backend->context.particleSystem.pages.empty() && showParticles) {
        backend->drawParticles(this, backend->context.particleSystem, VP);
    }

    // Draw text overlay using QPainter
    if (showOverlay) {
        QPainter painter(this);
        drawOverlay(painter);
        painter.end();
    }
}

void GLWidget::drawOverlay(QPainter &painter)
{
    painter.setRenderHint(QPainter::Antialiasing);
    
    size_t particleCount = 0;
    for (const auto &page : backend->context.particleSystem.pages) {
        particleCount += (page.PAGE_SIZE - page.finished);
    }
    
    size_t processCount = backend->context.processes.size();
    
    // Draw background rectangle
    int lineHeight = 20;
    int padding = 10;
    int numLines = 3;

    QRect bgRect(5, 5, 150, numLines * lineHeight + padding);
    painter.fillRect(bgRect, QColor(0, 0, 0, 150));
    
    int y = 20;
    int x = 10;

    // FPS
    painter.setPen(QColor(0, 0, 0));
    painter.drawText(x + 1, y + 1, QString("FPS: %1").arg(QString::number(fps, 'f', 1)));
    painter.setPen(QColor(255, 255, 0));
    painter.drawText(x, y, QString("FPS: %1").arg(QString::number(fps, 'f', 1)));
    y += lineHeight;
    
    // Particle count
    painter.setPen(QColor(0, 0, 0));
    painter.drawText(x + 1, y + 1, QString("Particles: %1").arg(particleCount));
    painter.setPen(QColor(0, 255, 255));
    painter.drawText(x, y, QString("Particles: %1").arg(particleCount));
    y += lineHeight;
    
    // Process count
    painter.setPen(QColor(0, 0, 0));
    painter.drawText(x + 1, y + 1, QString("Processes: %1").arg(processCount));
    painter.setPen(QColor(0, 255, 0));
    painter.drawText(x, y, QString("Processes: %1").arg(processCount));
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
