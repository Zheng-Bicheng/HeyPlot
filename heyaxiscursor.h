#ifndef HEYAXISCURSOR_H
#define HEYAXISCURSOR_H

#include "heycursor.h"
#include "qcustomplot.h"

typedef enum HeyAxis{
    xAxis,
    yAxis,
    unknown
} HeyAxis;


class HeyAxisCursor : public QObject
{
    Q_OBJECT
public:
    HeyAxisCursor(QCustomPlot *parent, HeyAxis axis);
    ~HeyAxisCursor();
    bool isShowing();
    bool show(double positionRatio = 0.1);
    bool hide();

    bool handleMousePress(QMouseEvent *event);
    bool handleMouseRelease(QMouseEvent *event);
    bool handleMouseMove(QMouseEvent *event);

    double getCursorDistance();

private:
    QCustomPlot *parent_ = nullptr;
    HeyAxis axis_ = HeyAxis::unknown;

    QPair<QPointer<HeyCursor>, QPointer<HeyCursor>> axisCursor;
    bool createCursor();
    void removeCursor();
    QPair<QPair<QPointF, QPointF>, QPair<QPointF, QPointF>> getCursorPosition(double positionRatio);

    bool isOnCursor(const HeyCursor& cursor, const QPointF& point, double max_length = 0);
};

#endif // HEYAXISCURSOR_H
