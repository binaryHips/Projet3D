#include "scribblearea.h"

#include <QPainter>
#include <QMouseEvent>
#include <QPaintEvent>
#include <QResizeEvent>
#include <QDebug>

ScribbleArea::ScribbleArea(QWidget *parent)
    : QWidget(parent)
{
    m_background = QImage(size(), QImage::Format_RGB32);
    m_background.fill(Qt::white);

    m_overlay = QImage(size(), QImage::Format_ARGB32_Premultiplied);
    m_overlay.fill(Qt::transparent);

    setAttribute(Qt::WA_StaticContents);

    // openImage(":/test.png");

}

bool ScribbleArea::openImage(const QString &fileName)
{
    QImage loaded;
    if (!loaded.load(fileName))
        return false;

    m_background = loaded.convertToFormat(QImage::Format_RGB32);
    resizeCanvas(m_background.size());
    update();
    m_modified = false;
    return true;
}

void ScribbleArea::loadImage(const QPixmap &pixmap)
{
    m_background = pixmap.toImage();
    update();
    m_modified = false;
}

void ScribbleArea::setBackgroundPixmap(const QPixmap &pixmap)
{
    if (pixmap.isNull())
        return;

    m_background = pixmap.toImage().convertToFormat(QImage::Format_RGB32);
    resizeCanvas(m_background.size());
    update();
}

void ScribbleArea::clearOverlay()
{
    if (!m_overlay.isNull()) {
        m_overlay.fill(Qt::transparent);
        m_modified = false;
        update();
    }
}

bool ScribbleArea::saveImage(const QString &fileName, const char *fileFormat) const
{
    if (m_background.isNull())
        return false;

    QImage result = m_background.convertToFormat(QImage::Format_ARGB32_Premultiplied);
    QPainter painter(&result);
    painter.drawImage(0, 0, m_overlay);
    return result.save(fileName, fileFormat);
}

void ScribbleArea::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        m_lastPoint = event->pos();
        m_drawing = true;
    }
}

void ScribbleArea::mouseMoveEvent(QMouseEvent *event)
{
    if ((event->buttons() & Qt::LeftButton) && m_drawing) {
        drawLineTo(event->pos());
    }
}

void ScribbleArea::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton && m_drawing) {
        drawLineTo(event->pos());
        m_drawing = false;
    }
}

void ScribbleArea::paintEvent(QPaintEvent * /* event */)
{
    QPainter painter(this);
    // background and overlay are maintained in widget coordinates
    if (!m_background.isNull())
        painter.drawImage(0, 0, m_background);
    if (!m_overlay.isNull())
        painter.drawImage(0, 0, m_overlay);
}

void ScribbleArea::resizeEvent(QResizeEvent *event)
{
    QSize newSize = event->size();
    // resizeCanvas(newSize);
    QWidget::resizeEvent(event);
}

void ScribbleArea::setPenColor(const QColor &color)
{
    setEraser(false);
    m_penColor = color;
}

void ScribbleArea::drawLineTo(const QPoint &endPoint)
{
    if (m_overlay.isNull())
        return;

    QPainter painter(&m_overlay);

    if (m_eraserMode) {
        // erase
        painter.setCompositionMode(QPainter::CompositionMode_Clear);
        QPen pen(Qt::transparent, m_penWidth, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin);
        painter.setPen(pen);
    } else {

        // Strong color in da middle
        QColor centerColor = m_penColor;
        centerColor.setAlpha(m_opacity);

        // Smaller radius for center to have 2 steps basically
        qreal centerRadius = qMax(1.0, m_penWidth / 2.0);

        QRadialGradient gradient(m_lastPoint, centerRadius);

        qreal midPos = 0.2 + 0.6 * m_softness;
        int midAlpha = qBound(0, int(m_opacity * (0.25 + 0.75 * m_softness)), 255);

        QColor midColor = centerColor;
        midColor.setAlpha(midAlpha);
        QColor edgeColor = centerColor;
        edgeColor.setAlpha(0);

        // same color for mid area
        gradient.setColorAt(0.0, centerColor);
        gradient.setColorAt(midPos * 0.5, centerColor);

        gradient.setColorAt(midPos, midColor);
        gradient.setColorAt(1.0, edgeColor);

        QPen pen(QBrush(gradient), m_penWidth, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin);
        pen.setCapStyle(Qt::RoundCap);

        painter.setPen(pen);
    }

    painter.drawLine(m_lastPoint, endPoint);

    int rad = (m_penWidth / 2) + 2;
    QRect dirtyRect = QRect(m_lastPoint, endPoint).normalized().adjusted(-rad, -rad, +rad, +rad);
    update(dirtyRect);

    m_lastPoint = endPoint;
    m_modified = true;
    emit modified();
}

void ScribbleArea::resizeCanvas(const QSize &newSize)
{
    if (newSize.isEmpty())
        return;

    if (m_background.size() != newSize && !m_background.isNull()) {
        m_background = m_background.scaled(newSize, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    } else if (m_background.isNull()) {
        m_background = QImage(newSize, QImage::Format_RGB32);
        m_background.fill(Qt::white);
    }

    if (m_overlay.isNull()) {
        m_overlay = QImage(newSize, QImage::Format_ARGB32_Premultiplied);
        m_overlay.fill(Qt::transparent);
    } else if (m_overlay.size() != newSize) {
        QImage newOverlay(newSize, QImage::Format_ARGB32_Premultiplied);
        newOverlay.fill(Qt::transparent);
        QPainter p(&newOverlay);
        p.setCompositionMode(QPainter::CompositionMode_SourceOver);
        p.drawImage(QRect(QPoint(0, 0), newSize), m_overlay);
        m_overlay = std::move(newOverlay);
    }

    update();
}

QPixmap ScribbleArea::getImage()
{
    if (m_background.isNull())
        return QPixmap();

    QImage result = m_background.convertToFormat(QImage::Format_ARGB32_Premultiplied);
    QPainter painter(&result);
    if (!m_overlay.isNull())
        painter.drawImage(0, 0, m_overlay);
    painter.end();

    return QPixmap::fromImage(result);
}
