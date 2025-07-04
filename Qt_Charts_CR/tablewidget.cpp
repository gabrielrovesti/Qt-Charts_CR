#include "tablewidget.h"

QT_CHARTS_USE_NAMESPACE

TableWidget::TableWidget(Chart *c){
    grafico = c;
}

Chart*TableWidget::getChartDisplayed() const{
    if(grafico) return grafico;
    else throw graphical_exceptions("Table");
}

void TableWidget::createChartDisplayed(){
    m = new Mapper(grafico);
    tableView = new QTableView();
    tableView->setModel(m);
    tableView->setMinimumWidth(300);
    tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    tableView->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    m->setParent(tableView);

    if(dynamic_cast<line*>(grafico)){
        int cont = 0;
            for(int i=0; i<grafico->getSeries().size(); i++){
            QVXYModelMapper *mapper = new QVXYModelMapper;
            mapper->setXColumn(cont);
            cont++;
            mapper->setYColumn(cont);
            cont++;
            mapper->setSeries(dynamic_cast<QLineSeries*>(grafico->getSeries().at(i)));
            mapper->setModel(m);
            setSeriesMapper();
        }
    }
    if(dynamic_cast<CandleStick*>(grafico)){
        int cont = 0;
        if(grafico->getSeries().size() > 0){
            QVCandlestickModelMapper *mapper = new QVCandlestickModelMapper;
            mapper->setFirstSetColumn(0);
            mapper->setLastSetColumn(m->columnCount()-1);
            mapper->setTimestampRow(cont);
            cont++;
            mapper->setOpenRow(cont);
            cont++;
            mapper->setHighRow(cont);
            cont++;
            mapper->setLowRow(cont);
            cont++;
            mapper->setCloseRow(cont);
            cont++;
            mapper->setSeries(dynamic_cast<QCandlestickSeries*>(grafico->getSeries().at(0)));
            mapper->setModel(m);
        }
    }
    if(dynamic_cast<pie*>(grafico)){
        if(grafico->getSeries().size() > 0){
            QHPieModelMapper *mapper = new QHPieModelMapper;
            mapper->setFirstColumn(0);
            mapper->setColumnCount(m->columnCount());
            mapper->setLabelsRow(0);
            mapper->setValuesRow(0);
            mapper->setSeries(dynamic_cast<QPieSeries*>(grafico->getSeries().at(0)));
            setSeriesMapper();
            mapper->setModel(m);
        }
    }
    if(dynamic_cast<bar*>(grafico)){
        if(grafico->getSeries().size() > 0){
            QVBarModelMapper *mapper = new QVBarModelMapper;
            mapper->setFirstBarSetColumn(0);
            mapper->setLastBarSetColumn(m->columnCount()-1);
            mapper->setSeries(dynamic_cast<QBarSeries*>(grafico->getSeries().at(0)));
            mapper->setModel(m);
            setSeriesMapper();
        }
    }
    chartView = new QChartView(grafico->getGrafico());
    chartView->setRenderHint(QPainter::Antialiasing);
    chartView->setMinimumSize(640, 480);
}

