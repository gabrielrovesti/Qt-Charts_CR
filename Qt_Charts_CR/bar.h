#ifndef BAR_H
#define BAR_H

#include "chart.h"

class bar: public Chart{
private:
    QBarSeries *series=nullptr;
public:
    explicit bar(QString="", QStringList = {}, double = 0, double = 20, QColor="black");
    virtual void finalizza();
    virtual void insert(QString="", QStringList = {});
    virtual void reloadGrafico();
    void setAxis(QStringList, double, double, QColor);
    bar* clone() const{return new bar(*this);}
};

#endif // BAR_H

