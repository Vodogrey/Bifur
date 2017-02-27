#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
{
    GUI();
    widgetsConfig();
}

void MainWindow::GUI()
{
    m_widget = new QWidget();
    m_layout = new QGridLayout();
    ok = new QPushButton("OK");
    m_tabs = new QTabWidget();

    m_layout->addWidget(m_tabs);

    m_widget->setLayout(m_layout);

    this->setCentralWidget(m_widget);

    m_iter = new iterationMaping();
    m_tabs->addTab(m_iter, tr("Итерационный"));

}


void MainWindow::widgetsConfig()
{
}
