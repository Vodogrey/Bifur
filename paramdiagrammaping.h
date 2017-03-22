#ifndef PARAMDIAGRAMMAPING_H
#define PARAMDIAGRAMMAPING_H

#include <QGridLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QDialog>


#include "qcustomplot.h"

#include <QObject>

class paramDiagramMaping : public QDialog
{
    Q_OBJECT
public:
    explicit paramDiagramMaping(QWidget *parent = 0);

protected:
    void GUI();

signals:

public slots:

private:
    QGridLayout* m_layout;

    QLineEdit* m_le_lambda;
    QLabel* m_lb_lambda;

    QLineEdit* m_le_countPoint;
    QLabel* m_lb_countPoint;

    QLineEdit* m_le_func;
    QLabel* m_lb_func;

    QLineEdit* m_le_started;
    QLabel* m_lb_started;

    QLineEdit* m_le_ended;
    QLabel* m_lb_ended;

    QLineEdit* m_le_derivative;
    QLabel* m_lb_derivative;

    QLineEdit* m_le_start_iter;
    QLabel* m_lb_start_iter;

    QPushButton* m_pb_start;

    QCustomPlot* m_plot;
    QCPCurve* m_iterCurve;
};

#endif // PARAMDIAGRAMMAPING_H