void TableWidget::setSeriesMapper(QString tipoOperazione){
    if(dynamic_cast<line*>(grafico)){
        int cont2 = 0;
        for(int i=0; i<grafico->getSeries().size(); i++){
            QString seriesColorHex = "#000000";
            seriesColorHex = "#" + QString::number(dynamic_cast<QLineSeries*>(grafico->getSeries().at(i))->pen().color().rgb(), 16).right(6).toUpper();
            m->addMapping(seriesColorHex, QRect(cont2, 0, 2, m->rowCount()));
            cont2+=2;
        }
    }
    if(dynamic_cast<bar*>(grafico)){
        QList<QBarSet *> barsets = dynamic_cast<QBarSeries*>(grafico->getSeries().at(0))->barSets();
        for (int i = 0; i < barsets.count(); i++) {
            QString seriesColorHex = "#000000";
            seriesColorHex = "#" + QString::number(barsets.at(i)->brush().color().rgb(), 16).right(6).toUpper();
            m->addMapping(seriesColorHex, QRect(i, 0, 1, barsets.at(i)->count()));
        }
        barsets.clear();
    }
    if(dynamic_cast<pie*>(grafico)){
        QList<QPieSlice *> slices = dynamic_cast<QPieSeries*>(grafico->getSeries().at(0))->slices();
        for(int i=0; i<slices.count(); i++){
            QString seriesColorHex = "#000000";
            seriesColorHex = "#" + QString::number(slices.at(i)->brush().color().rgb(), 16).right(6).toUpper();
            m->addMapping(seriesColorHex, QRect(i, 0, 1, 1));
        }
        slices.clear();
    }
    if(tipoOperazione == "addrow"){
        int div=1;
        if(grafico->getValues().size() > 0 && grafico->getLabels().size() > 0)
            div=grafico->getValues().size()/grafico->getLabels().size();
        grafico->insertAt(div, "0", true);
        grafico->reloadGrafico();
        createChartDisplayed();
    }
    else if(tipoOperazione == "addcol"){
        int val = grafico->getValues().size();
        for(int i=0; i<m->rowCount(); i++){
            if(dynamic_cast<CandleStick*>(grafico) && i == 0){
                grafico->insertAt(val+i, grafico->getLabels().at(i), true);
            }
            else{
                grafico->insertAt(val+i, "0", true);
            }
        }
        grafico->reloadGrafico();
        createChartDisplayed();
    }
    else if(tipoOperazione == "removerow"){
        int cont = 0;
        int incremento = (grafico->getValues().size()/grafico->getLabels().size())-1;
        int attualeIncremento = incremento;
        int size = grafico->getValues().size()-grafico->getLabels().size()+1;
        while(cont < size && attualeIncremento <= grafico->getValues().size()){
            grafico->remove(attualeIncremento);
            attualeIncremento+=incremento;
            cont++;
        }
        grafico->reloadGrafico();
        createChartDisplayed();
    }
    else if(tipoOperazione == "removecol"){
        grafico->rimuoviUltimaSerie();
        grafico->reloadGrafico();
        createChartDisplayed();
    }
}

Mapper*TableWidget::getMapper() const{
    if(m) return m;
    else throw graphical_exceptions("Table");
}

QTableView* TableWidget::getTableWidget() const{
    if(tableView) return tableView;
    else throw graphical_exceptions("Table");
}

QChartView* TableWidget::getChartDisplayedWidget() const{
    if(chartView) return chartView;
    else throw graphical_exceptions("Table");
}

