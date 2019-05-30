#include "MainWindow.hpp"
#include "TagsTab.hpp"

#include <QTabWidget>
#include <QWidget>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    tabs = new QTabWidget(this);
    setCentralWidget(tabs);

    tabs->addTab(new TagsTab, QString("Tags"));
}
