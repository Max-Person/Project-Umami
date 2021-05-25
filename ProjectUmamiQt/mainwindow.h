#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTableWidget>
#include <QMessageBox>
#include "umamidb_interface.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

enum ConfirmButtonState {
    noAction,
    edit,
    creation
};

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_titleSearchLine_returnPressed();

    void on_franchiseSearchLine_returnPressed();

    void on_titlesTable_cellClicked(int row, int column);

    void on_editTitleButton_clicked();

    void on_cancelChangeButton_clicked();

    void on_newTitleButton_clicked();

    void on_deleteTitleButton_clicked();

private:
    Ui::MainWindow *ui;

    int workingTitleId;
    ConfirmButtonState editTitleButtonState = noAction;

    void titleBrowserSetEnabled(bool sw);
    void displayTitle(TitleItem displayed);
    void updateTitleBrowser(QVector<TitleBrowserElement> &elements);
    void updateFranchiseBrowser(QVector<FranchiseBrowserElement> &elements);
};
#endif // MAINWINDOW_H
