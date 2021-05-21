#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    updateTitleBrowser();
    updateFranchiseBrowser();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::updateTitleBrowser()
{
    UmamiDB_interface db;

    QVector<TitleBrowserElement> browser = db.getTitleBrowser();

    for(int i=0; i<browser.size(); ++i)
    {
        ui->titlesTable->insertRow(i);
        ui->titlesTable->setItem(i, 0,  new QTableWidgetItem(browser.at(i).name));
        ui->titlesTable->setItem(i, 1,  new QTableWidgetItem(browser.at(i).genres));
        ui->titlesTable->setItem(i, 2,  new QTableWidgetItem(browser.at(i).type));
        ui->titlesTable->setItem(i, 3,  new QTableWidgetItem(browser.at(i).status));
        ui->titlesTable->item(i,0)->setData(Qt::UserRole, browser.at(i).id);
    }

}

void MainWindow::updateFranchiseBrowser()
{
    UmamiDB_interface db;

    QVector<FranchiseBrowserElement> browser = db.getFranchiseBrowser();

    for(int i=0; i<browser.size(); ++i)
    {
        ui->franchisesTable->insertRow(i);
        ui->franchisesTable->setItem(i, 0,  new QTableWidgetItem(browser.at(i).name));
        ui->franchisesTable->setItem(i, 1,  new QTableWidgetItem(browser.at(i).titles));
        ui->franchisesTable->item(i,0)->setData(Qt::UserRole, browser.at(i).id);
    }


}

