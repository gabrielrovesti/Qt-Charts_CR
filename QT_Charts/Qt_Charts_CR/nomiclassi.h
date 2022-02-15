#ifndef NOMICLASSI_H
#define NOMICLASSI_H

#include <QtCharts>

class nomiClassi{
private:
    static QString ClasseLine;
    static QString ClasseCandleStick;
    static QString ClassePie;
    static QString ClasseBar;
public:
    static QString getLineClass();
    static QString getCandleStickClass();
    static QString getPieClass();
    static QString getBarClass();
    ~nomiClassi()=default;
};

#endif // NOMICLASSI_H
