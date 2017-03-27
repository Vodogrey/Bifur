#ifndef PARAMDIAGRAMMAPING_H
#define PARAMDIAGRAMMAPING_H

#include <QObject>
#include <QWidget>
#include <QDialog>
#include <QLineEdit>
#include <QGridLayout>
#include <QLabel>
#include <QPushButton>
#include <QMenu>
#include <QFileDialog>
#include <QMessageBox>

#include "qcustomplot.h"
#include "paramdiagram.h"


#include <QDebug>


class paramDiagramMaping : public QDialog
{
    Q_OBJECT
public:
    explicit paramDiagramMaping(QWidget *parent = 0);

protected:
    void GUI();
    void addRandomGraph();
    void makeGraph();
    void clear();
    virtual void keyPressEvent(QKeyEvent *event);

signals:

public slots:

private slots:
    void mouseWheel();
    void mousePress();
    void slotStartClicked();
    void rightClick(QPoint pos);
    void saveAsImage();

private:

    int count=1000;// детализация
    int pix;//ширина изображения

    QGridLayout* m_layout;

    QLineEdit* m_le_x;
    QLabel* m_lb_x;

    QLineEdit* m_le_pix;
    QLabel* m_lb_pix;

    QLineEdit* m_le_expf;
    QLabel* m_lb_expf;

    QLineEdit* m_le_started;
    QLabel* m_lb_started;

    QLineEdit* m_le_ended;
    QLabel* m_lb_ended;

    QPushButton* m_pb_start;

    QCustomPlot* m_plot;
    QCPCurve* m_iterCurve;

    paramDiagram* m_paramDiagram;

    QString msg;// сообщение юзеру

    QString expf;//функция
    QString s_x;//Х0
    QString s_start;//начальное значение параметра
    QString s_end;//конечное значение параметра

    QVector<double> paramX;// абциса
    QVector<double> paramY;//итерационная лестница
};

#endif // PARAMDIAGRAMMAPING_H
