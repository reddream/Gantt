#include "ganttrect.h"
#include "constants.h"
#include <QtGui>
#include <QVariantAnimation>
#include <QTime>
QVariant blinker(const QBrush  &original, const QBrush &trash, qreal progress){
    int i = (int)(progress*5);
    if (i>4){
        return QBrush(original);
    } else if (i%2 == 0){
        return QBrush(Qt::black);
    } else {
        return QBrush(Qt::white);
    }

}

GanttRect::GanttRect(int taskIndex, int unitIndex, float largestAmount,  float start, float end, float amount, int UNITNAMEBARWIDTH, QColor color, QGraphicsItem *parent) : QGraphicsRectItem( start + UNITNAMEBARWIDTH,  (unitIndex+1)*UNITREPHEIGHT-DEFAULTTASKHEIGHT,  end-start ,  DEFAULTTASKHEIGHT*2,  parent)
{
    this->taskIndex = taskIndex;
    this->unitIndex = unitIndex;
    this->largestAmount = largestAmount;
    this->start = start;
    this->end = end;
    this->amount = amount;
    setBrush(color);
    pen = QPen(Qt::black, 0, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin);
    setPen(pen);
    setAcceptHoverEvents(true);
    this->UNITNAMEBARWIDTH = UNITNAMEBARWIDTH;

}
void GanttRect::isSelected(){

    pen.setStyle(Qt::DashLine);
    pen.setColor(Qt::white);
    setPen(pen);
    setZValue(1);



}
void GanttRect::blink(){
    QColor c = brush().color();

    for (int i = 0; i < 5; i++){
        setBrush(Qt::black);
        while ( QTime::currentTime().msec() < 500);
        setBrush(Qt::white);
        while ( QTime::currentTime().msec() < 999);
    }
    setBrush(c);
    /*qRegisterAnimationInterpolator<QBrush>(blinker);
    QPropertyAnimation* animation = new QPropertyAnimation(this, "brush");
    animation->setDuration(3000);
    animation->setStartValue(QBrush(brush().color()));
    animation->setEndValue(QBrush(Qt::red));
    animation->start();*/
}

void GanttRect::isNotSelected(){
    pen.setStyle(Qt::SolidLine);
    pen.setColor(Qt::black);
    setPen(pen);
    setZValue(0);
}

void GanttRect::isMarkedInFlow(){
    pen.setStyle(Qt::DotLine);
    setPen(pen);
}
void GanttRect::isNotMarkedInFlow(){
    pen.setStyle(Qt::SolidLine);
    setPen(pen);
}
void GanttRect::showAmount(bool yes){
    if (yes){
        setRect(start + UNITNAMEBARWIDTH,  (unitIndex+1)*UNITREPHEIGHT - UNITREPHEIGHT * amount / largestAmount,  end-start, UNITREPHEIGHT * amount / largestAmount);
    } else {
        setRect(start + UNITNAMEBARWIDTH,  (unitIndex+1)*UNITREPHEIGHT-DEFAULTTASKHEIGHT,  end-start ,  DEFAULTTASKHEIGHT*2);
    }
}

void GanttRect::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    iWasClicked(taskIndex);

}

void GanttRect::hoverEnterEvent(QGraphicsSceneHoverEvent *event){
    hoverStart(taskIndex);
}
void GanttRect::hoverLeaveEvent(QGraphicsSceneHoverEvent *event){
    hoverLeave(taskIndex);
}
