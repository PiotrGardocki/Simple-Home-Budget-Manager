#include "TableEditBox.hpp"

#include <QSqlTableModel>
#include <QTableView>
#include <QPushButton>
#include <QGridLayout>
#include <QHeaderView>

TableEditBox::TableEditBox(const QString &title, QSqlTableModel *model, QTableView *view, QWidget *parent)
    : QGroupBox(title, parent), model(model), view(view)
{
    model->setParent(this);
    model->setEditStrategy(QSqlTableModel::EditStrategy::OnManualSubmit);
    model->select();

    view->setParent(this);
    view->setModel(model);
    view->setSelectionMode(QAbstractItemView::SelectionMode::SingleSelection);
    view->setSelectionBehavior(QAbstractItemView::SelectionBehavior::SelectRows);
    view->setEditTriggers(QAbstractItemView::EditTrigger::NoEditTriggers);
    view->horizontalHeader()->setStretchLastSection(true);

    QPushButton * addButton = new QPushButton("Add", this);
    removeButton = new QPushButton("Remove", this);
    editButton = new QPushButton("Edit", this);

    disableButtons();

    QGridLayout * layout = new QGridLayout(this);
    layout->addWidget(addButton, 0, 0);
    layout->addWidget(removeButton, 1, 0);
    layout->addWidget(editButton, 2, 0);
    layout->addWidget(view, 0, 1, 3, 1);

    QItemSelectionModel * selectionModel = view->selectionModel();

    connect(selectionModel, &QItemSelectionModel::currentRowChanged, this, &TableEditBox::selectedItemChanged);

    connect(addButton, &QPushButton::clicked, this, [&](){ emit this->openAddDialog(this, model); });
    connect(removeButton, &QPushButton::clicked, this, &TableEditBox::removeRow);
    connect(editButton, &QPushButton::clicked, this, [&](){ emit this->openEditDialog(this, model); });
}

void TableEditBox::selectedItemChanged()
{
    if (view->currentIndex().isValid())
        enableButtons();
    else
        disableButtons();
}

void TableEditBox::removeRow()
{
    QModelIndex index = view->currentIndex();
    if (index.isValid())
        if (model->removeRow(index.row()))
        {
            tryToSubmitChanges();
            selectedItemChanged();
        }
}

void TableEditBox::enableButtons()
{
    editButton->setEnabled(true);
    removeButton->setEnabled(true);
}

void TableEditBox::disableButtons()
{
    editButton->setDisabled(true);
    removeButton->setDisabled(true);
}

bool TableEditBox::tryToSubmitChanges()
{
    bool success = model->submitAll();
    if (success)
        return true;
    model->revertAll();
    return false;
}
