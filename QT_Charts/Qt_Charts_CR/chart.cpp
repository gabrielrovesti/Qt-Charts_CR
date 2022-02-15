#include <chart.h>

QString Chart::fileIn;
QString Chart::fileOut;
QString Chart::nomeGrafico;
QStringList Chart::labels;
QStringList Chart::values;
QString Chart::struttura;
QHash<QString, QHash<QString, QStringList>> Chart::grafici;
QHash<QString, QHash<QString, QStringList>> Chart::options;
QList<QAbstractSeries*> Chart::serie;

Chart::Chart(){
    grafico = new QChart();
    axisX = new QBarCategoryAxis();
    axisY = new QValueAxis();
}

Chart::Chart(const Chart& c){
    if(this != &c){
        labels=c.labels;
        values=c.values;
        nomeGrafico=c.nomeGrafico;
        options=c.options;
        grafici=c.grafici;
    }
}

Chart::Chart(QString nome, QStringList l, QStringList v){
    nomeGrafico = nome;
    labels = l;
    values = v;
}

QList<QAbstractSeries*> Chart::getSeries() const{
    return serie;
}

QStringList Chart::getLabels() const{
    return labels;
}

void Chart::svuotaGrafico(){
    serie.clear();
    labels.clear();
    values.clear();
    grafici.clear();
}

void Chart::setOption(Chart *c, QHash<QString, QStringList> value){
    QHash<QString, QStringList> d;

    if(value["isCiambella"].isEmpty()){
        value.insert("isCiambella", {"0"});
    }

    d.insert("animazioni", value["animazioni"]);
    d.insert("asseX", value["asseX"]);
    d.insert("antialiasing", value["antialiasing"]);
    d.insert("tema", value["tema"]);
    d.insert("mostralegenda", value["mostralegenda"]);
    d.insert("isCiambella", value["isCiambella"]);
    d.insert("asseX", value["asseX"]);

    options.insert(getTipoGrafico(c), d);
}

QHash<QString, QHash<QString, QStringList>> Chart::getOptions(){
    return options;
}

QHash<QString, QHash<QString, QStringList>> Chart::getGraficiArray(){
    return grafici;
}

QStringList Chart::getValues() const{
    return values;
}

QChart* Chart::getGrafico() const{
    return grafico;
}

QString Chart::getFileInput(){
    return fileIn;
}
QString Chart::getFileOutput(){
    return fileOut;
}

void Chart::importa(QString fileInput){
    QFile f(fileInput);
    if (f.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        fileIn = fileInput;
        struttura = f.readAll();
    }
}

void Chart::setNomeGrafico(QString nome){
    nomeGrafico=nome;
}

QString Chart::getNomeGrafico() const{
    return nomeGrafico;
}

QString Chart::getTipoGrafico(Chart *c){
    const auto& ris = *c;
    QString s = typeid(ris).name();
    return s.remove(QRegExp("[0-9\n]"));
}

void Chart::esporta(QString fileInput){
    QString ext = QFileInfo(fileInput).suffix();
    if(!fileInput.isEmpty() && !fileInput.isNull()){
        if(ext == "json"){
           QJsonObject root, recordObject, Data;
           QList<QString> keys = grafici.keys();

           for(int k=0; k<keys.size(); k++){
               QJsonArray dataArray;
               QStringList l = grafici[keys[k]].value("labels");
               QStringList v = grafici[keys[k]].value("values");
               for(int i=0; i<l.size(); i++){
                    QJsonObject Data;

                    Data.insert("label", l[i]);
                    QJsonArray Data2;
                    for(int j=0; j<v.size()/l.size(); j++){
                        Data2.push_back(v[(i*(v.size()/l.size()))+j]);
                    }
                    Data.insert("values", Data2);
                    dataArray.push_back(Data);
               }

               QJsonArray dataArray2;
               QStringList keysOptions = options[keys[k]].keys();

               for(int i=0; i<keysOptions.size(); i++){
                    QJsonObject Data;
                    QJsonArray Data2;

                    for(int j=0; j<options[keys[k]].value(keysOptions[i]).size(); j++){
                        Data2.push_back(options[keys[k]].value(keysOptions[i])[j]);
                    }

                    Data.insert(keysOptions[i], Data2);
                    dataArray2.push_back(Data);
               }
               recordObject.insert("data", dataArray);
               recordObject.insert("options", dataArray2);
               recordObject.insert("nome", grafici[keys[k]].value("nome")[0]);
               root.insert(keys[k], recordObject);
           }
           QByteArray ba = QJsonDocument(root).toJson();
           QTextStream ts(stdout);

           QFile fout(fileInput);
           fout.open(QIODevice::WriteOnly);
           fout.write(ba);
           fout.close();
        }
        else{
        QFile file(fileInput);
        file.open(QIODevice::WriteOnly);
        QXmlStreamWriter xmlWriter;
        xmlWriter.setDevice(&file);
        xmlWriter.setAutoFormatting(true);
        xmlWriter.writeStartDocument();
        xmlWriter.writeStartElement("grafici");

        QList<QString> keys = grafici.keys();
        for(int k=0; k<keys.size(); k++){
            QStringList l = grafici[keys[k]].value("labels");
            QStringList v = grafici[keys[k]].value("values");

            xmlWriter.writeStartElement("grafico");
            xmlWriter.writeAttribute("nome", grafici[keys[k]].value("nome")[0]);
            xmlWriter.writeAttribute("type", keys[k]);

            xmlWriter.writeStartElement("data");
            QString s;
            for(int i=0; i<l.size(); i++){
                xmlWriter.writeStartElement("labels");
                 xmlWriter.writeAttribute("value", l[i]);
                 xmlWriter.writeEndElement();

                 xmlWriter.writeStartElement("values");
                 for(int j=0; j<v.size()/l.size(); j++){
                      xmlWriter.writeStartElement("value");

                      s = "";
                      s = (v[(i*(v.size()/l.size()))+j]);

                      xmlWriter.writeAttribute("value", s);
                      xmlWriter.writeEndElement();
                 }
                 xmlWriter.writeEndElement();
            }

            xmlWriter.writeEndElement();
            xmlWriter.writeStartElement("options");

            QStringList keysOptions = options[keys[k]].keys();
            for(int i=0; i<keysOptions.size(); i++){
                 s= "";
                 for(int j=0; j<options[keys[k]].value(keysOptions[i]).size(); j++){
                    s += (options[keys[k]].value(keysOptions[i])[j]);
                 }

                 xmlWriter.writeStartElement("option");

                 if(keysOptions[i]=="asseX"){
                     int size=s.size();
                     QString sz = QString::number(size);
                     xmlWriter.writeAttribute(keysOptions[i], sz);
                     for(int i=0; i<s.size(); i++){
                         xmlWriter.writeStartElement("asseX");
                         xmlWriter.writeAttribute("value", s.at(i));
                         xmlWriter.writeEndElement();
                     }
                     xmlWriter.writeEndElement();
                 }
                 else{
                     xmlWriter.writeAttribute(keysOptions[i], s);
                     xmlWriter.writeEndElement();
                 }
            }
        }
        xmlWriter.writeEndElement();
        xmlWriter.writeEndDocument();

        file.flush();
        file.close();
    }
}
else throw file_exceptions("Errore nell'esportazione del file");
}

