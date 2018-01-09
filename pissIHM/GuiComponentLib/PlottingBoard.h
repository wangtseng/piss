#ifndef PLOTTINGBOARD_H
#define PLOTTINGBOARD_H

#include <QWidget>

#include "qcustomplot.h"
#include "HistogramPoint.h"
#include "TransferPoint.h"
#include "ColorPoint.h"
#include <PlotPoint.h>
#include <QMetaEnum>

class PlottingBoard : public QCustomPlot
{
    Q_OBJECT
public:
    explicit PlottingBoard();
    ~PlottingBoard();

    void setWorkSpaceColor(QString workspaceColor);
    void setWorkSpaceColor(int r, int g, int b);

    void test();

    void embellshing();

    int addCurve(QString curveName, QString abscissa, QString ordinate, QString lineColor, int pointSize);
    int getCurveNumber();

    void doHistogramPlotting(int index, QVector<HistogramPoint*> points);
    void doTransformationPlotting(int index, QVector<TransferPoint*> points);
    void doColorTransformationPlotting(QVector<ColorPoint*> points);
    void legendSetting(bool visible);

    void setAbscissaRange(int min, int max);
    void setOrdinateRange(double min, double max);
    void setBoardSize(int width, int height);
    void setBackgroundColor(int red, int green, int blue);
    void setBasePenProperty(QColor baseLineColor, int width);
    void setTickPenProperty(QColor tickColor, int width);
    void setCurveStyle(QString pointScatterStyle, QString pointColor, int pointSize);
    void setInteractionTolerated(bool drag, bool zoom, bool selectedPlottables);
    void setGridStyle(bool abscissaAxisEnable, QString abscissaGridColor, QString abscissaSubGridColor, bool ordinateAxisEnable, QString ordinateGridColor, QString ordinateSubGridColor);

    int getOrdinateMaxValue();
    int getOrdinateMinValue();

private:
    QCustomPlot *customPlot;
    QFont legendFont;
    int courveCounter;
    PlotPoint *mPointUnderCursor;
    QStringList rgb;
    int y_min;
    int y_max;

    //! need to be analysed....
protected:
    //virtual void mousePressEvent(QMouseEvent *event);
    //virtual void mouseMoveEvent(QMouseEvent *event);
    virtual void keyPressEvent(QKeyEvent *event);
    virtual void keyReleaseEvent(QKeyEvent *event);

signals:
    void shiftStateChanged(bool var);
};

#endif // PLOTTINGBOARD_H
