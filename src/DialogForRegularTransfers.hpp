#ifndef DIALOGFOREGULARTRANSFERS_HPP
#define DIALOGFOREGULARTRANSFERS_HPP

#include "DialogForTransfers.hpp"

#include <QString>
#include <QModelIndex>

class QLineEdit;
class QDoubleSpinBox;
class QSpinBox;
class QGridLayout;
class QComboBox;
class QTableView;
class QSqlRelationalTableModel;

class DialogForRegularTransfers : public DialogForTransfers
{
    Q_OBJECT

public:
    explicit DialogForRegularTransfers(QWidget * parent = nullptr, QString tagsTableName = "", int transferId = -1);

    QString transferName() const;
    int transferValue() const;
    int dayToCountTransfer() const;

    void setTransferName(QString name);
    void setTransferValue(int value);
    void setDayToCountTransfer(int day);

public slots:
    virtual void accept() override;

    void addTag(QComboBox * tagsList, int transferId);
    void removeTag(QModelIndex index);

private:
    QSqlRelationalTableModel * createModel(const QString &tagsTableName, int transferId);
    QTableView * createView(QSqlRelationalTableModel * model);
    void createTagsTable(const QString & tagsTableName, int transferId);
    QComboBox * createTagsList();

private:
    QLineEdit * nameWidget;
    QDoubleSpinBox * valueWidget;
    QSpinBox * dayWidget;
    QGridLayout * layout;
    QSqlRelationalTableModel * model;
};

#endif // DIALOGFOREGULARTRANSFERS_HPP
