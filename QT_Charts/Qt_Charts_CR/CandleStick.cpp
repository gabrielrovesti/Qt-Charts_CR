#include "CandleStick.h"

CandleStick::CandleStick(QString nome, QStringList x, double min_Y, double max_Y, QColor col){
    minY = min_Y;
    maxY = max_Y;
    nomeGrafico = nome;
    grafico->setTitle(nomeGrafico);
    grafico->createDefaultAxes();

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

    series = new QCandlestickSeries();
    series->setName(nome);
    series->setIncreasingColor(QColor(Qt::green));
    series->setDecreasingColor(QColor(Qt::red));
}

void CandleStick::insert(QString l, QStringList v){
    labels.append(l);
    values.append(l);
    values.append(v);

    QCandlestickSet *candlestickSet = new QCandlestickSet();
    candlestickSet->setOpen(v[0].toDouble());
    candlestickSet->setHigh(v[1].toDouble());
    candlestickSet->setLow(v[2].toDouble());
    candlestickSet->setClose(v[3].toDouble());

    series->append(candlestickSet);
    finalizza();
}

void CandleStick::setNomeSerie(QString s){
    series->setName(s);
    nomeSerie = s;
}

void CandleStick::reloadGrafico(){
    grafico->removeSeries(series);
    series = new QCandlestickSeries();
    series->setName(nomeSerie);
    series->setIncreasingColor(QColor(Qt::green));
    series->setDecreasingColor(QColor(Qt::red));
    serie.clear();

    for(int i=0; i<labels.size(); i++){
        QCandlestickSet *candlestickSet = new QCandlestickSet();
        candlestickSet->setOpen(values[(i*5)+1].toDouble());
        candlestickSet->setHigh(values[(i*5)+2].toDouble());
        candlestickSet->setLow(values[(i*5)+3].toDouble());
        candlestickSet->setClose(values[(i*5)+4].toDouble());
        series->append(candlestickSet);
    }
    finalizza();
}

void CandleStick::finalizza(){
    if(!grafico->series().isEmpty()){
        grafico->removeSeries(series);
    }
    grafico->addSeries(series);
    serie.push_back(series);

    grafico->createDefaultAxes();

    axisX = qobject_cast<QBarCategoryAxis *>(grafico->axes(Qt::Horizontal).at(0));
    axisX->setCategories(labels);

    axisY = qobject_cast<QValueAxis *>(grafico->axes(Qt::Vertical).at(0));
    axisY->setMax(maxY);
    axisY->setMin(minY);

    salvaGrafico(this);
}

void CandleStick::setAxis(QStringList x, double min_Y, double max_Y, QColor col){
    axisX = new QBarCategoryAxis();
    axisX->setTitleText("Asse X");
    axisX->append(x);
    axisX->setLinePenColor(col);

    axisY = new QValueAxis();
    axisY->setTitleText("Asse Y");
    axisY->setMin(min_Y);
    axisY->setMax(max_Y);
    axisY->setLinePenColor(col);

}
