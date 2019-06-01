#ifndef DIALOGFOREGULARTRANSFERS_HPP
#define DIALOGFOREGULARTRANSFERS_HPP

#include <QDialog>
#include <QString>

class QLineEdit;
class QDoubleSpinBox;
class QSpinBox;

class DialogForRegularTransfers : public QDialog
{
    Q_OBJECT

public:
    DialogForRegularTransfers(QWidget * parent = nullptr);

    QString transferName() const;
    int transferValue() const;
    int dayToCountTransfer() const;

    void setTransferName(QString name);
    void setTransferValue(int value);
    void setDayToCountTransfer(int day);

public slots:
    virtual void accept() override;

private:
    QLineEdit * nameWidget;
    QDoubleSpinBox * valueWidget;
    QSpinBox * dayWidget;
};

#endif // DIALOGFOREGULARTRANSFERS_HPP
