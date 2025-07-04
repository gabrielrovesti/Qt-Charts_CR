#include "line.h"

line::line(QString nome, QStringList x, double min_Y, double max_Y, QColor col){
    minY = min_Y;
    maxY = max_Y;
    nomeGrafico = nome;
    grafico->setTitle(nomeGrafico);

    if(!x.isEmpty()){
        axisX = new QBarCategoryAxis();
        axisX->append(x);
        axisX->setLinePenColor(col);
        grafico->addAxis(axisX, Qt::AlignBottom);
    }
    else{
        axisX = new QBarCategoryAxis();
        axisX->setLinePenColor(col);
        grafico->addAxis(axisX, Qt::AlignBottom);
    }
    axisX->setTitleText("Asse X");
    axisY = new QValueAxis();
    axisY->setTitleText("Asse Y");
    axisY->setRange(minY, maxY);
    axisY->setLinePenColor(col);
    grafico->addAxis(axisY, Qt::AlignLeft);

    grafico->setTitle(nomeGrafico);

}

void line::insert(QString l, QStringList v){
    values.append(v);
    labels.append(l);
    series = new QLineSeries();
    series->setName(l);
    int count = 0;
    for(int i=0; i<v.size(); i++){
        series->append(count, v[i].toDouble());
        count++;
    }

    grafico->addSeries(series);
    serie.push_back(series);

    finalizza();
}

void line::reloadGrafico(){
    for(int i=0; i<serie.size(); i++){
        grafico->removeSeries(serie[i]);
    }
    serie.clear();

    for(int j=0; j<labels.size(); j++){
        series = new QLineSeries();
        series->setName(labels[j]);
        int count = 0;
        for(int i=0; i<values.size()/labels.size(); i++){
            series->append(count, values[i].toDouble());
            count++;
        }
        grafico->addSeries(series);
        serie.push_back(series);
    }
    finalizza();
}

void line::setAxis(QStringList X, double min_Y, double max_Y, QColor col){
    grafico->removeAxis(axisX);
    grafico->removeAxis(axisY);

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
        serie[i]->attachAxis(axisX);
        serie[i]->attachAxis(axisY);
    }
}

void line::finalizza(){
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

    salvaGrafico(this);
}
