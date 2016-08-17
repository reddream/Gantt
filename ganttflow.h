#ifndef GANTTFLOW_H
#define GANTTFLOW_H

#include <QObject>
#include <QGraphicsPolygonItem>
class GanttFlow : public QObject, public QGraphicsPolygonItem
{
    Q_OBJECT
public:
    explicit GanttFlow(int flowIndex, QPolygonF points,  QGraphicsItem *parent = 0);
    void isSelected();
    void isNotSelected();
private:
    int flowIndex;
signals:
    void iWasClicked(int index);
protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
public slots:
};

#endif // GANTTFLOW_H
