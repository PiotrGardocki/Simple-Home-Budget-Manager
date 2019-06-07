#include "DialogForLatestTransfers.hpp"

#include <QGridLayout>
#include <QDateEdit>
#include <QLabel>
#include <QCalendarWidget>

DialogForLatestTransfers::DialogForLatestTransfers(QWidget *parent, QString tagsTableName, int transferId)
    : DialogForTransfers(parent, tagsTableName, transferId)
{
    QGridLayout * layout = getInnerLayout();
    dateWidget = new QDateEdit(QDate::currentDate(), this);
    QCalendarWidget * calendar = new QCalendarWidget(dateWidget);
    dateWidget->setCalendarPopup(true);
    dateWidget->setCalendarWidget(calendar);

    layout->addWidget(dateWidget, 2, 1);
    layout->addWidget(new QLabel("Date:"), 2, 0);
}

QDate DialogForLatestTransfers::date() const
{
    return dateWidget->date();
}

void DialogForLatestTransfers::setDate(const QDate & newDate)
{
    dateWidget->setDate(newDate);
}