void Chart::setValues(int position, QString x){
    values[position] = x;
}

void Chart::salvaGrafico(Chart * c){
    QHash<QString, QStringList> dati;
    dati.insert("nome", {c->getNomeGrafico()});
    dati.insert("values", c->getValues());
    dati.insert("labels", c->getLabels());

    grafici.insert(getTipoGrafico(c), dati);
}

bool Chart::isEmpty(){
    return grafici.isEmpty() && labels.isEmpty() && values.isEmpty() && options.isEmpty();
}

void Chart::pulisciSerie(){
    serie.clear();
}

void Chart::rimuoviUltimaSerie(){
    serie.removeLast();
    int size = values.size()/labels.size();
    for(int i=0; i<size; i++){
        values.removeLast();
    }
    labels.removeLast();
}

void Chart::insertLabel(QString s){
    labels.append(s);
}

void Chart::setLabel(int i, QString s){
    labels.replace(i, s);
}

void Chart::insertAt(int position, QString x, bool everyPosition){
    QStringList a;
    if(!everyPosition){
        for(int i=0; i<position; i++){
            a.append(values[i]);
        }
        a.append(x);
        for(int i=position+1; i<values.size()+1; i++){
            a.append(values[i-1]);
        }
    }
    else{
        int cont = 0;
        int contt = position;
        if(values.size() != position){
            for(int i=0; i<values.size()+labels.size(); i++){
                if(contt == i){
                    a.append(x);
                    contt+=position+1;
                }
                else{
                    a.append(values[cont]);
                    cont++;
                }
            }
        }
        else{
            a = values;
            a.append(x);
        }
    }
    values = a;
}

void Chart::remove(int position){
    if(!values.isEmpty()){
        values.removeAt(position);
    }
    else throw logical_exceptions("Errore nella rimozione del valore dal grafico");
}

QString Chart::getStruttura(){
    return struttura;
}

void Chart::clean(){
    values.clear();
    labels.clear();
}

bool Chart::operator==(const Chart& c) const{
    return typeid(*this) == typeid(c) && labels==c.labels && values==c.values && nomeGrafico==c.nomeGrafico;
}

QStringList Chart::getAxisX() const{
    return axisX->categories();
}

QList<qreal> Chart::getAxisY() const{
    QList<qreal> a;
    a.append(axisY->min());
    a.append(axisY->max());
    return a;
}

void Chart::aggiungiValoreAsseX(QString s){
    axisX->append(s);
}

void Chart::rimuoviValoreAsseX(QString x){
    if(axisX->count()>0){
        axisX->remove(x);
    }
    else throw logical_exceptions("Errore nella rimozione del valore dall'asse");
}

int Chart::getMinValue() const{
    int min=values.at(0).toInt();
    for(int i=1; i<values.size(); i++){
        if(values[i].toInt() < min)
        min=values[i].toInt();
    }
    return min;
}

int Chart::getMaxValue() const{
    int max=values.at(0).toInt();
    for(int i=1; i<values.size(); i++){
        if(values[i].toInt() > max)
        max=values[i].toInt();
    }
    return max;
}

void Chart::setRangeAsseY(int x, int y){
    if(!axisY) {
        axisY=new QValueAxis();
        grafico->setAxisY(axisY, serie.at(0));
        grafico->addAxis(axisY, Qt::AlignBottom);
    }
    axisY->setRange(x, y);
}
