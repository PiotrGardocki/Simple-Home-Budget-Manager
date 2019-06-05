#ifndef DIALOGFOREGULARTRANSFERS_HPP
#define DIALOGFOREGULARTRANSFERS_HPP

#include "DialogForTransfers.hpp"

#include <QString>

class QSpinBox;

class DialogForRegularTransfers : public DialogForTransfers
{
    Q_OBJECT

public:
    explicit DialogForRegularTransfers(QWidget * parent = nullptr, QString tagsTableName = "", int transferId = -1);

    int dayToCountTransfer() const;

    void setDayToCountTransfer(int day);

private:
    QSpinBox * dayWidget;
};

#endif // DIALOGFOREGULARTRANSFERS_HPP
