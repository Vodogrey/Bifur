#ifndef MATHCALC_H
#define MATHCALC_H

#include <QObject>
#include "fenv.h"
#include <QRegExp>
#include <math.h>

#define OF if(!fetestexcept (FE_OVERFLOW)) return res; error(3); ErrorLog+=QString::number(_max); return NAN;

class mathCalc : public QObject
{
    Q_OBJECT
public:
    explicit mathCalc(QObject *parent = 0);
    double calc(QString, QString*, double, double);

signals:

public slots:

protected:

    double expr(QString);
    double term(QString);
    double prim(QString);
    double error(int);

    double exprch(QString);
    double ABS(QString);
    double TDBL(QString);
    double COS(QString);
    double TAN(QString);
    double CTAN(QString);
    double SIN(QString);
    double LOG(QString,QString);

    double plus(double,double);
    double min(double,double);
    double im(double, double);
    double div(double, double);
    double pw(double, double);

private:
    void setPatterns();
    QRegExp EXP;
    QRegExp TRM;
    QRegExp PRM;

    int _i=0, _max=0;
    QString ErrorLog="";
    double _X,_L;

};

#endif // MATHCALC_H
