#ifndef DIALOGFORTRANSFERS_HPP
#define DIALOGFORTRANSFERS_HPP

#include <QDialog>

class QLineEdit;
class QDoubleSpinBox;
class QGridLayout;
class QComboBox;
class QTableView;
class QSqlRelationalTableModel;

class DialogForTransfers : public QDialog
{
    Q_OBJECT

public:
    explicit DialogForTransfers(QWidget * parent = nullptr, QString tagsTableName = "", int transferId = -1);

    QString transferName() const;
    int transferValue() const;

    void setTransferName(QString name);
    void setTransferValue(int value);

public slots:
    virtual void accept() override;

private:
    void createInitialInnerLayout();

    QSqlRelationalTableModel * createModel(const QString &tagsTableName, int transferId);
    QTableView * createView(QSqlRelationalTableModel * model);
    void createTagsTable(const QString & tagsTableName, int transferId);
    QComboBox * createTagsList();

    void addTag(QComboBox * tagsList, int transferId);
    void removeTag(QModelIndex index);

protected:
    QGridLayout * getInnerLayout() const;

private:
    QLineEdit * nameWidget;
    QDoubleSpinBox * valueWidget;
    QGridLayout * layout;
    QGridLayout * innerLayout;
    QSqlRelationalTableModel * model;
};

#endif // DIALOGFORTRANSFERS_HPP
