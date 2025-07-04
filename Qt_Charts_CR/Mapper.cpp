#include "mapper.h"

Mapper::Mapper(Chart *c, QObject *parent) : QAbstractTableModel(parent), grafico(c){
    if(m_columnCount < -1 || m_rowCount < -1) throw graphical_exceptions("Mapper");

    if(dynamic_cast<bar*>(grafico) || dynamic_cast<CandleStick*>(grafico)){
        int div=0;
        m_columnCount = c->getLabels().size();

        if(c->getValues().size()>0 && c->getLabels().size()>0){
            m_rowCount = c->getValues().size();
            div = m_rowCount/m_columnCount;
        }
        else{
            m_rowCount=0;
        }

        for (int i = 0; i < div; i++) {
            QVector<qreal>* dataVec = new QVector<qreal>(m_columnCount);
            for (int k = 0; k < dataVec->size(); k++) {
                dataVec->replace(k, c->getValues().at(i+(k*div)).toDouble());
            }
            m_data.append(dataVec);
        }
    }
    if(dynamic_cast<pie*>(grafico)){
        m_columnCount = c->getValues().size();
        QVector<qreal>* dataVec = new QVector<qreal>(m_columnCount);
        for (int k = 0; k < dataVec->size(); k++) {
            dataVec->replace(k, c->getValues().at(k).toDouble());
        }
        m_data.append(dataVec);
    }
    if(dynamic_cast<line*>(grafico)){
        int cont = 0, cont2 = 0;
        m_columnCount = c->getLabels().size()*2;
        if(c->getValues().size()>0)
        m_rowCount = c->getValues().size()/c->getLabels().size();
        else
        m_rowCount=0;

        for (int i = 0; i < m_rowCount; i++) {
            QVector<qreal>* dataVec = new QVector<qreal>(m_columnCount);
            for (int k = 0; k < dataVec->size(); k++) {
                if(k % 2 == 0){
                    dataVec->replace(k, cont);
                }
                else{
                    dataVec->replace(k, c->getValues().at(i+cont2).toDouble());
                    cont2+=(m_rowCount);
                }
             }
            cont++;
            cont2=0;
            m_data.append(dataVec);
        }
    }
}

int Mapper::rowCount(const QModelIndex &parent) const{
    Q_UNUSED(parent)
    return parent.isValid() ? 0 : m_data.count();
}

int Mapper::columnCount(const QModelIndex &parent) const{
    Q_UNUSED(parent)
    return parent.isValid() ? 0 : m_columnCount;
}

QVariant Mapper::headerData(int section, Qt::Orientation orientation, int role) const{
    if (role != Qt::DisplayRole)
        return QVariant();
    if(dynamic_cast<CandleStick*>(grafico)){
        if (orientation == Qt::Vertical) {
            if (section % 5 == 0){
                return "time";
            }
            else if(section % 5 == 1){
                return "open";
            }
            else if(section % 5 == 2){
                return "high";
            }
            else if(section % 5 == 3){
                return "low";
            }
            else if(section % 5 == 4){
                return "close";
            }
        } else {
            if(section < grafico->getLabels().size()){
                return grafico->getLabels().at(section);
            }
        }
    }
    if(!dynamic_cast<line*>(grafico)){
        return QString("%1").arg(section + 1);
    }
    else{
        if (orientation == Qt::Horizontal) {
            if (section % 2 == 0)
                return "x";
            else
                return "y";
        } else {
            return QString("%1").arg(section + 1);
        }
    }
    return 0;
}

QVariant Mapper::data(const QModelIndex &index, int role) const{
    if (role == Qt::DisplayRole) {
        return m_data[index.row()]->at(index.column());
    } else if (role == Qt::EditRole) {
        return m_data[index.row()]->at(index.column());
    } else if (role == Qt::BackgroundRole) {
        for (const QRect &rect : m_mapping) {
            if (rect.contains(index.column(), index.row())){
                return QColor(m_mapping.key(rect));
            }
        }
        return QColor(Qt::white);
    }
    return QVariant();
}

bool Mapper::setData(const QModelIndex &index, const QVariant &value, int role){
    if (index.isValid() && role == Qt::EditRole) {
        m_data[index.row()]->replace(index.column(), value.toDouble());
        if(dynamic_cast<line*>(grafico)){
            if(index.column() % 2 == 1 && index.column() > 0){
                Chart::setValues((index.column()/2)*(grafico->getValues().size()/grafico->getLabels().size())+index.row(), value.toString());
            }
        }
        if(dynamic_cast<bar*>(grafico) || dynamic_cast<pie*>(grafico)){
            Chart::setValues((index.column()*(grafico->getValues().size()/grafico->getLabels().size()))+index.row(), value.toString());
        }
        if(dynamic_cast<CandleStick*>(grafico)){
            if(index.row() == 0){
                Chart::setLabel(index.column(), value.toString());
                grafico->finalizza();
            }
            else{
            Chart::setValues((index.column()*(grafico->getValues().size()/grafico->getLabels().size()))+index.row(), value.toString());
            }
        }
        emit dataChanged(index, index);

        Chart::salvaGrafico(grafico);
        return true;
    }
    return false;
}

Qt::ItemFlags Mapper::flags(const QModelIndex &index) const
{
    if(dynamic_cast<line*>(grafico)){
        if(index.column() % 2 != 0)
          return Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsEditable;
        else
          return Qt::ItemIsSelectable;
    }
    else{
        return Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsEditable;
    }
}

void Mapper::addMapping(QString color, QRect area){
    m_mapping.insertMulti(color, area);
}

bool Mapper::insertRow(int position, int rows, const QModelIndex &parent){
    beginInsertRows(parent, position, position + rows - 1);
    m_columnCount = columnCount();
    m_rowCount+=rows;
    operation_type = "addrow";

    QVector<qreal>* dataVec = new QVector<qreal>(m_columnCount);
    for (int k = 0; k < dataVec->size(); k++) {
         dataVec->replace(k, 0);
    }

    m_data.append(dataVec);
    endInsertRows();
    return true;
}

bool Mapper::removeRow(int position, int rows, const QModelIndex &parent){
    if(position < -1) throw graphical_exceptions("Mapper");
    beginRemoveRows(parent, position, position + rows - 1);
    operation_type = "removerow";
    m_rowCount-=rows;
    endRemoveRows();
    return true;
}

bool Mapper::insertColumn(int position, int columns, const QModelIndex &parent){
    beginInsertColumns(parent, position, position + columns - 1);
    operation_type = "addcol";
    for (int k = 0; k < m_data.count(); k++) {
         m_data[k]->insert(columns, 0);
    }
    m_columnCount+=columns;
    endInsertColumns();
    return true;
}

bool Mapper::removeColumn(int position, int columns, const QModelIndex &parent){
    if(position < -1) throw graphical_exceptions("Mapper");
    beginRemoveColumns(parent, position, position + columns - 1);
    operation_type = "removecol";
    endRemoveColumns();
    m_columnCount-=columns;
    return true;
}

QString Mapper::getOperationType() const{
    return operation_type;
}
