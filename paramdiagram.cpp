#include "paramdiagram.h"

paramDiagram::paramDiagram(QObject *parent) : QObject(parent)
{
    m_calc = new mathCalc();
}

short paramDiagram::comp(double a, double b){

    double c=a-b;
    if( qFabs(c)<=EPS)//a==b
        return 0;
    if(c<EPS)
        return 2;//a меньше b
    else
        return 3;
}

bool paramDiagram::param_calc( QString expf, int count, int pix, double param1, double param2, double x, double *iterX, double *iterY )
{
    double step;
    QString elog;

    if((step=(param2-param1)/pix)==NAN)
        return false;

   for (int i=0;i<pix; i++){

       param1=param1+step;

       for (int k=0;k<count; k++)
            if((x=m_calc->calc(expf, &elog, x, param1))==NAN)
                return false;


       for (int k=0;k<count; k++){
            if((x=m_calc->calc(expf, &elog, x, param1))==NAN)
                return false;
            iterY[i*count+k]=x;
            iterX[i*count+k]=param1;
       }
   }

    return true;
}

bool paramDiagram::param_read ( int count, int pix, QString expf, QString s_x, QString s_param1,
                                QString s_param2, QString *msg, double *paramX, double *paramY){

    double x;
    double param1;
    double param2;

    param1=m_calc->calc(s_param1, msg, 0, 0);
    if(*msg!=""||s_param1==""){
        *msg+=" - начало построения";
        return false;
    }

    param2=m_calc->calc(s_param2, msg, 0, 0);
    if(*msg!=""||s_param2==""){
        *msg+=" - конец построения";
        return false;
    }

    x=m_calc->calc(s_x, msg, 0, 0);
    if(*msg!=""||s_x==""){
        *msg+=" - x0";
        return false;
    }

    m_calc->calc(expf, msg, x, 0);
    if(*msg=="некорректное выражение"||expf==""){
        *msg+=" - f(x,l)";
        return false;
    }

    if(comp(param2, param1)!=3){
        *msg="max должен быть больше min";
        return false;
    }

    *msg="Ошибка построения. Поменяйте входные параметры.";

    return param_calc( expf, count, pix, param1, param2, x, paramX, paramY );
}

void paramDiagram::clear()
{
    m_calc->clear();
}
