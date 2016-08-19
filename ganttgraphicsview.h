#ifndef GANTTGRAPHICSVIEW_H
#define GANTTGRAPHICSVIEW_H

#include <QGraphicsView>
#include "ganttrect.h"
#include "ganttflow.h"
class GanttGraphicsView : public QGraphicsView
{
public:
    QGraphicsScene * scene;
    GanttGraphicsView();
    QList<GanttRect*> * tasks;
    QList<GanttFlow*> * flows;
    QList<QGraphicsLineItem*> * lines;
    QList<QGraphicsTextItem*> * unitLabels;
protected:
    void resizeEvent(QResizeEvent * event);
    void keyPressEvent(QKeyEvent * e);
    void keyReleaseEvent(QKeyEvent * e);
};

#endif // GANTTGRAPHICSVIEW_H
