#include "paramdiagrammaping.h"

paramDiagramMaping::paramDiagramMaping(QWidget *parent) : QDialog(parent)
{
    GUI();
    srand(QDateTime::currentDateTime().toTime_t());
    m_paramDiagram = new paramDiagram();
}


void paramDiagramMaping::GUI()
{
    m_le_pix = new QLineEdit("10");
    m_le_x = new QLineEdit("0.25");
    m_le_started = new QLineEdit("2.8");
    m_le_ended = new QLineEdit("4");
    m_le_expf = new QLineEdit("l*x*(1-x)");

    m_lb_pix = new QLabel("Разрешение");
    m_lb_x = new QLabel("х0");
    m_lb_started = new QLabel("Начало построения");
    m_lb_ended = new QLabel("Конец построения");
    m_lb_expf = new QLabel("f(x,l)=");

    m_pb_start = new QPushButton("Построить");

    m_plot = new QCustomPlot();
    m_plot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectAxes | QCP::iSelectPlottables);
    m_plot->setContextMenuPolicy(Qt::CustomContextMenu);
    m_plot->xAxis->setLabel("L");
    m_plot->yAxis->setLabel("X");
    m_iterCurve = new QCPCurve(m_plot->xAxis, m_plot->yAxis);

    m_layout = new QGridLayout(this);

    m_layout->addWidget(m_le_pix, 0, 1, 1, 8);
    m_layout->addWidget(m_lb_pix, 0, 0, 1, 1);

    m_layout->addWidget(m_le_x, 1, 1, 1, 8);
    m_layout->addWidget(m_lb_x, 1, 0, 1, 1);

    m_layout->addWidget(m_le_expf, 2, 1, 1, 8);
    m_layout->addWidget(m_lb_expf, 2, 0, 1, 1);

    m_layout->addWidget(m_le_started, 3, 1, 1, 8);
    m_layout->addWidget(m_lb_started, 3, 0, 1, 1);

    m_layout->addWidget(m_le_ended, 4, 1, 1, 8);
    m_layout->addWidget(m_lb_ended, 4, 0, 1, 1);

    m_layout->addWidget(m_pb_start, 7, 4, 1, 1);

    m_layout->addWidget(m_plot, 8, 0, 9, 9);

    connect(m_plot, SIGNAL(mouseWheel(QWheelEvent*)), this, SLOT(mouseWheel()));
    connect(m_plot, SIGNAL(mousePress(QMouseEvent*)), this, SLOT(mousePress()));

    connect(m_pb_start, SIGNAL(clicked()), this, SLOT(slotStartClicked()));
    connect(m_plot, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(rightClick(QPoint)));

}

void paramDiagramMaping::keyPressEvent(QKeyEvent *event)
{
    int key=event->key();

    if ( key==Qt::Key_Space ) {
        m_plot->rescaleAxes();
        m_plot->xAxis->setScaleRatio(m_plot->yAxis,1.0);
        m_plot->replot();
    }
}

void paramDiagramMaping::rightClick(QPoint pos)
{
    QMenu* menu = new QMenu(this);
    menu->setAttribute(Qt::WA_DeleteOnClose);

    menu->addAction("Сохранить как изображение", this, SLOT(saveAsImage()));

    menu->popup(m_plot->mapToGlobal(pos));
}

void paramDiagramMaping::saveAsImage()
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

void paramDiagramMaping::slotStartClicked()
{
    QMessageBox msgBox;

    expf=m_le_expf->text();
    pix=(m_le_pix->text()).toInt();
    s_x=m_le_x->text();
    s_start=m_le_started->text();
    s_end=m_le_ended->text();

    if(pix>0){

        paramX.resize(count*pix);
        paramY.resize(count*pix);

        if(m_paramDiagram->param_read (count, pix, expf, s_x, s_start, s_end, &msg, &paramX[0], &paramY[0]))
              makeGraph();
        else{

             msgBox.setText(msg);
             msgBox.exec();

        }

    }else{

         msgBox.setText("Неверное разрешение");
         msgBox.exec();

    }

    clear();
    m_paramDiagram->clear();

}

void paramDiagramMaping::mouseWheel()
{
    if (m_plot->xAxis->selectedParts().testFlag(QCPAxis::spAxis))
        m_plot->axisRect()->setRangeZoom(m_plot->xAxis->orientation());
    else if (m_plot->yAxis->selectedParts().testFlag(QCPAxis::spAxis))
        m_plot->axisRect()->setRangeZoom(m_plot->yAxis->orientation());
    else
        m_plot->axisRect()->setRangeZoom(Qt::Horizontal|Qt::Vertical);

}

void paramDiagramMaping::mousePress()
{
    if (m_plot->xAxis->selectedParts().testFlag(QCPAxis::spAxis))
        m_plot->axisRect()->setRangeDrag(m_plot->xAxis->orientation());
    else if (m_plot->yAxis->selectedParts().testFlag(QCPAxis::spAxis))
        m_plot->axisRect()->setRangeDrag(m_plot->yAxis->orientation());
    else
        m_plot->axisRect()->setRangeDrag(Qt::Horizontal|Qt::Vertical);
}

void paramDiagramMaping::makeGraph()
{

    m_plot->clearGraphs();

    m_iterCurve->setData(paramX, paramY);
    QPen iter;
    iter.setColor(QColor(Qt::black));
    iter.setWidthF(1);
    m_iterCurve->setPen(iter);

    m_plot->rescaleAxes();
    m_plot->xAxis->setScaleRatio(m_plot->yAxis,1.0);
    m_plot->replot();

}

void paramDiagramMaping::clear()
{
    msg.clear();
    expf.clear();
    s_x.clear();
    s_start.clear();
    s_end.clear();

    paramX.clear();
    paramY.clear();

}
