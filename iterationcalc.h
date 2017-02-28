#ifndef ITERATIONCALC_H
#define ITERATIONCALC_H

#include <QObject>
#include <QChar>
#include <qmath.h>

#include "mathcalc.h"
#define EPS 1e-16

class iterationCalc : public QObject
{
    Q_OBJECT
public:
    explicit iterationCalc(QObject *parent = 0);
    bool iter_read (int count, QString s_lam, QString s_k, QString s_X, QString s_Xmin, QString s_Xmax, QString expf, QString df, QString* msg, int* n, int* k, \
                    double* abcsX, double* iterX, double* iterY, double* linX, double* linY, double* stabX, double* stabY);
    void clear();

signals:

public slots:

protected:
    short comp(double a, double b);

    int iter_stup( QString expf, int k, double lam, double X, double *iterX, double *iterY);

    int iter_lin( QString expf, int count, double lam, double Xmin, double Xmax, double *stabX, double *stabY, double *linX, double *linY);

    int iter_anal( QString df, QString* msg, int n, double lam, double* stabX);


private:
    mathCalc* m_calc;
};

#endif // ITERATIONCALC_H
