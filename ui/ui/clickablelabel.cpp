#include "clickablelabel.h"

ClickableLabel::ClickableLabel(QWidget* parent, Qt::WindowFlags f)
    : QLabel(parent) {
}



ClickableLabel::~ClickableLabel() {}

void ClickableLabel::mousePressEvent(QMouseEvent* event) {
    if(isMap)
    {
        emit clicked(this->pixmap(Qt::ReturnByValue) , layer);
    }else
    {
        emit clickedFeat(this->pixmap(Qt::ReturnByValue) , feat_layer);
    }
}
