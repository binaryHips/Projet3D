#ifndef MAPITEM_H
#define MAPITEM_H

#include <QWidget>
#include <QLabel>
#include <QVBoxLayout>
#include "clickablelabel.h"

class MapItem : public QWidget
{
    Q_OBJECT
public:
    explicit MapItem(QWidget *parent = nullptr);
    explicit MapItem(QString image_path, QWidget *parent = nullptr);
    ~MapItem();

    virtual void resizeEvent(QResizeEvent* event) override;

    QVBoxLayout *layout;
    QPixmap map;
    ClickableLabel *map_image;

signals:

public slots:
    void clickedImage();

};

#endif // MAPITEM_H
