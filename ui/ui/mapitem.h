#ifndef MAPITEM_H
#define MAPITEM_H

#include <QWidget>
#include <QLabel>
#include <QVBoxLayout>

class MapItem : public QWidget
{
    Q_OBJECT
public:
    explicit MapItem(QWidget *parent = nullptr);
    explicit MapItem(QString image_path, QWidget *parent = nullptr);
    ~MapItem();
    QVBoxLayout layout;
    QLabel map_image;


signals:
};

#endif // MAPITEM_H
