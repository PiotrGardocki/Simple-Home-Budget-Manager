#include "DialogForTransfers.hpp"

#include <QGridLayout>
#include <QVBoxLayout>

#include <QLabel>
#include <QLineEdit>
#include <QDoubleSpinBox>
#include <QTableView>
#include <QPushButton>
#include <QFrame>
#include <QComboBox>
#include <QDialogButtonBox>
#include <QHeaderView>

#include <QSqlRelationalTableModel>
#include <QSqlRelation>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QSqlField>

#include <cmath>

DialogForTransfers::DialogForTransfers(QWidget *parent, QString tagsTableName, int transferId)
    : QDialog(parent, Qt::WindowType::Dialog | Qt::WindowType::MSWindowsFixedSizeDialogHint)
{
    layout = new QGridLayout(this);

    createInitialInnerLayout();

    QDialogButtonBox * buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Close, this);
    layout->addWidget(buttonBox, 3, 0, 1, 2);

    connect(buttonBox, &QDialogButtonBox::accepted, this, &DialogForTransfers::accept);
    connect(buttonBox, &QDialogButtonBox::rejected, this, &DialogForTransfers::reject);

    if (transferId >= 0)
        createTagsTable(tagsTableName, transferId);
}

QString DialogForTransfers::transferName() const
{
    return nameWidget->text();
}

int DialogForTransfers::transferValue() const
{
    double dValue = valueWidget->value();
    int value = static_cast<int>(std::round(dValue * 100));
    return value;
}

void DialogForTransfers::setTransferName(QString name)
{
    nameWidget->setText(name);
}

void DialogForTransfers::setTransferValue(int value)
{
    double dValue = value / 100.;
    valueWidget->setValue(dValue);
}

void DialogForTransfers::addTag(QComboBox * tagsList, int transferId)
{
    int currentIndex = tagsList->currentIndex();
    if (currentIndex < 0)
        return;

    QSqlRecord record;
    QSqlField field1("transfer_id", QVariant::Int);
    field1.setValue(transferId);
    QSqlField field2("tag_id", QVariant::Int);
    field2.setValue(tagsList->currentData());

    record.append(field1);
    record.append(field2);

    model->insertRecord(-1, record);
    if (!model->submitAll())
        model->revertAll();
}

void DialogForTransfers::removeTag(QModelIndex index)
{
    if (!index.isValid())
        return;

    model->removeRow(index.row());
    if (!model->submitAll())
        model->revertAll();
}

void DialogForTransfers::accept()
{
    emit accepted();
}

void DialogForTransfers::createInitialInnerLayout()
{
    innerLayout = new QGridLayout;
    layout->addLayout(innerLayout, 0, 0, 1, 2);

    innerLayout->addWidget(new QLabel("Transfer name:"), 0, 0);
    innerLayout->addWidget(new QLabel("Transfer value:"), 1, 0);

    nameWidget = new QLineEdit(this);
    innerLayout->addWidget(nameWidget, 0, 1);
    valueWidget = new QDoubleSpinBox(this);
    innerLayout->addWidget(valueWidget, 1, 1);

    valueWidget->setDecimals(2);
    valueWidget->setMaximum(1000000.);
}

QSqlRelationalTableModel * DialogForTransfers::createModel(const QString & tagsTableName, int transferId)
{
    QSqlRelationalTableModel * model = new QSqlRelationalTableModel(this);
    model->setTable(tagsTableName);
    model->setEditStrategy(QSqlRelationalTableModel::OnManualSubmit);
    model->setRelation(1, QSqlRelation("tags", "id", "tag_name"));
    model->setHeaderData(1, Qt::Horizontal, "Tags");

    QString filter = QString("transfer_id = %1").arg(transferId);
    model->setFilter(filter);

    model->select();

    return model;
}

QTableView * DialogForTransfers::createView(QSqlRelationalTableModel *model)
{
    QTableView * view = new QTableView(this);
    view->setModel(model);
    view->setColumnHidden(0, true);
    view->setColumnHidden(2, true);
    view->setEditTriggers(QTableView::NoEditTriggers);
    view->setSelectionMode(QAbstractItemView::SelectionMode::SingleSelection);
    view->setSelectionBehavior(QAbstractItemView::SelectionBehavior::SelectRows);
    view->horizontalHeader()->setStretchLastSection(true);

    return view;
}

void DialogForTransfers::createTagsTable(const QString & tagsTableName, int transferId)
{
    model = createModel(tagsTableName, transferId);

    QTableView * view = createView(model);

    QFrame * separator = new QFrame(this);
    separator->setFrameStyle(QFrame::HLine);
    layout->addWidget(separator, 1, 0, 1, 2);

    QWidget * tagsTableMenu = new QWidget(this);
    QVBoxLayout * menuLayout = new QVBoxLayout(tagsTableMenu);

    QPushButton * addTag = new QPushButton("Add tag", this);
    menuLayout->addWidget(addTag);
    QComboBox * tags = createTagsList();
    menuLayout->addWidget(tags);
    QPushButton * removeTag = new QPushButton("Remove", this);
    menuLayout->addWidget(removeTag);

    layout->addWidget(tagsTableMenu, 2, 0);

    connect(addTag, &QPushButton::clicked, this, [=](){ this->addTag(tags, transferId); });
    connect(removeTag, &QPushButton::clicked, this, [=](){ this->removeTag(view->currentIndex()); });

    layout->addWidget(view, 2, 1);
}

QComboBox * DialogForTransfers::createTagsList()
{
    QSqlQuery query("SELECT * FROM tags");
    QComboBox * comboBox = new QComboBox(this);

    while (query.next())
    {
        int id = query.value(0).toInt();
        QString tagName = query.value(1).toString();
        comboBox->addItem(tagName, id);
    }

    return comboBox;
}

QGridLayout * DialogForTransfers::getInnerLayout() const
{
    return innerLayout;
}
