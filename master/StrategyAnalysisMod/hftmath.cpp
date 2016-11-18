#include "hftmath.h"
#include <QDebug>
#include <QMap>

// ------------- Vector Operations -----------------------------------------
double VectorSubstract(QVector<double> &s1, QVector<double> &s2, QVector<double>& result, double *min, double *max)
{
    if(min) *min = 0;
    if(max) *max = 0;
    if((s1.size() != s2.size()) || (s1.size() != result.size())) {
        qDebug() << "Invalid Args";
        return 0.0;
    }
    int sz = s1.size();
    if(sz == 0 ) {
        qDebug() << "size of vector should not be empty!";
        return 0.0;
    }
    double av = 0.0;
    double xmin = DBL_MAX;
    double xmax = DBL_MIN;
    double tmp = 0;

    for( int i = 0; i < sz; i ++ ) {
        tmp = s1[i] - s2[i];
        if(tmp > xmax) xmax = tmp;
        else if(tmp < xmin) xmin = tmp;
        av += tmp;
        result[i] = tmp;
    }

    av /= sz;
    if(min) *min = xmin;
    if(max) *max = xmax;
    return av;
}


double VectorDiv(QVector<double> &s1, QVector<double> &s2, QVector<double> &result, double max_limt, double min_limit)
{
    if(s1.size() != s2.size() || s1.size() != result.size()) {
        return 0.0;
    }
    int sz = s1.size();
    if(sz == 0 ) {
        return 0.0;
    }
    double tmp, ptmp, av;
    tmp = ptmp = 1.0;
    av = 0.0;
    for( int i = 0; i < sz; i ++ ) {
        if(s2[i] == 0.0) tmp = ptmp;
        else tmp = s1[i]/s2[i];
        if(tmp > max_limt) tmp = max_limt;
        else if(tmp < min_limit) tmp = min_limit;
        ptmp = tmp;
        av += tmp;
        result[i] = tmp;
    }

    av /= sz;
    return av;
}


int EmuNormalizeDistribution(double u, double std_devi, QVector<double> &s, double xmin, double xmax, double ymax, int samples)
{
    if(xmin >= xmax)
        return -1 ;
    if(samples <= 0 )
        samples = 1000;
    if(s.size() < samples) {
        return -1;
    }

    double delta = (xmax - xmin)/samples;
    double coeff = 1.0/(sqrt(2*PI)*std_devi);
    double ampl  = ymax/coeff;
    double x  = xmin;
    double tx = 0.0;
    double ty = 0.0;
    for( int i = 0; i < samples; i ++ )  {
        tx = (x - u);
        ty = -((tx*tx)/(2*std_devi*std_devi));
        ty = coeff*exp(ty);
        s[i] = ty*ampl;
        x += delta;
    }

    return 0;
}

void VectorMinMaxElm(QVector<double> &v, double *min, double *max)
{
    int sz = v.size();
    *min  = 0.0;
    *max = 0.0;
    if(sz == 0 )
        return;

    double xmin = DBL_MAX;
    double xmax = DBL_MIN;
    double dx = 0;
    for(int i = 0; i < sz; i ++ ) {
        dx = v[i];
        if(dx > xmax) xmax = dx;
        else if(dx < xmin) xmin = dx;
    }

    *min = xmin;
    *max = xmax;
}


double VectorSkew(QVector<double> &v, double expect, double stddev)
{
    double skew = 0.0;
    int sz = v.size();
    double t = 0.0;
    if(sz < 3)
        return 0.0;

    for( int i = 0; i < sz; i ++ ) {
        t = v[i] - expect;
        skew += (t*t*t);
    }
    skew *= sz;
    t = ((sz-1)*(sz-2)*stddev*stddev*stddev);
    skew  = skew/t;

    return skew;
}


double VectorKurt(QVector<double> &v, double expect, double stddev)
{
    double kurt = 0.0;
    int sz = v.size();

    double t = 0.0;
    for( int i = 0; i < sz; i ++ ) {
        t = v[i] - expect;
        kurt += (t*t*t*t);
    }

    t = (sz-1)*stddev*stddev*stddev*stddev;
    kurt /= t;
    return kurt;
}

double VectorExpect(QVector<double> &v)
{
    double expect = 0.0;
    int sz = v.size();
    if(v.size() == 0 )
        return expect;

    for( int i = 0; i < sz; i ++ ) {
        expect += v[i];
    }
    expect /= sz;
    return expect;
}

double VectorExpect(QVector<double> &v, int startPos, int endPos)
{
    int siz = v.size();
    if(endPos < startPos)
        return 0.0;
    if(startPos < 0 )
        startPos = 0;
    if(endPos > siz - 1) {
        endPos = siz - 1;
    }

    double ex = 0.0;
    int n = 0;
    while(startPos <= endPos) {
        ex += v[startPos];
        ++ n;
        startPos ++;
    }
    ex /= n;
    return ex;
}

double VectorStdDeviation(QVector<double>& s, double expect)
{
    int sz = s.size();
    if(sz == 0 ) {
        qDebug() << "size of vector should not be empty!";
        return 0.0;
    }

    double var = 0.0;
    double diff = 0;
    for( int i = 0; i < sz; i ++ ) {
        diff = s[i] - expect;
        var += diff*diff;
    }
    var /= sz;
    var = sqrt(var);
    return var;
}

