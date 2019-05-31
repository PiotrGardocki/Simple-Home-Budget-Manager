#include "MainWindow.hpp"
#include "TagsTab.hpp"

#include <QTabWidget>
#include <QWidget>
#include <QSqlDatabase>
#include <QApplication>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(QApplication::applicationDirPath() + "/../../database.db");
    db.open();

    tabs = new QTabWidget(this);
    setCentralWidget(tabs);

    tabs->addTab(new TagsTab, QString("Tags"));
}
