#include "ganttgraphicsview.h"
#include <QtGui>
GanttGraphicsView::GanttGraphicsView()
{
    scene = new QGraphicsScene(0,0,frameSize().width(),frameSize().height());
    setScene(scene);
    tasks = new QList<GanttRect*>();
    lines = new QList<QGraphicsLineItem*>();
    unitLabels = new QList<QGraphicsTextItem*>();
    flows = new QList<GanttFlow*>();
    show();

}
void GanttGraphicsView::resizeEvent(QResizeEvent * event)
{

    //fitInView(scene->sceneRect(), Qt::KeepAspectRatio);
    //

};
void GanttGraphicsView::keyPressEvent(QKeyEvent *e){
    if (e->key() == Qt::Key_Shift){
        setDragMode(QGraphicsView::ScrollHandDrag);
    }else {
        QGraphicsView::keyPressEvent(e);
    }
}
void GanttGraphicsView::keyReleaseEvent(QKeyEvent *e){
    if (e->key() == Qt::Key_Shift){
        setDragMode(QGraphicsView::NoDrag);
    } else {
        QGraphicsView::keyPressEvent(e);
    }
}
