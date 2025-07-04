#ifndef PIE_H
#define PIE_H

#include "chart.h"

class pie : public Chart{
private:
    QPieSeries *series=nullptr;
    bool isDonut=false;
public:
    explicit pie(QString="", bool b = false, QStringList = {}, double = 0, double = 20, QColor="black");
    virtual void insert(QString="", QStringList = {});
    virtual void finalizza();
    virtual void reloadGrafico();
    bool getDonut() const;
    void setDonut(bool);
    void setAxis(QStringList, double, double, QColor);
    pie* clone() const{return new pie(*this);}
};
#endif // PIE_H
