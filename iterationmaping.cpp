#include "iterationmaping.h"

iterationMaping::iterationMaping(QWidget *parent) : QDialog(parent)
{
    GUI();
    srand(QDateTime::currentDateTime().toTime_t());

    m_iterCalc = new iterationCalc();
}

void iterationMaping::GUI()
{
    m_le_lambda = new QLineEdit("3.5");
    m_le_countPoint = new QLineEdit("5");
    m_le_started = new QLineEdit("-1");
    m_le_ended = new QLineEdit("1");
    m_le_func = new QLineEdit("l*x*(1-x)");//("l*x*(1-x)");
    m_le_derivative = new QLineEdit("l-2*x*l");//("l-2*x*l");
    m_le_start_iter = new QLineEdit("0.25");

    m_lb_countPoint = new QLabel("Величина лестницы");
    m_lb_func = new QLabel("Функция");
    m_lb_lambda = new QLabel("Параметр");
    m_lb_started = new QLabel("Начало построения");
    m_lb_ended = new QLabel("Конец построения");
    m_lb_derivative = new QLabel("Производная");
    m_lb_start_iter = new QLabel("Начало лестницы");

    m_pb_start = new QPushButton("Построить");

    m_plot = new QCustomPlot();
    m_plot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectAxes | QCP::iSelectPlottables);
    m_plot->xAxis->setLabel("x");
    m_plot->yAxis->setLabel("y");

    m_layout = new QGridLayout(this);

    m_layout->addWidget(m_le_lambda, 0, 1, 1, 1);
    m_layout->addWidget(m_lb_lambda, 0, 0, 1, 1);

    m_layout->addWidget(m_le_countPoint, 1, 1, 1, 1);
    m_layout->addWidget(m_lb_countPoint, 1, 0, 1, 1);

    m_layout->addWidget(m_le_start_iter, 2, 1, 1, 1);
    m_layout->addWidget(m_lb_start_iter, 2, 0, 1, 1);

    m_layout->addWidget(m_le_started, 3, 1, 1, 1);
    m_layout->addWidget(m_lb_started, 3, 0, 1, 1);

    m_layout->addWidget(m_le_ended, 4, 1, 1, 1);
    m_layout->addWidget(m_lb_ended, 4, 0, 1, 1);

    m_layout->addWidget(m_le_func, 5, 1, 1, 1);
    m_layout->addWidget(m_lb_func, 5, 0, 1, 1);

    m_layout->addWidget(m_le_derivative, 6, 1, 1, 1);
    m_layout->addWidget(m_lb_derivative, 6, 0, 1, 1);

    m_layout->addWidget(m_pb_start, 7, 1, 1, 1);

    m_layout->addWidget(m_plot, 8, 0, 2, 2);

    connect(m_plot, SIGNAL(mouseWheel(QWheelEvent*)), this, SLOT(mouseWheel()));
    connect(m_plot, SIGNAL(mousePress(QMouseEvent*)), this, SLOT(mousePress()));

    connect(m_pb_start, SIGNAL(clicked()), this, SLOT(slotStartClicked()));

    //    addRandomGraph();
    //    m_plot->rescaleAxes();

}

void iterationMaping::keyPressEvent(QKeyEvent *event)
{
    int key=event->key();

    if ( key==Qt::Key_Space ) {
        m_plot->rescaleAxes();
        m_plot->replot();
    }
}

void iterationMaping::slotStartClicked()
{
    //int count=1000;//кол-во точек (>100)

    //QString s_lam="1.5";//параметр (лямбда)
    //QString s_k="5"; //количество ступений итерационный лестницы
    //QString s_X="0.25";//начало итерационной лестницы
    //QString s_Xmin="0";//левый предел
    //QString s_Xmax="1";//правый предел
    //QString expf="l*x*(1-x)";//функция
    //QString df="l-2*x*l";// производная

    lambda = m_le_lambda->text();
    s_X=m_le_start_iter->text();
    xMin = m_le_started->text();
    xMax = m_le_ended->text();
    func = m_le_func->text();
    derivative = m_le_derivative->text();
    s_k = m_le_countPoint->text();

    abcsX.resize(2);
    iterX.resize(s_k.toInt()*2+1);//итерационная лестница
    iterY.resize(s_k.toInt()*2+1);
    linX.resize(countPoints);//функция
    linY.resize(countPoints);
    stabX.resize(countPoints);//график устойчивых точек
    stabY.resize(countPoints);

    if( m_iterCalc->iter_read(
                countPoints, lambda, s_k, s_X, xMin, xMax, func, derivative,
                &msg, &abcsX[0], &iterX[0], &iterY[0],
                &linX[0], &linY[0], &stabX[0], &stabY[0] )
            ) {

        makeGraph();
    }
    QMessageBox msgBox;
    msgBox.setText(msg);
    msgBox.exec();


    clear();
    m_iterCalc->clear();
}

void iterationMaping::mouseWheel()
{
    if (m_plot->xAxis->selectedParts().testFlag(QCPAxis::spAxis))
        m_plot->axisRect()->setRangeZoom(m_plot->xAxis->orientation());
    else if (m_plot->yAxis->selectedParts().testFlag(QCPAxis::spAxis))
        m_plot->axisRect()->setRangeZoom(m_plot->yAxis->orientation());
    else
        m_plot->axisRect()->setRangeZoom(Qt::Horizontal|Qt::Vertical);
}

void iterationMaping::mousePress()
{
    if (m_plot->xAxis->selectedParts().testFlag(QCPAxis::spAxis))
        m_plot->axisRect()->setRangeDrag(m_plot->xAxis->orientation());
    else if (m_plot->yAxis->selectedParts().testFlag(QCPAxis::spAxis))
        m_plot->axisRect()->setRangeDrag(m_plot->yAxis->orientation());
    else
        m_plot->axisRect()->setRangeDrag(Qt::Horizontal|Qt::Vertical);
}

void iterationMaping::makeGraph()
{

    m_plot->clearGraphs();

    m_plot->addGraph(); // !! стрелки
    m_plot->graph()->setData(iterX, iterY);
    QPen iter;
    iter.setColor(QColor(Qt::green));
    iter.setWidthF(1);
    m_plot->graph()->setPen(iter);

    //QCPScatterStyle iterScatter;
    //iterScatter.setBrush(QCPScatterStyle::ssPeace);
    m_plot->graph(0)->setScatterStyle(QCPScatterStyle::ssPeace);

    m_plot->addGraph();
    m_plot->graph()->setData(abcsX, abcsX);
    QPen abcsPen;
    abcsPen.setColor(QColor(Qt::red));
    abcsPen.setWidthF(1);
    m_plot->graph()->setPen(abcsPen);

    m_plot->addGraph();
    m_plot->graph()->setData(linX, linY);
    QPen func;
    func.setColor(QColor(Qt::blue));
    func.setWidthF(2);
    m_plot->graph()->setPen(func);

    m_plot->addGraph();
    m_plot->graph()->setData(stabX, stabY);
    QPen stab;
    stab.setColor(QColor(Qt::yellow));
    stab.setWidthF(1);
    m_plot->graph()->setPen(stab);

    m_plot->rescaleAxes();
    m_plot->replot();
}

void iterationMaping::clear()
{
    msg.clear();
    abcsX.clear();
    iterX.clear();
    iterY.clear();
    linX.clear();
    linY.clear();
    stabX.clear();
    stabY.clear();
    countSteps.clear();
}
