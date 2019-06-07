#ifndef DIALOGFORLATESTTRANSFERS_HPP
#define DIALOGFORLATESTTRANSFERS_HPP

#include "DialogForTransfers.hpp"

#include <QDate>

class QDateEdit;

class DialogForLatestTransfers : public DialogForTransfers
{
    Q_OBJECT

public:
    explicit DialogForLatestTransfers(QWidget * parent = nullptr, QString tagsTableName = "", int transferId = -1);

    QDate date() const;
    void setDate(const QDate & newDate);

private:
    QDateEdit * dateWidget;
};

#endif // DIALOGFORLATESTTRANSFERS_HPP
