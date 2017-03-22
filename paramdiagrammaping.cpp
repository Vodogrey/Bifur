#include "paramdiagrammaping.h"

paramDiagramMaping::paramDiagramMaping(QWidget *parent) : QDialog(parent)
{
    GUI();
}


void paramDiagramMaping::GUI()
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
    m_plot->setContextMenuPolicy(Qt::CustomContextMenu);
    m_plot->xAxis->setLabel("x");
    m_plot->yAxis->setLabel("y");
    m_iterCurve = new QCPCurve(m_plot->xAxis, m_plot->yAxis);

    m_layout = new QGridLayout(this);

    m_layout->addWidget(m_le_lambda, 0, 1, 1, 8);
    m_layout->addWidget(m_lb_lambda, 0, 0, 1, 1);

    m_layout->addWidget(m_le_countPoint, 1, 1, 1, 8);
    m_layout->addWidget(m_lb_countPoint, 1, 0, 1, 1);

    m_layout->addWidget(m_le_start_iter, 2, 1, 1, 8);
    m_layout->addWidget(m_lb_start_iter, 2, 0, 1, 1);

    m_layout->addWidget(m_le_started, 3, 1, 1, 8);
    m_layout->addWidget(m_lb_started, 3, 0, 1, 1);

    m_layout->addWidget(m_le_ended, 4, 1, 1, 8);
    m_layout->addWidget(m_lb_ended, 4, 0, 1, 1);

    m_layout->addWidget(m_le_func, 5, 1, 1, 8);
    m_layout->addWidget(m_lb_func, 5, 0, 1, 1);

    m_layout->addWidget(m_le_derivative, 6, 1, 1, 8);
    m_layout->addWidget(m_lb_derivative, 6, 0, 1, 1);

    m_layout->addWidget(m_pb_start, 7, 4, 1, 1);

    m_layout->addWidget(m_plot, 8, 0, 9, 9);
}
