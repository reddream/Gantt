#include "ganttflow.h"
#include <QGraphicsPolygonItem>
#include <QPen>
GanttFlow::GanttFlow(int flowIndex, QPolygonF points, QGraphicsItem *parent) : QGraphicsPolygonItem(points ,parent)
{
    this->flowIndex = flowIndex;
    QPen unitPen(Qt::black, 0, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin);
    setPen(unitPen);
    QBrush fillPen(QColor(200,200,200,127));
    setBrush(fillPen);
    //setVisible(false);
}
void GanttFlow::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    iWasClicked(flowIndex);

}
void GanttFlow::isSelected(){
    QPen pen(Qt::black, 0, Qt::DashLine, Qt::RoundCap, Qt::RoundJoin);
    setPen(pen);
}
void GanttFlow::isNotSelected(){
    QPen pen(Qt::black, 0, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin);
    setPen(pen);
}
