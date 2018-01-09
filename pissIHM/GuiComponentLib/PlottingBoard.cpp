#include "PlottingBoard.h"

PlottingBoard::PlottingBoard() : QCustomPlot(){

    //! des variables utiles....
    this->rgb<<"red"<<"green"<<"blue";
    this->courveCounter = 0;
    mPointUnderCursor = new PlotPoint(this);

    //! des propriete des courbes
    legendFont = QFont("Segoe UI", 9, QFont::AnyStyle, true);  // start out with MainWindow's font..
    this->setFixedHeight(250);

    this->setLocale(QLocale(QLocale::English, QLocale::UnitedKingdom)); // period as decimal separator and comma as thousand separator

    // move bars above graphs and grid below bars:
    this->addLayer("abovemain", this->layer("main"), QCustomPlot::limAbove);
    this->addLayer("belowmain", this->layer("main"), QCustomPlot::limBelow);

    // set some pens, brushes and backgrounds:
    this->xAxis->setBasePen(QPen(Qt::white, 1));
    this->yAxis->setBasePen(QPen(Qt::white, 1));

    this->xAxis->setTickPen(QPen(Qt::white, 1));
    this->yAxis->setTickPen(QPen(Qt::white, 1));

    this->xAxis->setSubTickPen(QPen(Qt::white, 1));
    this->yAxis->setSubTickPen(QPen(Qt::white, 1));

    this->xAxis->setTickLabelColor(Qt::white);
    this->yAxis->setTickLabelColor(Qt::white);

    this->xAxis->grid()->setPen(QPen(QColor("orange"), 1, Qt::SolidLine));
    this->yAxis->grid()->setPen(QPen(QColor("orange"), 1, Qt::SolidLine));
    this->xAxis->grid()->setSubGridPen(QPen(QColor("orange"), 1, Qt::NoPen));
    this->yAxis->grid()->setSubGridPen(QPen(QColor("orange"), 1, Qt::NoPen));
    this->xAxis->grid()->setSubGridVisible(true);
    this->yAxis->grid()->setSubGridVisible(true);
    this->xAxis->grid()->setZeroLinePen(Qt::NoPen);
    this->yAxis->grid()->setZeroLinePen(Qt::NoPen);
    this->xAxis->setLabelColor(Qt::white);
    this->yAxis->setLabelColor(Qt::white);
    this->xAxis->setLabelFont(legendFont);
    this->yAxis->setLabelFont(legendFont);
}

PlottingBoard::~PlottingBoard(){}


void PlottingBoard::embellshing(){
    this->xAxis->setUpperEnding(QCPLineEnding::esSpikeArrow);
    this->yAxis->setUpperEnding(QCPLineEnding::esSpikeArrow);
}

void PlottingBoard::setWorkSpaceColor(QString workspaceColor){
    this->setBackground(QColor(workspaceColor));
}

void PlottingBoard::setWorkSpaceColor(int r, int g, int b){
    this->setBackground(QColor(r,g,b));
}

int PlottingBoard::getOrdinateMaxValue(){
    return y_max;
}

int PlottingBoard::getOrdinateMinValue(){
    return y_min;
}

//--------------------------------------------------------------------------------------------------------------------------------
//!
//! \brief PlottingBoard::setGridStyle
//! \param abscissaAxisEnable
//! \param abscissaGridColor
//! \param abscissaSubGridColor
//! \param ordinateAxisEnable
//! \param ordinateGridColor
//! \param ordinateSubGridColor
//!
void PlottingBoard::setGridStyle(bool abscissaAxisEnable, QString abscissaGridColor, QString abscissaSubGridColor, bool ordinateAxisEnable, QString ordinateGridColor, QString ordinateSubGridColor){
    this->xAxis->grid()->setVisible(abscissaAxisEnable);
    this->xAxis->grid()->setSubGridVisible(abscissaAxisEnable);
    if(abscissaAxisEnable){
        this->xAxis->grid()->setPen(QPen(QColor(abscissaGridColor), 1, Qt::SolidLine));
        this->xAxis->grid()->setSubGridPen(QPen(QColor(abscissaSubGridColor), 1, Qt::SolidLine));
    }
    this->yAxis->grid()->setVisible(ordinateAxisEnable);
    this->yAxis->grid()->setSubGridVisible(ordinateAxisEnable);
    if(ordinateAxisEnable){
        this->yAxis->grid()->setSubGridPen(QPen(QColor(ordinateGridColor), 1, Qt::SolidLine));
        this->yAxis->grid()->setPen(QPen(QColor(ordinateSubGridColor), 1, Qt::SolidLine));
    }

    this->xAxis->grid()->setZeroLinePen(Qt::SolidLine);
    this->yAxis->grid()->setZeroLinePen(Qt::SolidLine);
}

