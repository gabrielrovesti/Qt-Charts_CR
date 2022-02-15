#ifndef CHART_H
#define CHART_H

#include <QtCharts>
#include <QtXml>
#include <QObject>
#include <bits/stdc++.h>
#include "exceptions.h"

class Chart{
protected:
    static QString nomeGrafico;
    static QStringList labels, values;
    static QString struttura;
    QChart* grafico = nullptr;
    static QList<QAbstractSeries*> serie;
    static QHash<QString, QHash<QString, QStringList>> grafici;
    static QHash<QString, QHash<QString, QStringList>> options;
    static QString fileIn;
    static QString fileOut;
    QBarCategoryAxis *axisX=nullptr;
    QValueAxis *axisY=nullptr;
    int minY=0, maxY=0;
public:
    //Costruttori
    Chart(QString, QStringList, QStringList);
    Chart(const Chart&);
    Chart();

    //Metodi di modifica ed inserimento
    virtual void insert(QString, QStringList) = 0;
    void insertAt(int, QString, bool = false);
    static void setValues(int, QString);
    virtual void finalizza()=0;
    virtual void setAxis(QStringList, double = 0, double = 20, QColor = "black")=0;
    virtual void reloadGrafico() = 0;
    static void setOption(Chart*, QHash<QString, QStringList>);
    void setNomeGrafico(QString);
    bool isEmpty();

    //Metodi di impostazione/settings
    static void setLabel(int, QString);
    static void salvaGrafico(Chart*);
    static void svuotaGrafico();
    void insertLabel(QString);
    void rimuoviUltimaSerie();
    void remove(int);
    virtual Chart* clone() const=0;
    virtual bool operator==(const Chart&) const;
    void aggiungiValoreAsseX(QString);
    void rimuoviValoreAsseX(QString);
    void setRangeAsseY(int, int);
    void pulisciSerie();

    //Metodi raccolta dati
    static QHash<QString, QHash<QString, QStringList>> getOptions();
    static QHash<QString, QHash<QString, QStringList>> getGraficiArray();
    QList<QAbstractSeries*> getSeries() const;
    QChart* getGrafico() const;
    QStringList getLabels() const;
    QStringList getValues() const;
    int getMinValue() const;
    int getMaxValue() const;
    QString getNomeGrafico() const;
    static QString getTipoGrafico(Chart *);
    static QString getStruttura();
    QStringList getAxisX() const;
    QList<qreal> getAxisY() const;

    //Metodi gestione file
    static void importa(QString);
    static void esporta(QString);
    static QString getFileInput();
    static QString getFileOutput();

    //Pulizia e distruttore
    void clean();
    virtual~Chart(){
        nomeGrafico.clear();
        values.clear();
        labels.clear();
        grafico->removeAllSeries();
        delete axisX;
        delete axisY;
        delete grafico;
    };
};

#endif // CHART_H
