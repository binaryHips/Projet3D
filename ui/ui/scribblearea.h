#ifndef SCRIBBLEAREA_H
#define SCRIBBLEAREA_H

#include <QWidget>
#include <QImage>
#include <QPixmap>
#include <QColor>

class ScribbleArea : public QWidget
{
    Q_OBJECT

public:
    explicit ScribbleArea(QWidget *parent = nullptr);

    bool openImage(const QString &fileName);
    void loadImage(const QPixmap &pixmap);
    void setBackgroundPixmap(const QPixmap &pixmap);
    void setPenColor(const QColor &color) { m_penColor = color; }
    void setPenWidth(int w) { m_penWidth = w; }
    void setPenOpacity(int alpha) { m_opacity = alpha;}
    void clearOverlay();
    bool saveImage(const QString &fileName, const char *fileFormat = "PNG") const;

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
};

#endif // SCRIBBLEAREA_H
