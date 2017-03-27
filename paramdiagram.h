#ifndef PARAMDIAGRAM
#define PARAMDIAGRAM

#include <QObject>
#include <QChar>
#include <qmath.h>

#include "mathcalc.h"
#define EPS 1e-16

class paramDiagram : public QObject
{
    Q_OBJECT
public:
    explicit paramDiagram(QObject *parent = 0);
    bool param_read ( int count, int pix, QString expf, QString s_x, QString s_param1,
                                    QString s_param2, QString* msg, double *paramX, double *paramY);
    void clear();

signals:

public slots:

protected:
    short comp(double a, double b);

    bool param_calc( QString expf, int count, int pix, double param1, double param2, double x, double *iterX, double *iterY );


private:
    mathCalc* m_calc;
};

#endif // PARAMDIAGRAM

