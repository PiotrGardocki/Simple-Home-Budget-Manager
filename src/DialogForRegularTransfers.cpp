#include "DialogForRegularTransfers.hpp"

#include <QGridLayout>
#include <QLabel>
#include <QLineEdit>
#include <QDoubleSpinBox>
#include <QSpinBox>
#include <QDialogButtonBox>

#include <cmath>

DialogForRegularTransfers::DialogForRegularTransfers(QWidget *parent)
    : QDialog(parent, Qt::WindowType::Dialog | Qt::WindowType::MSWindowsFixedSizeDialogHint)
{
    QGridLayout * layout = new QGridLayout(this);

    layout->addWidget(new QLabel("Transfer name:"), 0, 0);
    layout->addWidget(new QLabel("Transfer value:"), 1, 0);
    layout->addWidget(new QLabel("Month of day to count:"), 2, 0);

    nameWidget = new QLineEdit(this);
    layout->addWidget(nameWidget, 0, 1);
    valueWidget = new QDoubleSpinBox(this);
    layout->addWidget(valueWidget, 1, 1);
    dayWidget = new QSpinBox(this);
    layout->addWidget(dayWidget, 2, 1);

    dayWidget->setRange(1, 31);
    valueWidget->setDecimals(2);
    valueWidget->setMaximum(1000000.);

    QDialogButtonBox * buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Close, this);
    layout->addWidget(buttonBox, 3, 0, 1, 2);

    connect(buttonBox, &QDialogButtonBox::accepted, this, &DialogForRegularTransfers::accept);
    connect(buttonBox, &QDialogButtonBox::rejected, this, &DialogForRegularTransfers::reject);
}

QString DialogForRegularTransfers::transferName() const
{
    return nameWidget->text();
}

int DialogForRegularTransfers::transferValue() const
{
    double dValue = valueWidget->value();
    int value = static_cast<int>(std::round(dValue * 100));
    return value;
}

int DialogForRegularTransfers::dayToCountTransfer() const
{
    return dayWidget->value();
}

void DialogForRegularTransfers::setTransferName(QString name)
{
    nameWidget->setText(name);
}

void DialogForRegularTransfers::setTransferValue(int value)
{
    double dValue = value / 100.;
    valueWidget->setValue(dValue);
}

void DialogForRegularTransfers::setDayToCountTransfer(int day)
{
    dayWidget->setValue(day);
}

void DialogForRegularTransfers::accept()
{
    emit accepted();
}
