#ifndef SCRIBBLEAREA_H
#define SCRIBBLEAREA_H

#include <QWidget>
#include <QImage>
#include <QPixmap>
#include <QColor>

#include "includes_all_cpu.h"

class ScribbleArea : public QWidget
{
    Q_OBJECT

public:
    explicit ScribbleArea(QWidget *parent = nullptr);

    bool openImage(const QString &fileName);
    void loadImage(const QPixmap &pixmap);
    void setBackgroundPixmap(const QPixmap &pixmap);
    void setPenColor(const QColor &color);
    void setPenWidth(int w) { m_penWidth = w; }
    void setPenOpacity(int alpha) { m_opacity = alpha;}
    void setEraser(bool enabled) { m_eraserMode = enabled; }
    bool isEraser() const { return m_eraserMode; }
    void clearOverlay();
    QPixmap getImage();
    QPixmap getOverlayPixmap();
    void setOverlayPixmap(const QPixmap &pixmap);
    bool saveImage(const QString &fileName, const char *fileFormat = "PNG") const;

    MAP_LAYERS layer = MAP_LAYERS::SAND; // default is still sand

signals:
    void modified();

protected:
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void paintEvent(QPaintEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;

private:
    void drawLineTo(const QPoint &endPoint);
    void resizeCanvas(const QSize &newSize);

    QImage m_background;
    QImage m_overlay;

    bool m_modified = false;
    bool m_drawing = false;
    QPoint m_lastPoint;
    QColor m_penColor = Qt::black;
    int m_penWidth = 3;
    int m_opacity = 255;
    bool m_eraserMode = false;
};

#endif // SCRIBBLEAREA_H