//--------------------------------------------------------------------------------------------------------------------------------
//!
//! \brief PlottingBoard::setBoardSize
//! \param width
//! \param height
//!
void PlottingBoard::setBoardSize(int width, int height){
    this->setFixedSize(width, height);
}

//--------------------------------------------------------------------------------------------------------------------------------
//!
//! \brief PlottingBoard::setBasePenProperty
//! \param baseLineColor
//! \param width
//!
void PlottingBoard::setBasePenProperty(QColor baseLineColor, int width){
    this->xAxis->setBasePen(QPen(baseLineColor, width));
    this->yAxis->setBasePen(QPen(baseLineColor, width));
}

//--------------------------------------------------------------------------------------------------------------------------------
//!
//! \brief PlottingBoard::setTickPenProperty
//! \param tickColor
//! \param width
//!
void PlottingBoard::setTickPenProperty(QColor tickColor, int width){
    this->xAxis->setTickPen(QPen(tickColor, width));
    this->yAxis->setTickPen(QPen(tickColor, width));
    this->xAxis->setSubTickPen(QPen(tickColor, width));
    this->yAxis->setSubTickPen(QPen(tickColor, width));
    this->xAxis->setTickLabelColor(tickColor);
    this->yAxis->setTickLabelColor(tickColor);
}

//--------------------------------------------------------------------------------------------------------------------------------
//!
//! \brief PlottingBoard::setCurveStyle
//! \param pointScatterStyle
//! \param pointColor
//! \param pointSize
//!
void PlottingBoard::setCurveStyle(QString pointScatterStyle, QString pointColor, int pointSize){
    this->graph(0)->setLineStyle(QCPGraph::lsNone);
    if(pointScatterStyle == "circle"){
        this->graph(0)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle, QColor(pointColor), QColor(pointColor), pointSize));
    }
}

//--------------------------------------------------------------------------------------------------------------------------------
//!
//! \brief setInteractionTolerated
//! \param drag
//! \param zoom
//! \param selectedPlottables
//!
void PlottingBoard::setInteractionTolerated(bool drag, bool zoom, bool selectedPlottables){
    if((!drag)&&zoom&&selectedPlottables){
        this->setInteractions(QCP::iRangeZoom | QCP::iSelectPlottables);
    }
}

//! --------------------------------------------------------------------------------------------------------------------------------
//!
//! \brief PlottingBoard::setBackgroundColor
//! \param red
//! \param greeen
//! \param blue
//!
void PlottingBoard::setBackgroundColor(int red, int green, int blue){
    this->setBackground(QColor(red, green, blue));
}

//! --------------------------------------------------------------------------------------------------------------------------------
//!
//! \brief PlottingBoard::getCurveNumber
//! \return
//!
int PlottingBoard::getCurveNumber(){
    return this->courveCounter;
}

//! --------------------------------------------------------------------------------------------------------------------------------
//!
//! \brief PlottingBoard::legendSetting
//! \param visible
//!
void PlottingBoard::legendSetting(bool visible){

    if(!visible){
        this->legend->setVisible(visible);
        return;
    }

    this->legend->setVisible(visible);
    this->legendFont.setPointSize(8); // and make a bit smaller for legend
    this->legend->setFont(legendFont);
    this->legend->setTextColor(Qt::white);
    this->legend->setBrush(QBrush(Qt::transparent));
    this->legend->setBorderPen(QPen(Qt::white));
    this->axisRect()->insetLayout()->setInsetAlignment(0, Qt::AlignTop|Qt::AlignRight);
}

//!--------------------------------------------------------------------------------------------------------------------------------
//!
//! \brief PlottingBoard::setAbscissaRange
//! \param min
//! \param max
//!
void PlottingBoard::setAbscissaRange(int min, int max){
    this->xAxis->setRange(min, max);
}

//! --------------------------------------------------------------------------------------------------------------------------------
//!
//! \brief PlottingBoard::setOrdinateRange
//! \param min
//! \param max
//!
void PlottingBoard::setOrdinateRange(double min, double max){
    this->yAxis->setRange(min, max);
}

