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
    explicit MapItem(QPixmap image, MAP_LAYERS layer, QWidget *parent = nullptr);
    explicit MapItem(QPixmap image, FEATURE_LAYERS layer, QWidget *parent = nullptr);
    ~MapItem();

    void updateMap(QPixmap im);

    QVBoxLayout *layout;
    QPixmap map;
    ClickableLabel *map_image;
    bool isMap = true;
    MAP_LAYERS m_layer = MAP_LAYERS::SAND; // Default val
    FEATURE_LAYERS m_feat_layer;

protected :
    virtual void resizeEvent(QResizeEvent* event) override;

signals:

public slots:

};

#endif // MAPITEM_H
