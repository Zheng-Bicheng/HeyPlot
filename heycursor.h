#ifndef HEYCURSOR_H
#define HEYCURSOR_H

#include "qcustomplot.h"

class HeyCursor : public QCPItemLine
{
    Q_OBJECT
public:
    explicit HeyCursor(QCustomPlot *parent = nullptr) : QCPItemLine(parent){};
    explicit HeyCursor(const QPair<QPointF, QPointF>& point, QCustomPlot *parent, const QPen& pen = QPen(Qt::red,2,Qt::DashLine));
    bool getDragging();
    void setDragging(bool dragging);


private:
    bool dragging_ = false;
};

#endif // HEYCURSOR_H
