#include "widget.h"
#include "./ui_widget.h"

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    QVector<double> xData, yData;
    double xMin = 0;  // x轴最小值
    double xMax = 10; // x轴最大值
    double dx = 0.1;  // 步长
    for (double x = xMin; x <= xMax; x += dx) {
        xData.append(x);
    }
    for (int i = 0; i < xData.size(); ++i) {
        double x = xData.at(i);
        double y = qSin(x);  // 正弦函数
        yData.append(y);
    }

    // 清除已存在的图形
    ui->heyplot->clearGraphs();

    // 添加新的图形
    ui->heyplot->addGraph();
    ui->heyplot->graph(0)->setData(xData, yData);

    // 设置x轴和y轴范围
    ui->heyplot->xAxis->setRange(xMin, xMax);
    ui->heyplot->yAxis->setRange(-1, 1); // 正弦函数的范围通常为[-1, 1]

    // 设置轴标签
    ui->heyplot->xAxis->setLabel("X");
    ui->heyplot->yAxis->setLabel("Y");

    // 重新绘制图形
    ui->heyplot->replot();

    connect(ui->heyplot,&HeyPlot::xAxisCursorMove,this,[](double cursorDistance) {
        qDebug() << "x distance " << cursorDistance;
    });
}

Widget::~Widget()
{
    delete ui;
}


void Widget::on_pushButton_clicked()
{
    // 关闭游标
    ui->heyplot->hideCursor(HeyAxis::xAxis);
}


void Widget::on_pushButton_2_clicked()
{
    // 打开游标
    ui->heyplot->showCursor(HeyAxis::xAxis);
}

