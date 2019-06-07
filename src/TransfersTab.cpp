#include "TransfersTab.hpp"
#include "TableEditBox.hpp"
#include "DialogForRegularTransfers.hpp"
#include "DialogForLatestTransfers.hpp"
#include "CurrencyProxyModel.hpp"

#include <QSqlTableModel>
#include <QSqlField>
#include <QSqlRecord>
#include <QTableView>
#include <QVBoxLayout>
#include <QDate>

#include <cmath>

TransfersTab::TransfersTab(QWidget *parent, bool positiveNumbers)
    : QWidget(parent), positiveNumbers(positiveNumbers)
{
    layout = new QVBoxLayout(this);

    createRegularTransfersSection();
    createLatestTransfersSection();
}

void TransfersTab::createRegularTransfersSection()
{
    QSqlTableModel * model = new QSqlTableModel(this);
    model->setTable("regular_transfers");
    model->setHeaderData(1, Qt::Horizontal, "Transfer name");
    model->setHeaderData(2, Qt::Horizontal, "Value");
    model->setHeaderData(3, Qt::Horizontal, "Day of month to count");

    TableEditBox * regularTransfersSection = createSection(model, "Regular");

    connect(regularTransfersSection, &TableEditBox::openAddDialog, this, &TransfersTab::openAddDialogForRegularTransfers);
    connect(regularTransfersSection, &TableEditBox::openEditDialog, this, &TransfersTab::openEditDialogForRegularTransfers);
}

void TransfersTab::createLatestTransfersSection()
{
    QSqlTableModel * model = new QSqlTableModel(this);
    model->setTable("normal_transfers");
    model->setHeaderData(1, Qt::Horizontal, "Transfer name");
    model->setHeaderData(2, Qt::Horizontal, "Value");
    model->setHeaderData(3, Qt::Horizontal, "Date");

    TableEditBox * normalTransfersSection = createSection(model, "Latest");

    connect(normalTransfersSection, &TableEditBox::openAddDialog, this, &TransfersTab::openAddDialogForLatestTransfers);
    connect(normalTransfersSection, &TableEditBox::openEditDialog, this, &TransfersTab::openEditDialogForLatestTransfers);
}

TableEditBox *TransfersTab::createSection(QSqlTableModel *model, const QString & titlePrefix)
{
    QString filter = positiveNumbers ? QString("transfer_value > 0") : QString("transfer_value < 0");
    model->setFilter(filter);

    CurrencyProxyModel * currencyProxy = new CurrencyProxyModel(this);
    currencyProxy->setCurrencyColumn(2);
    currencyProxy->setCurrencySuffix(QString("zł"));
    currencyProxy->setSourceModel(model);

    QTableView * view = new QTableView(this);
    view->setModel(currencyProxy);
    view->setColumnHidden(0, true);

    QString title = titlePrefix;
    title += positiveNumbers ? QString(" incomes") : QString(" expenses");
    TableEditBox * transfersSection = new TableEditBox(title, model, view, this);
    layout->addWidget(transfersSection);

    return transfersSection;
}

void TransfersTab::openAddDialogForRegularTransfers(TableEditBox * section, QSqlTableModel * model)
{
    DialogForRegularTransfers * dialog = new DialogForRegularTransfers(this);
    dialog->open();

    connect(dialog, &DialogForRegularTransfers::accepted,
            this, [=](){ this->addDialogForRegularTransfersAccepted(section, model, dialog); });
}

void TransfersTab::openEditDialogForRegularTransfers(TableEditBox * section, QSqlTableModel * model, QModelIndex index)
{
    if (!index.isValid())
        return;

    QSqlRecord record = model->record(index.row());

    DialogForRegularTransfers * dialog = new DialogForRegularTransfers(this, "regular_transfers_tags", record.value("id").toInt());

    QString transferName = record.value("transfer_name").toString();
    int transferValue = record.value("transfer_value").toInt();
    int dayToCountTransfer = record.value("month_day_to_count_transfer").toInt();

    dialog->setTransferName(transferName);
    dialog->setTransferValue(std::abs(transferValue));
    dialog->setDayToCountTransfer(dayToCountTransfer);

    dialog->open();

    connect(dialog, &DialogForRegularTransfers::accepted,
            this, [=](){ this->editDialogForRegularTransfersAccepted(section, model, dialog, index, record); });
}

void TransfersTab::openAddDialogForLatestTransfers(TableEditBox * section, QSqlTableModel * model)
{
    DialogForLatestTransfers * dialog = new DialogForLatestTransfers(this);
    dialog->open();

    connect(dialog, &DialogForLatestTransfers::accepted,
            this, [=](){ this->addDialogForLatestTransfersAccepted(section, model, dialog); });
}

