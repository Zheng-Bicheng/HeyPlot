#ifndef HEYPLOT_H
#define HEYPLOT_H

#include "heyaxiscursor.h"
#include "qcustomplot.h"


class HeyPlot : public QCustomPlot
{
    Q_OBJECT
public:
    explicit HeyPlot(QWidget *parent = nullptr);
    ~HeyPlot();

    bool showCursor(const HeyAxis& axis);
    bool hideCursor(const HeyAxis& axis);
    bool axisCursorIsInitialize();

signals:
    void xAxisCursorMove(double cursorDistance);
    void yAxisCursorMove(double cursorDistance);

protected:
    virtual void mouseMoveEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
    virtual void mouseReleaseEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
    virtual void mousePressEvent(QMouseEvent *event) Q_DECL_OVERRIDE;

private:
    QPointer<HeyAxisCursor> xAxisCursor;
    QPointer<HeyAxisCursor> yAxisCursor;
};




#endif // HEYPLOT_H
