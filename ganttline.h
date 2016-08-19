#ifndef GANTTLINE_H
#define GANTTLINE_H

#include <QObject>
#include <QGraphicsLineItem>
class GanttLine : public QObject, public QGraphicsLineItem
{
    Q_OBJECT

public:
    explicit GanttLine(int flowIndex, qreal x1, qreal y1, qreal x2, qreal y2, QGraphicsItem *parent = Q_NULLPTR);
private:
    int flowIndex;
signals:
    void iWasClicked(int index);
protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
public slots:
};

#endif // GANTTLINE_H