void TransfersTab::openEditDialogForLatestTransfers(TableEditBox * section, QSqlTableModel * model, QModelIndex index)
{
    if (!index.isValid())
        return;

    QSqlRecord record = model->record(index.row());

    DialogForLatestTransfers * dialog = new DialogForLatestTransfers(this, "normal_transfers_tags", record.value("id").toInt());

    QString transferName = record.value("transfer_name").toString();
    int transferValue = record.value("transfer_value").toInt();
    QString dateString = record.value("date").toString();
    QDate date = QDate::fromString(dateString, "dd.MM.yyyy");

    dialog->setTransferName(transferName);
    dialog->setTransferValue(std::abs(transferValue));
    dialog->setDate(date);

    dialog->open();

    connect(dialog, &DialogForRegularTransfers::accepted,
            this, [=](){ this->editDialogForLatestTransfersAccepted(section, model, dialog, index, record); });
}

void TransfersTab::addDialogForRegularTransfersAccepted(TableEditBox * section, QSqlTableModel * model, DialogForRegularTransfers * dialog)
{
    QString transferName = dialog->transferName();
    int transferValue = dialog->transferValue();
    int day = dialog->dayToCountTransfer();

    if (day >= 1 && day <= 31 && transferValue != 0)
    {
        transferValue = std::abs(transferValue);
        if (!positiveNumbers)
            transferValue = -transferValue;

        QSqlRecord record;

        QSqlField field1("transfer_name", QVariant::Type::String);
        field1.setValue(transferName);
        record.append(field1);
        QSqlField field2("transfer_value", QVariant::Type::Int);
        field2.setValue(transferValue);
        record.append(field2);
        QSqlField field3("month_day_to_count_transfer", QVariant::Type::Int);
        field3.setValue(day);
        record.append(field3);

        if (model->insertRecord(-1, record))
        {
            section->tryToSubmitChanges();
            section->selectedItemChanged();
        }
    }

    dialog->reject();
}

void TransfersTab::editDialogForRegularTransfersAccepted(TableEditBox * section, QSqlTableModel * model, DialogForRegularTransfers * dialog, QModelIndex index, QSqlRecord record)
{
    QString transferName = dialog->transferName();
    int transferValue = dialog->transferValue();
    int day = dialog->dayToCountTransfer();

    // add checking old values
    if (day >= 1 && day <= 31 && transferValue != 0)
    {
        transferValue = std::abs(transferValue);
        if (!positiveNumbers)
            transferValue = -transferValue;

        record.setValue("transfer_name", transferName);
        record.setValue("transfer_value", transferValue);
        record.setValue("month_day_to_count_transfer", day);

        if (model->setRecord(index.row(), record))
        {
            section->tryToSubmitChanges();
            section->selectedItemChanged();
        }
    }

    dialog->reject();
}

void TransfersTab::addDialogForLatestTransfersAccepted(TableEditBox * section, QSqlTableModel * model, DialogForLatestTransfers * dialog)
{
    QString transferName = dialog->transferName();
    int transferValue = dialog->transferValue();
    QDate date = dialog->date();
    QString dateString = date.toString("dd.MM.yyyy");

    if (date.isValid() && transferValue != 0)
    {
        transferValue = std::abs(transferValue);
        if (!positiveNumbers)
            transferValue = -transferValue;

        QSqlRecord record;

        QSqlField field1("transfer_name", QVariant::Type::String);
        field1.setValue(transferName);
        record.append(field1);
        QSqlField field2("transfer_value", QVariant::Type::Int);
        field2.setValue(transferValue);
        record.append(field2);
        QSqlField field3("date", QVariant::Type::String);
        field3.setValue(dateString);
        record.append(field3);

        if (model->insertRecord(-1, record))
        {
            section->tryToSubmitChanges();
            section->selectedItemChanged();
        }
    }

    dialog->reject();
}

void TransfersTab::editDialogForLatestTransfersAccepted(TableEditBox * section, QSqlTableModel * model, DialogForLatestTransfers * dialog, QModelIndex index, QSqlRecord record)
{
    QString transferName = dialog->transferName();
    int transferValue = dialog->transferValue();
    QDate date = dialog->date();
    QString dateString = date.toString("dd.MM.yyyy");

    // add checking old values
    if (date.isValid() && transferValue != 0)
    {
        transferValue = std::abs(transferValue);
        if (!positiveNumbers)
            transferValue = -transferValue;

        record.setValue("transfer_name", transferName);
        record.setValue("transfer_value", transferValue);
        record.setValue("date", dateString);

        if (model->setRecord(index.row(), record))
        {
            section->tryToSubmitChanges();
            section->selectedItemChanged();
        }
    }

    dialog->reject();
}

