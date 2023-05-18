#include "heyplot.h"

HeyPlot::HeyPlot(QWidget *parent) :
    QCustomPlot(parent)
{
    // 启用滚轮缩放 | 鼠标点击拖动 | 主图中的项目可以被选中
    setInteractions(QCP::iRangeZoom | QCP::iRangeDrag | QCP::iSelectItems);
    xAxisCursor = new HeyAxisCursor(this, HeyAxis::xAxis);
    yAxisCursor = new HeyAxisCursor(this, HeyAxis::yAxis);
}

HeyPlot::~HeyPlot()
{

}

bool HeyPlot::showCursor(const HeyAxis& axis)
{
    if(axis == HeyAxis::xAxis){
        return xAxisCursor->show();
    }else if(axis == HeyAxis::yAxis){
        return yAxisCursor->show();
    }else{
        return false;
    }
    return true;
}


bool HeyPlot::hideCursor(const HeyAxis& axis)
{
    if(axis == HeyAxis::xAxis){
        return xAxisCursor->hide();
    }else if(axis == HeyAxis::yAxis){
        return yAxisCursor->hide();
    }else{
        return false;
    }
    return true;
}

void HeyPlot::mouseMoveEvent(QMouseEvent *event)
{
    if(xAxisCursor->handleMouseMove(event)){
        emit xAxisCursorMove(xAxisCursor->getCursorDistance());
        return;
    }else if(yAxisCursor->handleMouseMove(event)){
        emit yAxisCursorMove(yAxisCursor->getCursorDistance());
        return;
    }
    QCustomPlot::mouseMoveEvent(event);
}

void HeyPlot::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        if(xAxisCursor->handleMouseRelease(event) || yAxisCursor->handleMouseRelease(event)){
            return;
        }
    }
    QCustomPlot::mouseReleaseEvent(event);
}

void HeyPlot::mousePressEvent(QMouseEvent *event)
{
    //  检查左键是否按下
    if (event->button() == Qt::LeftButton) {
        if(xAxisCursor->handleMousePress(event) || yAxisCursor->handleMousePress(event)){
            return;
        }
    }
    QCustomPlot::mousePressEvent(event);
}
