#ifndef QTABLEWIDGETNUMBERITEM_H
#define QTABLEWIDGETNUMBERITEM_H

#include <QTableWidgetItem>

class QTableWidgetNumberItem : public QTableWidgetItem
{
public:
    QTableWidgetNumberItem(float number);
    bool operator <(const QTableWidgetItem &other) const{
        return text().toFloat() < other.text().toFloat();
    }

};

#endif // QTABLEWIDGETNUMBERITEM_H
