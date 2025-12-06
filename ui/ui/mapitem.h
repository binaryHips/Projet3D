#ifndef MAPITEM_H
#define MAPITEM_H

#include <QWidget>
#include <QLabel>
#include <QVBoxLayout>
#include "clickablelabel.h"

#include "includes_all_cpu.h"

class MapItem : public QWidget
{
    Q_OBJECT
public:
    explicit MapItem(QWidget *parent = nullptr);
    explicit MapItem(QString image_path, QWidget *parent = nullptr);
    explicit MapItem(QPixmap image, QWidget *parent = nullptr);
    ~MapItem();

    void updateMap(QPixmap im);

    QVBoxLayout *layout;
    QPixmap map;
    ClickableLabel *map_image;
    MAP_LAYERS m_layer = MAP_LAYERS::SAND; // Default val

protected :
    virtual void resizeEvent(QResizeEvent* event) override;

signals:

public slots:

};

#endif // MAPITEM_H
