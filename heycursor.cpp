#include "heycursor.h"


HeyCursor::HeyCursor(const QPair<QPointF, QPointF>& point, QCustomPlot *parent, const QPen& pen)
    : QCPItemLine(parent)
{
    // 设置笔刷
    setPen(pen);

    // 设置数据类型
    start->setType(QCPItemPosition::ptPlotCoords);
    end->setType(QCPItemPosition::ptPlotCoords);

    start->setCoords(point.first.x(), point.first.y());
    end->setCoords(point.second.x(), point.second.y());

    // 设置游标可选
    setSelectable(true);
}

bool HeyCursor::getDragging()
{
    return dragging_;
}

void HeyCursor::setDragging(bool dragging)
{
    setSelected(dragging);
    dragging_ = dragging;
}


