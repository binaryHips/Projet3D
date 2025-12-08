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
    qDebug() << "pix size : " << pixmap.size();
    m_background = pixmap.toImage();
    update();
    m_modified = false;
}

void ScribbleArea::setBackgroundPixmap(const QPixmap &pixmap)
{
	if (pixmap.isNull())
		return;

	m_background = pixmap.toImage().convertToFormat(QImage::Format_RGB32);
    qDebug() << "size : " << m_background.size();
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

void ScribbleArea::drawLineTo(const QPoint &endPoint)
{
	if (m_overlay.isNull())
		return;

	QPainter painter(&m_overlay);
    m_penColor.setAlpha(m_opacity);
	QPen pen(m_penColor, m_penWidth, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin);
	painter.setPen(pen);
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
