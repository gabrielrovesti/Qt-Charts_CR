#include "bar.h"

bar::bar(QString nome, QStringList x, double min_Y, double max_Y, QColor col){
    minY = min_Y;
    maxY = max_Y;
    nomeGrafico = nome;

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

    series = new QBarSeries();
    grafico->setTitle(nomeGrafico);
}

void bar::insert(QString l, QStringList v){
   labels.append(l);
   values.append(v);
   QBarSet* barlabels = new QBarSet(l);
   for (int i=0; i<v.size(); i++)
        *barlabels << v[i].toDouble();
   series->append(barlabels);

   finalizza();
}

void bar::reloadGrafico(){
    grafico->removeSeries(series);

    series = new QBarSeries();
    serie.clear();

    for(int k=0; k<labels.size(); k++){
        QBarSet* barlabels = new QBarSet(labels[k]);
        for (int i=0; i<values.size()/labels.size(); i++)
             *barlabels << values[k*(values.size()/labels.size())+i].toDouble();
        series->append(barlabels);
    }
    finalizza();
}

void bar::finalizza(){
    if(!grafico->series().isEmpty()){
        grafico->removeSeries(series);
    }
    grafico->addSeries(series);
    serie.push_back(series);
    serie[0]->attachAxis(axisX);
    serie[0]->attachAxis(axisY);

    salvaGrafico(this);
}

void bar::setAxis(QStringList x, double min_Y, double max_Y, QColor col){
    grafico->removeAxis(axisX);
    grafico->removeAxis(axisY);

    axisX = new QBarCategoryAxis();
    axisX->setTitleText("Asse X");
    axisX->append(x);
    axisX->setLinePenColor(col);

    axisY = new QValueAxis();
    axisY->setTitleText("Asse Y");
    axisY->setRange(min_Y, max_Y);
    axisY->setLinePenColor(col);

    grafico->addAxis(axisX, Qt::AlignBottom);
    grafico->addAxis(axisY, Qt::AlignLeft);

    for(int i=0; i<serie.size(); i++){
        serie[i]->attachAxis(axisX);
        serie[i]->attachAxis(axisY);
    }
}