void TableWidget::getDataFromFile(){
    QString ext = QFileInfo(Chart::getFileInput()).suffix();

    QHash<QString, Chart *> _map;

    _map.insert(getLineClass(), new line("Line Chart"));
    _map.insert(getBarClass(), new bar("Bar Chart"));
    _map.insert(getCandleStickClass(), new CandleStick("CandleStick Chart"));
    _map.insert(getPieClass(), new pie("Pie Chart"));

    if(ext == "json"){
    QJsonParseError parseError;
    QJsonDocument doc2;

    QByteArray ba2 = Chart::getStruttura().toUtf8();
    doc2 = QJsonDocument::fromJson(ba2, &parseError);

    if (parseError.error != QJsonParseError::NoError) {
        throw file_exceptions("JSON");
    } else {
            QJsonDocument document = QJsonDocument::fromJson(ba2);
            QJsonObject object = document.object();
            QStringList tipoGrafico = object.keys();
            Chart::svuotaGrafico();

            for(int k=0; k<tipoGrafico.size(); k++){
                QJsonObject res = object[tipoGrafico[k]].toObject();

                QJsonDocument doc(res);
                QString strJson(doc.toJson(QJsonDocument::Compact));

                if(strJson == "{}"){
                   throw file_exceptions("JSON");
                }
                auto it = _map.value(tipoGrafico[k]);

                QJsonValue valuee2 = res.value("options");
                QJsonArray array2 = valuee2.toArray();
                QStringList values2;

                QHash<QString, QStringList> options;

                foreach (const QJsonValue & value, array2){
                    QJsonObject obj = value.toObject();
                    QStringList a;
                    values2.clear();
                    QJsonArray array = obj[obj.keys().at(0)].toArray();
                    foreach (const QJsonValue &i, array){
                        a.append(i.toString());
                    }

                    options[obj.keys().at(0)].append(a);
                }

                Chart::setOption(it, options);
                auto op = Chart::getOptions();

                QJsonValue valuee = res.value("data");
                QJsonArray array = valuee.toArray();
                QString label;
                QStringList values;

                foreach (const QJsonValue & value, array) {
                    QJsonObject obj = value.toObject();
                    label = obj["label"].toString();
                    values.clear();
                    QJsonArray array = obj["values"].toArray();
                     foreach (const QJsonValue &i, array){
                        values.append(i.toString());
                    }
                    it->insert(label, values);
                }

                it->setNomeGrafico(res.value("nome").toString());
                Chart::salvaGrafico(it);

                it->setAxis(op[tipoGrafico[k]].value("asseX"));
                it->clean();
                it->pulisciSerie();
            }
        }
    }
    else{
    QFile file(Chart::getFileInput());

    Chart::svuotaGrafico();
    QDomDocument q;

    if(!file.open(QIODevice::ReadOnly | QIODevice::Text))
        throw file_exceptions("XML");
    else{
        if(!q.setContent(&file)) throw file_exceptions("XML");
        file.close();
    }

    QDomElement root = q.documentElement();
    QDomNodeList nodes = root.elementsByTagName("grafico");

    for(int k=0; k<nodes.count(); k++){
        QDomElement s = nodes.at(k).toElement();
        auto it = _map.value(s.attribute("type"));

        QString label;
        QStringList values;
        QHash<QString, QStringList> options_hash;

        it->setNomeGrafico(s.attribute("nome"));
        QDomElement data = s.firstChildElement();

        if(data.tagName()=="data"){
            QDomNodeList ChildData = data.childNodes();
            int j = 0;
            while(j < ChildData.count()){
                 label.clear();
                 if(ChildData.at(j).toElement().tagName() == "labels"){
                     label.append(ChildData.at(j).toElement().attribute("value"));
                 }
                 j++;
                 QDomElement next=ChildData.at(j).toElement();
                 if(next.tagName() == "values"){
                     values.clear();
                     QDomNodeList child_values_list=next.childNodes();
                     for(int i=0; i<child_values_list.count(); i++){
                         values.append(child_values_list.at(i).toElement().attribute("value"));
                     }
                 }
                 it->insert(label, values);
                 j++;
            }
        }

        QDomElement opzioni = data.nextSiblingElement();
        QStringList string_axis;

        if(opzioni.tagName()=="options"){
            QDomNodeList ChildOptions = opzioni.childNodes();
             int i=0, c=0;
             while(i<ChildOptions.count()){
                 QDomNamedNodeMap map = ChildOptions.at(i).attributes();
                  for(int j = 0; j < map.length(); ++j){
                       if(!(map.item(j).isNull())){
                           QDomNode debug = map.item(j);
                           QDomAttr attr = debug.toAttr();
                           if(!attr.isNull()){
                               if(attr.name() == "asseX"){
                                   QDomNodeList ChildAxis = ChildOptions.at(i).childNodes();
                                   while(c<ChildAxis.count()){
                                       string_axis.append(ChildAxis.at(c).toElement().attribute("value"));
                                       string_axis.join(",");
                                       c++;
                                   }
                                   string_axis.back().remove(",");
                                   options_hash.insert(debug.nodeName(), string_axis);
                               }
                               else options_hash.insert(debug.nodeName(), {debug.nodeValue()});
                          }
                      }
                 }
                 Chart::setOption(it, options_hash);
                 i++;
             }
        }
        Chart::salvaGrafico(it);
        it->setAxis(string_axis);
        it->clean();
        it->pulisciSerie();
        }
    }
}
