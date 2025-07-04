#ifndef TABLEWIDGET_H
#define TABLEWIDGET_H

#include "mapper.h"
#include "nomiclassi.h"
#include "chart.h"

class TableWidget : public QMainWindow, public nomiClassi{
    Q_OBJECT
public:
    TableWidget(Chart*);
    TableWidget()=default;
    QTableView* getTableWidget() const;
    QChartView* getChartDisplayedWidget() const;
    Mapper* getMapper() const;
    Chart* getChartDisplayed() const;
    void createChartDisplayed();
    void setSeriesMapper(QString = "");
    static void getDataFromFile();
    virtual ~TableWidget(){
        tableView->deleteLater();
        chartView->deleteLater();
    }
private:
    QTableView *tableView = new QTableView();
    QChartView *chartView = new QChartView();
    Mapper *m = nullptr;
    Chart* grafico = nullptr;
};

#endif // TABLEWIDGET_H
