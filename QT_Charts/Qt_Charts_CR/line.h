#ifndef LINE_H
#define LINE_H

#include "chart.h"

class line : public Chart{
private:
    QLineSeries *series=nullptr;
public:
    explicit line(QString="", QStringList = {}, double = 0, double = 20, QColor="black");
    virtual void finalizza();
    virtual void insert(QString="", QStringList = {});
    virtual void reloadGrafico();
    void setAxis(QStringList, double, double, QColor);
    line* clone() const{return new line(*this);}
};

#endif // LINE_H
