#include "TagsTab.hpp"

#include <QSqlQueryModel>
#include <QSqlTableModel>
#include <QSqlField>
#include <QSqlRecord>

#include <QTableView>
#include <QApplication>
#include <QGridLayout>
#include <QPushButton>
#include <QInputDialog>
#include <QHeaderView>

TagsTab::TagsTab(QWidget *parent) : QWidget(parent)
{
    model = new QSqlTableModel(this);
    model->setTable("tags");
    model->setEditStrategy(QSqlTableModel::EditStrategy::OnManualSubmit);
    model->select();

    view = new QTableView(this);
    view->setModel(model);
    view->setSelectionMode(QAbstractItemView::SelectionMode::SingleSelection);
    view->setSelectionBehavior(QAbstractItemView::SelectionBehavior::SelectRows);
    view->setEditTriggers(QAbstractItemView::EditTrigger::NoEditTriggers);
    view->horizontalHeader()->setStretchLastSection(true);
    view->setColumnHidden(0, true);

    QPushButton * addButton = new QPushButton("Add", this);
    removeButton = new QPushButton("Remove", this);
    editButton = new QPushButton("Edit", this);

    disableButtons();

    QGridLayout * layout = new QGridLayout(this);
    layout->addWidget(addButton, 0, 0);
    layout->addWidget(removeButton, 0, 1);
    layout->addWidget(editButton, 0, 2);
    layout->addWidget(view, 1, 0, 1, 3);

    QItemSelectionModel * selectionModel = view->selectionModel();

    connect(selectionModel, &QItemSelectionModel::currentRowChanged, this, &TagsTab::selectedItemChanged);

    connect(addButton, &QPushButton::clicked, this, &TagsTab::openAddTagDialog);
    connect(removeButton, &QPushButton::clicked, this, &TagsTab::removeTag);
    connect(editButton, &QPushButton::clicked, this, &TagsTab::openEditTagDialog);
}

void TagsTab::selectedItemChanged()
{
    if (view->currentIndex().isValid())
        enableButtons();
    else
        disableButtons();
}

void TagsTab::openAddTagDialog()
{
    bool dialogSucceeded;
    QString newTagName = QInputDialog::getText(this, "Add new tag", "Enter tag name:",
                                               QLineEdit::Normal, "", &dialogSucceeded,
                                               Qt::MSWindowsFixedSizeDialogHint | Qt::Dialog);

    if (dialogSucceeded && !newTagName.isEmpty())
    {
        QSqlField field("tag_name", QVariant::Type::String);
        field.setValue(newTagName);
        QSqlRecord record;
        record.append(field);
        if (model->insertRecord(-1, record))
        {
            tryToSubmitChanges();
            selectedItemChanged();
        }
    }
}

void TagsTab::openEditTagDialog()
{
    QModelIndex index = view->currentIndex();
    if (!index.isValid())
        return;

    QSqlRecord record = model->record(index.row());
    QString oldTagName = record.value("tag_name").toString();

    bool dialogSucceeded;
    QString newTagName = QInputDialog::getText(this, "Edit tag", "Enter new tag name:",
                                               QLineEdit::Normal, oldTagName, &dialogSucceeded,
                                               Qt::MSWindowsFixedSizeDialogHint | Qt::Dialog);

    if (dialogSucceeded && !newTagName.isEmpty() && oldTagName != newTagName)
    {
        record.setValue("tag_name", newTagName);
        if (model->setRecord(index.row(), record))
            tryToSubmitChanges();
    }
}

void TagsTab::removeTag()
{
    QModelIndex index = view->currentIndex();
    if (index.isValid())
        if (model->removeRow(index.row()))
        {
            tryToSubmitChanges();
            selectedItemChanged();
        }
}

void TagsTab::enableButtons()
{
    editButton->setEnabled(true);
    removeButton->setEnabled(true);
}

void TagsTab::disableButtons()
{
    editButton->setDisabled(true);
    removeButton->setDisabled(true);
}

bool TagsTab::tryToSubmitChanges()
{
    bool success = model->submitAll();
    if (success)
        return true;
    model->revertAll();
    return false;
}
