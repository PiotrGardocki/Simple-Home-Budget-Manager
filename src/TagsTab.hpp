#ifndef TAGSTAB_HPP
#define TAGSTAB_HPP

#include <QWidget>
#include <QModelIndex>

class QPushButton;
class QSqlTableModel;
class QTableView;

class TagsTab : public QWidget
{
    Q_OBJECT
public:
    explicit TagsTab(QWidget *parent = nullptr);

public slots:
    void selectedItemChanged();

    void openAddTagDialog();
    void openEditTagDialog();
    void removeTag();

private:
    void enableButtons();
    void disableButtons();
    bool tryToSubmitChanges();

private:
    QPushButton * editButton;
    QPushButton * removeButton;
    QSqlTableModel * model;
    QTableView * view;
};

#endif // TAGSTAB_HPP