//! --------------------------------------------------------------------------------------------------------------------------------
//!
//! \brief PlottingBoard::addCurve
//! \param curveName
//! \param abscissa
//! \param ordinate
//! \param lineColor
//! \return
//!
int PlottingBoard::addCurve(QString curveName, QString abscissa, QString ordinate, QString lineColor, int pointSize){

    if(curveName != "grayValue->color"){
        // create graph and assign data to it:
        this->addGraph();

        // give the axes some labels:
        this->xAxis->setLabel(abscissa);
        this->yAxis->setLabel(ordinate);

        this->graph(0)->setPen(QPen(QColor(lineColor)));
        //this->graph(0)->setBrush(QBrush(QPixmap("./dali.png")));
        this->graph(0)->setLineStyle(QCPGraph::lsLine);
        this->graph(0)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle, Qt::white, Qt::white, pointSize));
        this->graph(0)->setErrorType(QCPGraph::etValue);
        this->graph(0)->setName(curveName);
    }
    else{

        for(unsigned char cpt = 0; cpt < 3; cpt++){
            // create graph and assign data to it:
            this->addGraph();
            // give the axes some labels:
            this->xAxis->setLabel(abscissa);
            this->yAxis->setLabel(ordinate);

            this->graph(cpt)->setPen(QPen(QColor(rgb[cpt])));
            //this->graph(0)->setBrush(QBrush(QPixmap("./dali.png")));
            this->graph(cpt)->setLineStyle(QCPGraph::lsLine);
            this->graph(cpt)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle, QColor(rgb[cpt]), Qt::white, pointSize));
            this->graph(cpt)->setErrorType(QCPGraph::etValue);
            this->graph(cpt)->setName(rgb[cpt]);
         }
    }
    this->replot();

    return this->courveCounter++;
}

//! --------------------------------------------------------------------------------------------------------------------------------
//!
//! \brief PlottingBoard::doPlotting
//! \param index
//! \param points
//!
void PlottingBoard::doHistogramPlotting(int index, QVector<HistogramPoint*> points){
    // Allow user to drag axis ranges with mouse, zoom with mouse wheel and select graphs by clicking:
    this->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectPlottables);

    // generate some data:
    int length = points.size();
    y_min = 1000;
    y_max = 0;

    QVector<double> x(length), y(length);

    for (int i=0; i<length; ++i){

        x[i] = points[i]->getAbscissaValue();
      y[i] = points[i]->getOrdinateValue();

      if(y_max<y[i]){
         y_max = y[i];
      }

      if(y_min >y[i]){
         y_min = y[i];
      }
    }

    this->graph(index)->clearData();
    this->graph(index)->setData(x, y);
    this->xAxis->setRange(points[0]->getAbscissaValue() - 20, points[length-1]->getAbscissaValue()+20);
    this->yAxis->setRange(y_min-200, y_max+200);

    this->replot();
}

//--------------------------------------------------------------------------------------------------------------------------------
//!
//! \brief PlottingBoard::doTransformationPlotting
//! \param index
//! \param points
//!
void PlottingBoard::doTransformationPlotting(int index, QVector<TransferPoint*> points){
    // Allow user to drag axis ranges with mouse, zoom with mouse wheel and select graphs by clicking:
    this->setInteractions(QCP::iRangeZoom | QCP::iSelectPlottables);
    // generate some data:
    int length = points.size();
    int y_min = 1000;
    int y_max = 0;

    QVector<double> x(length), y(length);

    for (int i=0; i<length; ++i){
      x[i] = points[i]->getAbscissaValue();
      y[i] = points[i]->getOrdinateValue();
      if(y_max<y[i]){
         y_max = y[i];
      }

      if(y_min >y[i]){
         y_min = y[i];
      }
    }
    this->graph(index)->clearData();
    this->graph(index)->setData(x, y);

    this->replot();
}

void PlottingBoard::doColorTransformationPlotting(QVector<ColorPoint*> points){
    // Allow user to drag axis ranges with mouse, zoom with mouse wheel and select graphs by clicking:
    this->setInteractions(QCP::iRangeZoom | QCP::iSelectPlottables);
    // generate some data:
    int length = points.size();
    int y_min = 1000;
    int y_max = 0;

    QVector<double> x(length), red(length), green(length), blue(length);

    for (unsigned char i=0; i<length; ++i){
      x[i] = points[i]->getAbscissaValue();
      red[i] = points[i]->getRedValue();
      green[i] = points[i]->getGreenValue();
      blue[i] = points[i]->getBlueValue();

      if(y_max<red[i]){
         y_max = red[i];
      }

      if(y_max<green[i]){
         y_max = green[i];
      }

      if(y_max<blue[i]){
         y_max = blue[i];
      }

      if(y_min >red[i]){
         y_min = red[i];
      }

      if(y_min >green[i]){
         y_min = green[i];
      }

      if(y_min >blue[i]){
         y_min = blue[i];
      }
    }

    this->graph(0)->clearData();
    this->graph(0)->setData(x, red);

    this->graph(1)->clearData();
    this->graph(1)->setData(x, green);

    this->graph(2)->clearData();
    this->graph(2)->setData(x, blue);

    this->replot();
}


