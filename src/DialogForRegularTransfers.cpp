#include "DialogForRegularTransfers.hpp"

#include <QGridLayout>
#include <QLabel>
#include <QSpinBox>

DialogForRegularTransfers::DialogForRegularTransfers(QWidget * parent, QString tagsTableName, int transferId)
    : DialogForTransfers(parent, tagsTableName, transferId)
{
    QGridLayout * layout = getInnerLayout();
    dayWidget = new QSpinBox(this);
    dayWidget->setRange(1, 31);

    layout->addWidget(dayWidget, 2, 1);
    layout->addWidget(new QLabel("Month of day to count:"), 2, 0);
}

int DialogForRegularTransfers::dayToCountTransfer() const
{
    return dayWidget->value();
}

void DialogForRegularTransfers::setDayToCountTransfer(int day)
{
    dayWidget->setValue(day);
}
