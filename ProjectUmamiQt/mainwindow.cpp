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

    ui->cancelChangeButton->hide();

    ui->titleNameLine->setReadOnly(true);
    ui->titleStudioLine->setReadOnly(true);
    ui->titleReleaseDateline->setReadOnly(true);
    ui->titleEndingDateLine->setReadOnly(true);
    ui->titleStatusLine->setReadOnly(true);
    ui->titleTypeLine->setReadOnly(true);
    ui->titleFranchiseLine->setReadOnly(true);
    ui->titleDescText->setReadOnly(true);

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

    workingTitleId = displayed.id;
    ui->titleNameLabel->setText("Тайтл:");
    ui->titleNameLine->setText(displayed.name);
    ui->titleStudioLine->setText(displayed.studio);
    ui->titleReleaseDateline->setText(displayed.releaseDate.toString(Qt::ISODate));
    ui->titleEndingDateLine->setText(displayed.endingDate.toString(Qt::ISODate));
    ui->titleStatusLine->setText(displayed.status);
    ui->titleTypeLine->setText(displayed.type);
    ui->titleFranchiseLine->setText(displayed.franchise);
    ui->titleDescText->setText(displayed.description);

}

void MainWindow::on_editTitleButton_clicked()
{
    if(editTitleButtonState == false)
    {
        editTitleButtonState = !editTitleButtonState;
        titleBrowserSetEnabled(false);

        ui->cancelChangeButton->show();

        ui->titleNameLine->setReadOnly(false);
        ui->titleStudioLine->setReadOnly(false);
        ui->titleReleaseDateline->setReadOnly(false);
        ui->titleEndingDateLine->setReadOnly(false);
        ui->titleStatusLine->setReadOnly(false);
        ui->titleTypeLine->setReadOnly(false);
        ui->titleFranchiseLine->setReadOnly(false);
        ui->titleDescText->setReadOnly(false);
    }
    else
    {
        int updatedId = workingTitleId;
        QString updatedName = ui->titleNameLine->text();
        QString updatedDescription = ui->titleDescText->toPlainText();
        QDate updatedReleaseDate = QDate::fromString(ui->titleReleaseDateline->text(), Qt::ISODate);
        QDate updatedEndingDate = QDate::fromString(ui->titleEndingDateLine->text(), Qt::ISODate);

        UmamiDB_interface db;

        QString status = ui->titleStatusLine->text();
        int statusID = db.getStatusIdByName(status);
        if(statusID == -1)
            return;

        QString type = ui->titleTypeLine->text();
        int typeID = db.getTypeIdByName(type);
        if(typeID == -1)
            return;

        QString Franchise = ui->titleFranchiseLine->text();
        int franchiseID = db.getFranchiseIdByName(Franchise);
        if(franchiseID == -1)
            return;

        QString studio = ui->titleStudioLine->text();
        int studioID = db.getStudioIdByName(studio);
        if(studioID == -1)
            return;

        db.updateTitle(updatedId, updatedName, updatedReleaseDate, updatedEndingDate, updatedDescription, franchiseID, studioID, statusID, typeID);

        editTitleButtonState = !editTitleButtonState;
        QVector<TitleBrowserElement> temp = db.getTitleBrowser();
        updateTitleBrowser(temp);
        titleBrowserSetEnabled(true);

        ui->cancelChangeButton->hide();

        ui->titleNameLine->setReadOnly(true);
        ui->titleStudioLine->setReadOnly(true);
        ui->titleReleaseDateline->setReadOnly(true);
        ui->titleEndingDateLine->setReadOnly(true);
        ui->titleStatusLine->setReadOnly(true);
        ui->titleTypeLine->setReadOnly(true);
        ui->titleFranchiseLine->setReadOnly(true);
        ui->titleDescText->setReadOnly(true);

    }
}

void MainWindow::titleBrowserSetEnabled(bool sw)
{
    ui->titleSearchLine->setEnabled(sw);
    ui->titlesTable->setEnabled(sw);
    ui->refreshTitlesTableButton->setEnabled(sw);
    ui->newTitleButton->setEnabled(sw);
}

void MainWindow::on_cancelChangeButton_clicked()
{
    editTitleButtonState = false;

    UmamiDB_interface db;

    TitleItem displayed = db.getTitleById(workingTitleId);

    ui->titleNameLabel->setText("Тайтл:");
    ui->titleNameLine->setText(displayed.name);
    ui->titleStudioLine->setText(displayed.studio);
    ui->titleReleaseDateline->setText(displayed.releaseDate.toString(Qt::ISODate));
    ui->titleEndingDateLine->setText(displayed.endingDate.toString(Qt::ISODate));
    ui->titleStatusLine->setText(displayed.status);
    ui->titleTypeLine->setText(displayed.type);
    ui->titleFranchiseLine->setText(displayed.franchise);
    ui->titleDescText->setText(displayed.description);

    titleBrowserSetEnabled(true);

    ui->cancelChangeButton->hide();

    ui->titleNameLine->setReadOnly(true);
    ui->titleStudioLine->setReadOnly(true);
    ui->titleReleaseDateline->setReadOnly(true);
    ui->titleEndingDateLine->setReadOnly(true);
    ui->titleStatusLine->setReadOnly(true);
    ui->titleTypeLine->setReadOnly(true);
    ui->titleFranchiseLine->setReadOnly(true);
    ui->titleDescText->setReadOnly(true);
}
