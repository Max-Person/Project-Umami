#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTableWidget>
#include "umamidb_interface.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_titleSearchLine_returnPressed();

    void on_franchiseSearchLine_returnPressed();

private:
    Ui::MainWindow *ui;

    void updateTitleBrowser(QVector<TitleBrowserElement> &elements);
    void updateFranchiseBrowser(QVector<FranchiseBrowserElement> &elements);
};
#endif // MAINWINDOW_H
