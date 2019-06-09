#ifndef TRANSFERSTAB_HPP
#define TRANSFERSTAB_HPP

#include <QWidget>
#include <QModelIndex>
#include <QSqlRecord>

class QGroupBox;
class QVBoxLayout;
class QSqlTableModel;
class QErrorMessage;

class DialogForRegularTransfers;
class DialogForLatestTransfers;
class TableEditBox;

class TransfersTab : public QWidget
{
    Q_OBJECT
public:
    explicit TransfersTab(QWidget *parent = nullptr, bool positiveNumbers = true);

private:
    void createRegularTransfersSection();
    void createLatestTransfersSection();
    TableEditBox * createSection(QSqlTableModel * model, const QString & titlePrefix);

private slots:
    void openAddDialogForRegularTransfers(TableEditBox * section, QSqlTableModel * model);
    void openEditDialogForRegularTransfers(TableEditBox * section, QSqlTableModel * model, QModelIndex index);

    void openAddDialogForLatestTransfers(TableEditBox * section, QSqlTableModel * model);
    void openEditDialogForLatestTransfers(TableEditBox * section, QSqlTableModel * model, QModelIndex index);

private:
    void addDialogForRegularTransfersAccepted(TableEditBox * section, QSqlTableModel * model, DialogForRegularTransfers * dialog);
    void editDialogForRegularTransfersAccepted(TableEditBox * section, QSqlTableModel * model, DialogForRegularTransfers * dialog, QModelIndex index, QSqlRecord record);

    void addDialogForLatestTransfersAccepted(TableEditBox * section, QSqlTableModel * model, DialogForLatestTransfers * dialog);
    void editDialogForLatestTransfersAccepted(TableEditBox * section, QSqlTableModel * model, DialogForLatestTransfers * dialog, QModelIndex index, QSqlRecord record);

    bool checkTransferValue(int value);

private:
    QErrorMessage * errorMessages;
    QVBoxLayout * layout;
    bool positiveNumbers;
};

#endif // TRANSFERSTAB_HPP
