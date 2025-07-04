#include "pie.h"

pie::pie(QString nome, bool b, QStringList x, double min_Y, double max_Y, QColor col){
    if(max_Y > 0 && !x.isEmpty()){
        minY = min_Y;
        maxY = max_Y;
        axisX = new QBarCategoryAxis();
        axisX->append(x);
        axisX->setLinePenColor(col);

        axisY = new QValueAxis();
        axisY->setRange(minY, maxY);
        axisY->setLinePenColor(col);

        grafico->addAxis(axisX, Qt::AlignBottom);
        grafico->addAxis(axisY, Qt::AlignLeft);
    }
    nomeGrafico = nome;
    series = new QPieSeries();
    if(b){
        series->setHoleSize(0.35);
        isDonut=true;
    }
    else{
        series->setHoleSize(0);
        isDonut=false;
    }
    grafico->setTitle(nomeGrafico);
}

void pie::insert(QString x, QStringList y){
    labels.append(x);
    values.append(y);
    series->append(x, y[0].toDouble());
    finalizza();
}

void pie::reloadGrafico(){
    grafico->removeSeries(series);
    series = new QPieSeries();
    if(isDonut){
        series->setHoleSize(0.35);
    }
    else{
        series->setHoleSize(0);
    }
    serie.clear();
    for(int k=0; k<labels.size(); k++){
        for (int i=0; i<values.size()/labels.size(); i++){
            series->append(labels[k], values[k*(values.size()/labels.size())+i].toDouble());
        }
    }
    finalizza();
}

void pie::finalizza(){
    serie.push_back(series);
    if(!grafico->series().isEmpty()){
        grafico->removeSeries(series);
    }
    grafico->addSeries(series);
    if(maxY > 0 && !axisX->categories().isEmpty()){
        for(int i=0; i<serie.size(); i++){
            QList<QAbstractAxis*> a = serie[i]->attachedAxes();
            if(a.isEmpty()){
                serie[i]->attachAxis(axisX);
                serie[i]->attachAxis(axisY);
            }
            else{
                serie[i]->detachAxis(axisX);
                serie[i]->detachAxis(axisY);
                serie[i]->attachAxis(axisX);
                serie[i]->attachAxis(axisY);
            }
        }
    }
}

void pie::setAxis(QStringList X, double min_Y, double max_Y, QColor col){
    axisX = new QBarCategoryAxis();
    for(int i=0; i<X.size(); i++){
       axisX->append(X);
    }
    axisX->setLinePenColor(col);
    axisY = new QValueAxis();
    axisY->setRange(min_Y, max_Y);
    axisY->setLinePenColor(col);
    grafico->addAxis(axisX, Qt::AlignBottom);
    grafico->addAxis(axisY, Qt::AlignLeft);

    for(int i=0; i<serie.size(); i++){
        grafico->setAxisX(axisX, serie[i]);
        grafico->setAxisY(axisY, serie[i]);
    }
}

bool pie::getDonut() const{
    return isDonut;
}

void pie::setDonut(bool b){
    if(b) series->setHoleSize(0.35);
    else series->setHoleSize(0);
}


