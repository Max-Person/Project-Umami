#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    UmamiDB_interface db;

    QVector<TitleBrowserElement> titleBrowser = db.getTitleBrowser();
    QVector<FranchiseBrowserElement> franchiseBrowser = db.getFranchiseBrowser();

    updateTitleBrowser(titleBrowser);
    updateFranchiseBrowser(franchiseBrowser);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::updateTitleBrowser(QVector<TitleBrowserElement> &elements)
{
    ui->titlesTable->clearContents();
    ui->titlesTable->setRowCount(0);
    for(int i=0; i<elements.size(); ++i)
    {
        ui->titlesTable->insertRow(i);
        ui->titlesTable->setItem(i, 0,  new QTableWidgetItem(elements.at(i).name));
        ui->titlesTable->setItem(i, 1,  new QTableWidgetItem(elements.at(i).genres));
        ui->titlesTable->setItem(i, 2,  new QTableWidgetItem(elements.at(i).type));
        ui->titlesTable->setItem(i, 3,  new QTableWidgetItem(elements.at(i).status));
        ui->titlesTable->item(i,0)->setData(Qt::UserRole, elements.at(i).id);
    }

}

void MainWindow::updateFranchiseBrowser(QVector<FranchiseBrowserElement> &elements)
{
    ui->franchisesTable->clearContents();
    ui->franchisesTable->setRowCount(0);
    for(int i=0; i<elements.size(); ++i)
    {
        ui->franchisesTable->insertRow(i);
        ui->franchisesTable->setItem(i, 0,  new QTableWidgetItem(elements.at(i).name));
        ui->franchisesTable->setItem(i, 1,  new QTableWidgetItem(elements.at(i).titles));
        ui->franchisesTable->item(i,0)->setData(Qt::UserRole, elements.at(i).id);
    }

}

void MainWindow::on_titleSearchLine_returnPressed()
{
    QString searchName = ui->titleSearchLine->text();
    if(searchName.isEmpty())
        return;

    UmamiDB_interface db;

    QVector<TitleBrowserElement> searchResults = db.getTitleBrowserByName(searchName);

    updateTitleBrowser(searchResults);

}

void MainWindow::on_franchiseSearchLine_returnPressed()
{
    QString searchName = ui->franchiseSearchLine->text();
    if(searchName.isEmpty())
        return;

    UmamiDB_interface db;

    QVector<FranchiseBrowserElement> searchResults = db.getFranchiseBrowserByName(searchName);

    updateFranchiseBrowser(searchResults);

}

void MainWindow::on_titlesTable_cellClicked(int row, int column)
{
    int id = ui->titlesTable->item(row, 0)->data(Qt::UserRole).toInt();

    UmamiDB_interface db;

    TitleItem displayed = db.getTitleById(id);

    ui->titleNameLabel->setText(displayed.name);
    ui->titleStudioLine->setText(displayed.studio);
    ui->titleReleaseDateline->setText(displayed.releaseDate.toString());
    ui->titleEndingDateLine->setText(displayed.endingDate.toString());
    ui->titleStatusLine->setText(displayed.status);
    ui->titleTypeLine->setText(displayed.type);
    ui->titleFranchiseLine->setText(displayed.franchise);
    ui->titleDescText->setText(displayed.description);

}
