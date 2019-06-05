#ifndef DIALOGFORTRANSFERS_HPP
#define DIALOGFORTRANSFERS_HPP

#include <QDialog>

class DialogForTransfers : public QDialog
{
    Q_OBJECT

public:
    explicit DialogForTransfers(QWidget * parent = nullptr, QString tagsTableName = "", int transferId = -1);
};

#endif // DIALOGFORTRANSFERS_HPP
