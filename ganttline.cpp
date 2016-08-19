#include "ganttline.h"
#include <QPen>
GanttLine::GanttLine(int flowIndex, qreal x1, qreal y1, qreal x2, qreal y2, QGraphicsItem *parent) : QGraphicsLineItem(x1, y1, x2, y2, parent){
    this->flowIndex = flowIndex;
    QPen unitPen(Qt::black, 3, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin);
    unitPen.setWidth(0);
    setPen(unitPen);
    //setVisible(false);
}
void GanttLine::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    iWasClicked(flowIndex);

}
