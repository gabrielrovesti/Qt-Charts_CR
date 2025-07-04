#ifndef MAPPER_H
#define MAPPER_H

#include <QtCore>
#include <QtGui>
#include "bar.h"
#include "pie.h"
#include "CandleStick.h"
#include "line.h"

class Mapper : public QAbstractTableModel{
    Q_OBJECT
public:
    Mapper()=default;
    Mapper(Chart *c, QObject *parent = 0);
    virtual ~Mapper(){
        m_mapping.clear();
        qDeleteAll(m_data);
    };

    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    int columnCount(const QModelIndex &parent = QModelIndex()) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole);
    bool insertColumn(int position, int columns, const QModelIndex &parent = QModelIndex());
    bool removeColumn(int position, int columns, const QModelIndex &parent = QModelIndex());
    bool insertRow(int position, int rows, const QModelIndex &parent = QModelIndex());
    bool removeRow(int position, int rows, const QModelIndex &parent = QModelIndex());

    Qt::ItemFlags flags(const QModelIndex &index) const;
    void addMapping(QString color, QRect area);
    QString getOperationType() const;
private:
    QList<QVector<qreal>*> m_data;
    QHash<QString, QRect> m_mapping;
    int m_columnCount = 0;
    int m_rowCount = 0;
    QString operation_type = "";
protected:
    Chart *grafico = nullptr;
};

#endif // MAPPER_H
