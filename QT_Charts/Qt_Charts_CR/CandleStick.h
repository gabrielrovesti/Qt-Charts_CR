#ifndef CANDLESTICK_H
#define CANDLESTICK_H

#include "chart.h"

class CandleStick : public Chart{
private:
    QCandlestickSeries *series=nullptr;
    QString nomeSerie="";
public:
    explicit CandleStick(QString="", QStringList = {}, double = 0, double = 20, QColor="black");
    virtual void finalizza();
    virtual void insert(QString, QStringList={"0", "0", "0", "0"});
    virtual void reloadGrafico();
    void updateAxisX();
    void setNomeSerie(QString);
    void setAxis(QStringList, double, double, QColor);
    CandleStick* clone() const{return new CandleStick(*this);}
};

#endif // CANDLESTICK_H
