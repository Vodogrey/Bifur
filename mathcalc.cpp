#include "mathcalc.h"

mathCalc::mathCalc(QObject *parent) : QObject(parent)
{
    setPatterns();
}

void mathCalc::setPatterns()
{
    EXP.setPattern ( "(.*)(\\-|\\+)(.*)" );
    TRM.setPattern ( "(.*)(\\/|\\*|\\^)(.*)" );
    PRM.setPattern ( "(\\()(.*)(\\))|(\\|)(.*)(\\|)|"
                     "((LOG)(\\()(.*)(,)(.*)(\\)))|"
                     "((sin|cos|tg|ctg)(\\()(.*)(\\)))|"
                     "((([0-9]+\\.[0-9]+)|([0-9]+)))|(e)|(pi)|(x)|(l)"
                     );
}

double mathCalc::calc(QString data, QString* ptr, double a, double b)
{


    _X=a;
    _L=b;
    double res=expr(data);
    if(_i){
        int size=ErrorLog.length();
        _i=ErrorLog.mid(size-1,1).toInt();
        switch (_i) {
        case 1:
            ErrorLog="некорректное выражение";
            break;
        case 2:
            ErrorLog="";
            ErrorLog.resize(size-1);
            ErrorLog="деление на 0";
            break;
        case 3:
            ErrorLog="";
            ErrorLog.resize(size-2);
            ErrorLog="переполнение";
            break;
        case 4:
            ErrorLog="Операнды логарифма меньше нуля";
            break;
        default:
            ErrorLog="некорректное выражение";
            break;
        }


    }
    *ptr=ErrorLog;
    _i=0;
    _max=0;
    _L=0;
    _X=0;
    return res;
}

double mathCalc::error(int n)
{
    if(_max<n)_max=n;
    _i=n;
    return NAN;
}

double mathCalc::plus(double a, double b){
    double res=a+b;
    OF;
}

double mathCalc::min(double a, double b){
    double res=a-b;
    OF;
}

double mathCalc::im(double a, double b){
    double res=a*b;
    OF;
}

double mathCalc::pw(double a, double b){
    double res=pow(a,b);
    OF;
}

double mathCalc::div(double a, double b){
    if(!b){
        error(2);
        ErrorLog+=QString::number(_max);
        return NAN;
    }
    double res=a/b;
    OF;
}



double mathCalc::exprch(QString str)
{
    double res=expr(str);

    if(isnan(res))
        if(_max>1){
            ErrorLog="в скобках "+QString::number(_max);
            _max=0;
        }

    return res;
}

double mathCalc::TDBL(QString a){
    double res=a.toDouble();

    if(a.length()>2&&!res)
        return error(3);

    return res;
}

double mathCalc::ABS(QString str)
{
    double res=expr(str);

    if(isnan(res))
        if(_max>1){
            ErrorLog="в модуле "+QString::number(_max);
            _max=0;
        }

    return fabs(res);
}

double mathCalc::COS(QString str)
{
    double res=cos(expr(str));

    if(isnan(res))
        if(_max>1){
            ErrorLog="в косинусе "+QString::number(_max);
            _max=0;
        }

    return res;
}

double mathCalc::SIN(QString str)
{
    double res=sin(expr(str));

    if(isnan(res))
        if(_max>1){
            ErrorLog="в синусе "+QString::number(_max);
            _max=0;
        }

    return res;
}

double mathCalc::TAN(QString str)
{
    double res=tan(expr(str));

    if(isnan(res))
        if(_max>1){
            ErrorLog="в тангенсе "+QString::number(_max);
            _max=0;
        }

    return res;
}

double mathCalc::CTAN(QString str)
{
    double res=tan(expr(str));
    if(!res)
        return error(2);
    res=1/res;

    if(isnan(res))
        if(_max>1){
            ErrorLog="в котангенсе "+QString::number(_max);
            _max=0;
        }

    return res;
}

double mathCalc::LOG(QString a,QString b)
{
    double lg1=expr(a);

    if(lg1<0){
        ErrorLog="в логарифме 4";
        return error(4);
    }

    double lg2=expr(b);

    if(lg2<0){
        ErrorLog="в логарифме 4";
        return error(4);
    }

    lg1=log(lg2)/log(lg1);

    if(isnan(lg1))
        if(_max>1){
            ErrorLog="в логарифме "+QString::number(_max);
            _max=0;
        }

    return lg1;
}

double mathCalc::expr(QString str){
    QString b0,a,b,sgn,buf=str;
    double left, right;
    while(EXP.indexIn(str,0)!=-1){
        a=EXP.cap(1),b=EXP.cap(3)+sgn+b0,sgn=EXP.cap(2);
        left=expr(a);
        if(!_i)
            right=term(b);
        if(!_i){
            if(sgn=="+")
                return plus(left,right);
            if(sgn=="-")
                return min(left,right);
        }else{
            str=a;
            b0=b;
            _i=0;
        }
    }
    return term(buf);
}

double mathCalc::term(QString str){
    QString b0,a,b,sgn,buf=str;
    double left, right;
    while(TRM.indexIn(str,0)!=-1){
        a=TRM.cap(1),b=TRM.cap(3)+sgn+b0,sgn=TRM.cap(2);
        left=term(a);
        if(!_i)
            right=prim(b);
        if(!_i){
            if(sgn=="*")
                return im(left,right);
            if(sgn=="^")
                return pw(left,right);
            if(sgn=="/")
                return div(left,right);
        }else{
            str=a;
            b0=b;
            _i=0;
        }
    }
    return prim(buf);
}

double mathCalc::prim(QString str){

    double min=1;
    if (str[0]==45){
        min=-1;
        str=str.mid(1);

    }
    if(!(PRM.indexIn(str,0))&&!(str.length()-PRM.matchedLength())){

        QString ab=PRM.cap(5);
        QString num=PRM.cap(19);
        QString exp=PRM.cap(2);
        QString tr=PRM.cap(15);
        QString trval=PRM.cap(17);
        QString lg1=PRM.cap(10);
        QString lg2=PRM.cap(12);
        QString pi=PRM.cap(24);
        QString e=PRM.cap(23);
        QString x=PRM.cap(25);
        QString l=PRM.cap(26);
        if(x.length())
            return _X;
        if(l.length())
            return _L;
        if(pi.length())
            return M_PI;
        if(e.length())
            return M_E;
        if(num.length())
            return min*TDBL(num);
        if(ab.length())
            return min*ABS(ab);
        if(exp.length())
            return min*exprch(exp);
        if(tr=="cos")
            return min*COS(trval);
        if(tr=="sin")
            return min*SIN(trval);
        if(tr=="tg")
            return min*TAN(trval);
        if(tr=="ctg")
            return min*CTAN(trval);
        if(lg1.length()&&lg2.length())
            return min*LOG(lg1,lg2);

    }
    return error(1);
}