//!!!!!!!!!!!!!!!!!
//! to be deleted...
void PlottingBoard::test(){
    this->legend->setVisible(false);
    //this->legend->setFont(QFont("Helvetica", 9));
    this->legend->setRowSpacing(-3);
    QVector<QCPScatterStyle::ScatterShape> shapes;
//    shapes << QCPScatterStyle::ssCross;
//    shapes << QCPScatterStyle::ssPlus;
//    shapes << QCPScatterStyle::ssCircle;
//    shapes << QCPScatterStyle::ssDisc;
//    shapes << QCPScatterStyle::ssSquare;
    shapes << QCPScatterStyle::ssDiamond;
//    shapes << QCPScatterStyle::ssStar;
//    shapes << QCPScatterStyle::ssTriangle;
//    shapes << QCPScatterStyle::ssTriangleInverted;
//    shapes << QCPScatterStyle::ssCrossSquare;
//    shapes << QCPScatterStyle::ssPlusSquare;
//    shapes << QCPScatterStyle::ssCrossCircle;
//    shapes << QCPScatterStyle::ssPlusCircle;
    shapes << QCPScatterStyle::ssPeace;
    shapes << QCPScatterStyle::ssCustom;

    QPen pen;
    // add graphs with different scatter styles:
    for (int i=0; i<shapes.size(); ++i)
    {
      this->addGraph();
      pen.setColor(QColor(qSin(i*0.3)*100+100, qSin(i*0.6+0.7)*100+100, qSin(i*0.4+0.6)*100+100));
      // generate data:
      QVector<double> x(10), y(10);
      for (int k=0; k<10; ++k)
      {
        x[k] = k/10.0 * 4*3.14 + 0.01;
        y[k] = 7*qSin(x[k])/x[k] + (shapes.size()-i)*5;
      }
      this->graph()->setData(x, y);
      this->graph()->rescaleAxes(true);
      this->graph()->setPen(pen);
      //this->graph()->setName(QCPScatterStyle::staticMetaObject.enumerator(QCPScatterStyle::staticMetaObject.indexOfEnumerator("ScatterShape")).valueToKey(shapes.at(i)));
      this->graph()->setLineStyle(QCPGraph::lsLine);
      // set scatter style:
      if (shapes.at(i) != QCPScatterStyle::ssCustom)
      {
        this->graph()->setScatterStyle(QCPScatterStyle(shapes.at(i), 10));
      }
      else
      {
        QPainterPath customScatterPath;
        for (int i=0; i<3; ++i)
          customScatterPath.cubicTo(qCos(2*M_PI*i/3.0)*9, qSin(2*M_PI*i/3.0)*9, qCos(2*M_PI*(i+0.9)/3.0)*9, qSin(2*M_PI*(i+0.9)/3.0)*9, 0, 0);
        this->graph()->setScatterStyle(QCPScatterStyle(customScatterPath, QPen(Qt::black, 0), QColor(40, 70, 255, 50), 10));
      }
    }
    // set blank axis lines:
    this->rescaleAxes();
    this->xAxis->setTicks(false);
    this->yAxis->setTicks(false);
    this->xAxis->setTickLabels(false);
    this->yAxis->setTickLabels(false);

    // make top right axes clones of bottom left axes:
    this->axisRect()->setupFullAxesBox();
}


/*void PlottingBoard::mousePressEvent(QMouseEvent *event){

    if (event->button() == Qt::LeftButton && mPointUnderCursor) {

        mPointUnderCursor->startMoving(event->localPos(),
                                       event->modifiers().testFlag(Qt::ShiftModifier));
        return;
    }

    QCustomPlot::mousePressEvent(event);
}

void PlottingBoard::mouseMoveEvent(QMouseEvent *event){
    QCustomPlot::mouseMoveEvent(event);
    if (event->buttons() == Qt::NoButton) {
        PlotPoint *plotPoint = qobject_cast<PlotPoint*>(itemAt(event->localPos(), true));
        if (plotPoint != mPointUnderCursor) {
            if (mPointUnderCursor == NULL) {
                // cursor moved from empty space to item
                plotPoint->setActive(true);
                setCursor(Qt::OpenHandCursor);
            } else if (plotPoint == NULL) {
                // cursor move from item to empty space
                mPointUnderCursor->setActive(false);
                unsetCursor();
            } else {
                // cursor moved from item to item
                mPointUnderCursor->setActive(false);
                plotPoint->setActive(true);
            }
            mPointUnderCursor = plotPoint;
            replot();
        }
    }
}*/

void PlottingBoard::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Shift) {
        emit shiftStateChanged(true);
    }
    //QCustomPlot::keyPressEvent(event);
}

void PlottingBoard::keyReleaseEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Shift) {
        emit shiftStateChanged(false);
    }
    //QCustomPlot::keyReleaseEvent(event);
}
