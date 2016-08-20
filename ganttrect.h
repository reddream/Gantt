#ifndef GANTTRECT_H
#define GANTTRECT_H
#include <QPen>
#include <QGraphicsRectItem>
class GanttRect : public QObject, public QGraphicsRectItem
{
    Q_OBJECT
public:
    explicit GanttRect(int taskIndex, int unitIndex, float largestAmount, float start, float end, float amount, int UNITNAMEBARWIDTH, QColor color, QGraphicsItem *parent = Q_NULLPTR);
    int taskIndex;
    int unitIndex;
    void showAmount(bool yes);
    void isSelected();
    void isNotSelected();
    void isMarkedInFlow();
    void isNotMarkedInFlow();
    void blink();
private:
    float largestAmount;
    float start;
    float end;
    float amount;
    int UNITNAMEBARWIDTH;
    QPen pen;

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void hoverEnterEvent(QGraphicsSceneHoverEvent *event);
    void hoverLeaveEvent(QGraphicsSceneHoverEvent *event);
signals:
    void iWasClicked(int index);
    void hoverStart(int index);
    void hoverLeave(int index);
};

#endif // GANTTRECT_H
