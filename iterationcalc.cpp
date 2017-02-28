#include "iterationcalc.h"

iterationCalc::iterationCalc(QObject *parent) : QObject(parent)
{
    m_calc = new mathCalc();
}


short iterationCalc::comp(double a, double b){

    double c=a-b;
    if( qFabs(c)<=EPS)//a==b
        return 0;
    if(c<EPS)
        return 2;//a меньше b
    else
        return 3;
}

int iterationCalc::iter_stup( QString expf, int k, double lam,
                              double X, double *iterX, double *iterY)
{
    QString elog="";

    iterX[0]=X;
    iterY[0]=0;

    for(int i=1;i<k*2+1;i++){

        iterX[i]=X;
        X=m_calc->calc (expf, &elog, X, lam);

        if (X==NAN)  {
            if (i==1) {
                iterX[0]=0;
                iterX[1]=0;
                return 1;
            }
            else  return 0;
        }

        iterY[i]=X;

        iterY[++i]=X;
        iterX[i]=X;
    }

    return 0;
}

int iterationCalc::iter_lin( QString expf, int count, double lam, double Xmin,
                             double Xmax, double *stabX, double *stabY, double *linX, double *linY)
{
    QString elog="";

    short cmp;
    short cmp_buf;
    int n=0;
    double stb_buf;
    double stp=(Xmax-Xmin)/count;

    for(int i=0;i<count;i++){

        linX[i]=Xmin+i*stp;
        linY[i]=m_calc->calc(expf, &elog, linX[i], lam);

        //if (*elog!="")
        //    return -1;

        cmp_buf=comp(linY[i], linX[i]);

        if (((cmp_buf==0)||(abs(cmp-cmp_buf)==1))&&i) {
            stb_buf=linX[i-1];
            stb_buf=(linX[i]+linX[i-1])*0.5;
            stabX[n]=stb_buf;
            stabY[n]=0;
            stabX[++n]=stb_buf;
            stabY[n]=stb_buf;
            stabX[++n]=stb_buf;
            stabY[n]=0;
            n++;
        }

        cmp=cmp_buf;

    }

    return n/3;
}

int iterationCalc::iter_anal( QString df, QString* msg, int n, double lam, double* stabX)
{
    double Q;
    QString elog="";
    for(int i=0;i<n;i++){

        Q=m_calc->calc(df, &elog, stabX[i*3], lam);

        if (Q==NAN){
            *msg += "Мультипликаор для неподвижной точки X"
                    + QString::number(i) + "="
                    + QString::number(stabX[n*3])
                    + ", т.к. при его вычислении возникла ошибка: "
                    + elog + "\n";
            continue;
        }

        if (fabs(Q)>1)
            *msg+="Неподвижная точка X"+QString::number(i)+"="+QString::number(stabX[i*3])+" неустойчива\n";
        else
            *msg+="Неподвижная точка X"+QString::number(i)+"="+QString::number(stabX[i*3])+" устойчива\n";
    }

    return 0;
}

bool iterationCalc::iter_read (int count, QString s_lam, QString s_k, QString s_X,
                               QString s_Xmin, QString s_Xmax, QString expf,
                               QString df, QString* msg, int* n, int* k,
                               double* abcsX, double* iterX, double* iterY,
                               double* linX, double* linY, double* stabX,
                               double* stabY){

    double lam;
    double X;
    double Xmin;
    double Xmax;

    lam=m_calc->calc(s_lam, msg, 0, 0);
    if(*msg!=""){
        *msg+=" - L";
        return false;
    }

    X=m_calc->calc(s_X, msg, 0, 0);
    if(*msg!=""){
        *msg+=" - Х";
        return false;
    }

    Xmin=m_calc->calc(s_Xmin, msg, 0, 0);
    if(*msg!=""){
        *msg+=" - Xmin";
        return false;
    }

    Xmax=m_calc->calc(s_Xmax, msg, 0, 0);
    if(*msg!=""){
        *msg+=" - Xmax";
        return false;
    }

    double buf=m_calc->calc(s_k, msg, 0, 0);
    if(*msg!=""){
        *msg+=" - k";
        return false;
    }
    *k=(int)buf;

    m_calc->calc(expf, msg, Xmin, lam);
    if(*msg=="некорректное выражение"){
        *msg+=" - f(x)";
        return false;
    }

    m_calc->calc(df, msg, Xmin, lam);
    if(*msg=="некорректное выражение"){
        *msg+=" - df(x)/dx";
        return false;
    }

    if(comp(Xmax, Xmin)!=3){
        *msg="Xmax должен быть больше Xmin";
        return false;
    }
    *msg="";

    abcsX[0]=Xmin;
    abcsX[1]=Xmax;

    *n=iter_lin(expf, count, lam, Xmin, Xmax, &stabX[0], &stabY[0], &linX[0], &linY[0]);

    if(iter_stup(expf, *k, lam, X, &iterX[0], &iterY[0]))
        *msg+="Точка начала построения итерационной диаграммы выбрана вне области определения функции";

    *msg+="Всего неподвижных точек: "+QString::number(*n)+".\n";

    if (n)
        iter_anal( df, msg, *n, lam, &stabX[0]);

    *k=*k*2+1;
    *n=*n*3;

    return true;
}

void iterationCalc::clear()
{
    m_calc->clear();
}
