#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QObject>
#include <QWidget>
#include <QPushButton>
#include <QGridLayout>
#include <QDebug>
#include <QTabWidget>

#include "iterationmaping.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = 0);

signals:

public slots:

protected:
    void GUI();
    void widgetsConfig();

private:
    QWidget* m_widget;
    QGridLayout* m_layout;
    QPushButton* ok;
    QTabWidget* m_tabs;

    iterationMaping* m_iter;
};

#endif // MAINWINDOW_H
