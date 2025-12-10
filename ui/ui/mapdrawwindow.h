#ifndef MAPDRAWWINDOW_H
#define MAPDRAWWINDOW_H

#include <QWidget>

// This object will in theory contain all the logic that the second page will have,
// the main window will own this object its just to declutter the MainWindow cuz idk i hate massive classes.

class MapDrawWindow : public QWidget
{
    Q_OBJECT
public:
    explicit MapDrawWindow(QWidget *parent = nullptr);

private:

signals:

};

#endif // MAPDRAWWINDOW_H
