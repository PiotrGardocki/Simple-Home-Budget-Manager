#include "CurrencyProxyModel.hpp"

CurrencyProxyModel::CurrencyProxyModel(QObject *parent)
    : QIdentityProxyModel(parent)
{

}

void CurrencyProxyModel::setCurrencySuffix(const QString & suffix)
{
    currencySuffix = suffix;
}

void CurrencyProxyModel::setCurrencyColumn(int column)
{
    currencyColumn = column;
}

QVariant CurrencyProxyModel::data(const QModelIndex & index, int role) const
{
    QVariant modelData = QIdentityProxyModel::data(index, role);

    if (role == Qt::DisplayRole && index.column() == currencyColumn)
    {
        int value = modelData.toInt();
        int afterCommaValue = value % 100;
        int beforeCommaValue = value - afterCommaValue;
        beforeCommaValue /= 100;

        QString stringValue = QString("%1,%2 %3").
                              arg(beforeCommaValue).
                              arg(afterCommaValue, 2, 10, QChar('0')).
                              arg(currencySuffix);

        modelData = stringValue;
    }

    return modelData;
}
