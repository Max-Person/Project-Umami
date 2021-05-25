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

    void on_cancelTitleChangeButton_clicked();

    void on_newTitleButton_clicked();

    void on_deleteTitleButton_clicked();

    void on_franchisesTable_cellClicked(int row, int column);

    void on_editFranchiseButton_clicked();

    void on_deleteFranchiseButton_clicked();

    void on_newFranchiseButton_clicked();

    void on_cancelFranchiseChangeButton_clicked();

private:
    Ui::MainWindow *ui;

    int workingFranchiseId;
    int workingTitleId;
    ConfirmButtonState editTitleButtonState = noAction;
    ConfirmButtonState editFranchiseButtonState = noAction;

    void titleBrowserSetEnabled(bool sw);
    void titleEditorSetReadOnly(bool sw);
    void displayTitle(TitleItem displayed);
    void updateTitleBrowser(QVector<TitleBrowserElement> &elements);

    void franchiseBrowserSetEnabled(bool sw);
    void franchiseEditorSetReadOnly(bool sw);
    void displayFranchise(FranchiseItem displayed);
    void updateFranchiseBrowser(QVector<FranchiseBrowserElement> &elements);
    void updateFranchiseTitleBrowser(QVector<TitleBrowserElement> &elements);
};
#endif // MAINWINDOW_H
