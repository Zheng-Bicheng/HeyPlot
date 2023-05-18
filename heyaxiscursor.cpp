#include "heyaxiscursor.h"
#include <QDebug>
#include <iostream>
#include <utility>
#include <tuple>

HeyAxisCursor::HeyAxisCursor(QCustomPlot *parent, HeyAxis axis)
    : parent_{parent}, axis_{axis}
{
    if((axis_ != HeyAxis::xAxis) && (axis_ != HeyAxis::yAxis)){
        qDebug() << "When initializing HeyAxisCursor, the HeyAxis parameter only supports XAxis or YAxis.";
        QApplication::exit(-1);
    }
}

HeyAxisCursor::~HeyAxisCursor()
{
    removeCursor();
}

bool HeyAxisCursor::isShowing()
{
    if(axisCursor.first == nullptr || axisCursor.second == nullptr){
        return false;
    }
    return true;
}

bool HeyAxisCursor::show(double positionRatio)
{
    if(axisCursor.first != nullptr || axisCursor.second != nullptr){
        qDebug() << "The HeyAxisCursor has already been showed.";
        return false;
    }

    if((axis_ != HeyAxis::xAxis) && (axis_ != HeyAxis::yAxis)){
        qDebug() << "When initializing HeyAxisCursor, the HeyAxis parameter only supports XAxis or YAxis.";
        return false;
    }
    auto cursorPosition = getCursorPosition(positionRatio);
    axisCursor.first = new HeyCursor(cursorPosition.first, parent_);
    axisCursor.second = new HeyCursor(cursorPosition.second, parent_);
    parent_->replot();
    return true;
}

bool HeyAxisCursor::hide()
{
    if(axisCursor.first == nullptr || axisCursor.second == nullptr){
        return false;
    }
    parent_->removeItem(axisCursor.first);
    parent_->removeItem(axisCursor.second);
    removeCursor();
    parent_->replot();
    return true;
}

bool HeyAxisCursor::handleMousePress(QMouseEvent *event)
{
    if(!isShowing()){
        return false;
    }

    HeyCursor* cursors[2];
    std::tie(cursors[0], cursors[1]) = axisCursor;
    for(int i=0;i<2;i++){
        if(isOnCursor(*cursors[i],event->pos())){
            cursors[i]->setDragging(true);
            parent_->replot();
            event->accept();
            return true;
        }
    }
    return false;
}

bool HeyAxisCursor::handleMouseRelease(QMouseEvent *event)
{
    if(!isShowing()){
        return false;
    }

    HeyCursor* cursors[2];
    std::tie(cursors[0], cursors[1]) = axisCursor;
    for(int i=0;i<2;i++){
        if(isOnCursor(*cursors[i],event->pos())){
            cursors[i]->setDragging(false);
            parent_->replot();
            event->accept();
            return true;
        }
    }
    return false;
}

bool HeyAxisCursor::handleMouseMove(QMouseEvent *event)
{
    if(!isShowing()){
        return false;
    }
    HeyCursor* cursors[2];
    std::tie(cursors[0], cursors[1]) = axisCursor;
    for(int i=0;i<2;i++){
        if(cursors[i]->getDragging() == true){
            // 计算鼠标移动的偏移量
            double xAxisValue = parent_->xAxis->pixelToCoord(event->pos().x());
            cursors[i]->start->setCoords(xAxisValue, parent_->yAxis->range().lower);
            cursors[i]->end->setCoords(xAxisValue, parent_->yAxis->range().upper);
            // 重新绘制
            parent_->replot();
            // 阻止事件继续传递给父控件或基类
            event->accept();
        }
    }
    return true;
}

double HeyAxisCursor::getCursorDistance()
{
    if(!isShowing()){
        return -1;
    }

    if(axis_ == HeyAxis::xAxis){
        return qAbs(axisCursor.first->start->coords().x() - axisCursor.second->start->coords().x());
    }else if(axis_ == HeyAxis::yAxis){
        return qAbs(axisCursor.first->start->coords().y() - axisCursor.second->start->coords().y());
    }
    return -1;
}

bool HeyAxisCursor::createCursor()
{
    if(axis_ == HeyAxis::unknown){
        qDebug() << "When create Cursor, the HeyAxis parameter only supports XAxis or YAxis.";
        return false;
    }

    return true;
}

void HeyAxisCursor::removeCursor()
{
    delete axisCursor.first;
    delete axisCursor.second;
}

QPair<QPair<QPointF, QPointF>, QPair<QPointF, QPointF>> HeyAxisCursor::getCursorPosition(double positionRatio)
{
    double xAxisMin = parent_->xAxis->range().lower;
    double xAxisMax = parent_->xAxis->range().upper;
    double xAxisRangeCenter = parent_->xAxis->range().center();
    double xAxisRangeSize = parent_->xAxis->range().size();

    double yAxisMin = parent_->yAxis->range().lower;
    double yAxisMax = parent_->yAxis->range().upper;
    double yAxisRangeCenter = parent_->yAxis->range().center();
    double yAxisRangeSize = parent_->yAxis->range().size();
    if(axis_ == HeyAxis::xAxis){
        double xAxisLeft = (xAxisRangeCenter - xAxisRangeSize / 2) + xAxisRangeSize * positionRatio;
        double xAxisRight = (xAxisRangeCenter + xAxisRangeSize / 2) - xAxisRangeSize * positionRatio;
        QPair<QPointF, QPointF> xAxisLeftCursorPosition = {{xAxisLeft,yAxisMin}, {xAxisLeft,yAxisMax}};
        QPair<QPointF, QPointF> xAxisRightCursorPosition = {{xAxisRight,yAxisMin}, {xAxisRight,yAxisMax}};
        QPair<QPair<QPointF, QPointF>, QPair<QPointF, QPointF>> xAxisCursorPosition = {xAxisLeftCursorPosition, xAxisRightCursorPosition};
        return xAxisCursorPosition;
    }else if(axis_ == HeyAxis::yAxis){
        double yAxisLow = (yAxisRangeCenter - yAxisRangeSize / 2) + yAxisRangeSize * positionRatio;
        double yAxisHigh = (yAxisRangeCenter + yAxisRangeSize / 2) - yAxisRangeSize * positionRatio;
        QPair<QPointF, QPointF> yAxisLowCursorPosition = {{xAxisMin,yAxisLow}, {xAxisMax,yAxisLow}};
        QPair<QPointF, QPointF> yAxisHighCursorPosition = {{xAxisMin,yAxisHigh}, {xAxisMax,yAxisHigh}};
        QPair<QPair<QPointF, QPointF>, QPair<QPointF, QPointF>> yAxisCursorPosition = {yAxisLowCursorPosition, yAxisHighCursorPosition};
        return yAxisCursorPosition;
    }else{
        return {{},{}};
    }
}

bool HeyAxisCursor::isOnCursor(const HeyCursor& cursor, const QPointF& point, double max_length)
{
    if(max_length == 0){
        max_length = parent_->xAxis->range().size()/250;
    }

    double pointX = cursor.start->keyAxis()->pixelToCoord(point.x());
    double cursorX = cursor.start->coords().x();
    double pointY = cursor.start->valueAxis()->pixelToCoord(point.y());
    double cursorY = cursor.start->coords().y();
    if(axis_ == HeyAxis::xAxis){
        return max_length >= qAbs(pointX - cursorX);
    } else if(axis_ == HeyAxis::yAxis){
        return max_length >= qAbs(pointY - cursorY);
    }
    return false;
}
