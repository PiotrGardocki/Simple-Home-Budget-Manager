#ifndef CURRENCYPROXYMODEL_HPP
#define CURRENCYPROXYMODEL_HPP

#include <QIdentityProxyModel>

class CurrencyProxyModel : public QIdentityProxyModel
{
    Q_OBJECT

public:
    CurrencyProxyModel(QObject * parent = nullptr);

    void setCurrencySuffix(const QString & suffix);
    void setCurrencyColumn(int column);

    virtual QVariant data(const QModelIndex & index, int role = Qt::DisplayRole) const override;

private:
    int currencyColumn;
    QString currencySuffix;
};

#endif // CURRENCYPROXYMODEL_HPP
