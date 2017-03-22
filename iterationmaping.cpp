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

    connect(m_plot, SIGNAL(mouseWheel(QWheelEvent*)), this, SLOT(mouseWheel()));
    connect(m_plot, SIGNAL(mousePress(QMouseEvent*)), this, SLOT(mousePress()));

    connect(m_pb_start, SIGNAL(clicked()), this, SLOT(slotStartClicked()));
    connect(m_plot, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(rightClick(QPoint)));


    //    addRandomGraph();
    //    m_plot->rescaleAxes();
}

void iterationMaping::keyPressEvent(QKeyEvent *event)
{
    int key=event->key();

    if ( key==Qt::Key_Space ) {
        m_plot->rescaleAxes();
        m_plot->xAxis->setScaleRatio(m_plot->yAxis,1.0);
        m_plot->replot();
    }
}

void iterationMaping::rightClick(QPoint pos)
{
    QMenu* menu = new QMenu(this);
    menu->setAttribute(Qt::WA_DeleteOnClose);

    menu->addAction("Сохранить как изображение", this, SLOT(saveAsImage()));

    menu->popup(m_plot->mapToGlobal(pos));
}

void iterationMaping::saveAsImage()
{
    QString fileWay = QFileDialog::getSaveFileName(this,
                                                QString::fromUtf8("Выберите файл"),
                                                QDir::currentPath(),
                                                "BMP (*.bmp);;JPG (*.jpg);; PNG (*.png)");
    QFile file(fileWay);
    QString fileType = QFileInfo(file).suffix();

    if(!file.open(QIODevice::WriteOnly | QFile::WriteOnly)) {
        QMessageBox::warning(0, "Ошибка создания файла",
                             QObject::tr("\n Не удалось создать файл."));
        return;
    }

    if(fileType == "bmp")
        m_plot->saveBmp(fileWay, 0, 0, 1.0, -1);
    if(fileType == "jpg")
        m_plot->saveJpg(fileWay, 0, 0, 1.0, -1);
    if(fileType == "png")
        m_plot->savePng(fileWay, 0, 0, 1.0, -1);

}

void iterationMaping::slotStartClicked()
{

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

    m_iterCurve->setData(iterX, iterY);
    QPen iter;
    iter.setColor(QColor(Qt::green));
    iter.setWidthF(1);
    m_iterCurve->setPen(iter);

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
    m_plot->xAxis->setScaleRatio(m_plot->yAxis,1.0);
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
