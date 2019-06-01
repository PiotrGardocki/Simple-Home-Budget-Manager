#ifndef TRANSFERSSECTION_HPP
#define TRANSFERSSECTION_HPP

#include <QGroupBox>
#include <QString>
#include <QModelIndex>

class QPushButton;
class QSqlTableModel;
class QTableView;

class TableEditBox : public QGroupBox
{
    Q_OBJECT
public:
    explicit TableEditBox(const QString & title, QSqlTableModel * model,
                              QTableView * view, QWidget *parent = nullptr);

public slots:
    void selectedItemChanged();
    void removeRow();

signals:
    void openAddDialog(TableEditBox * section, QSqlTableModel * model);
    void openEditDialog(TableEditBox * section, QSqlTableModel * model, QModelIndex index);

private:
    void enableButtons();
    void disableButtons();

public:
    bool tryToSubmitChanges();

private:
    QPushButton * removeButton;
    QPushButton * editButton;
    QSqlTableModel * model;
    QTableView * view;
};

#endif // TRANSFERSSECTION_HPP
