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

    ui->titlesTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->franchisesTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->franchiseTitlesTable->setEditTriggers(QAbstractItemView::NoEditTriggers);

    ui->titlesTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->franchisesTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->franchiseTitlesTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    ui->cancelTitleChangeButton->hide();
    ui->cancelFranchiseChangeButton->hide();

    titleEditorSetReadOnly(true);
    franchiseEditorSetReadOnly(true);

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

void MainWindow::franchiseBrowserSetEnabled(bool sw)
{
    ui->franchiseSearchLine->setEnabled(sw);
    ui->franchisesTable->setEnabled(sw);
    ui->franchiseTitlesTable->setEnabled(sw);

    if(sw)
    {
        ui->newFranchiseButton->show();
        ui->deleteFranchiseButton->show();
    }
    else
    {
        ui->newFranchiseButton->hide();
        ui->deleteFranchiseButton->hide();
    }
}

void MainWindow::franchiseEditorSetReadOnly(bool sw)
{
    ui->franchiseNameLine->setReadOnly(sw);
    ui->franchiseDescText->setReadOnly(sw);
}

void MainWindow::displayFranchise(FranchiseItem displayed)
{
    UmamiDB_interface db;

    ui->franchiseNameLabel->setText("Франшиза:");
    ui->franchiseNameLine->setText(displayed.name);
    ui->franchiseDescText->setText(displayed.description);

    QVector<TitleBrowserElement> franchiseTitles = db.getTitleBrowserByFranchise(displayed.id);
    updateFranchiseTitleBrowser(franchiseTitles);
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

void MainWindow::updateFranchiseTitleBrowser(QVector<TitleBrowserElement> &elements)
{
    ui->franchiseTitlesTable->clearContents();
    ui->franchiseTitlesTable->setRowCount(0);
    for(int i=0; i<elements.size(); ++i)
    {
        ui->franchiseTitlesTable->insertRow(i);
        ui->franchiseTitlesTable->setItem(i, 0,  new QTableWidgetItem(elements.at(i).name));
        ui->franchiseTitlesTable->setItem(i, 1,  new QTableWidgetItem(elements.at(i).genres));
        ui->franchiseTitlesTable->setItem(i, 2,  new QTableWidgetItem(elements.at(i).type));
        ui->franchiseTitlesTable->setItem(i, 3,  new QTableWidgetItem(elements.at(i).status));
        ui->franchiseTitlesTable->item(i,0)->setData(Qt::UserRole, elements.at(i).id);
    }

}

void MainWindow::on_titleSearchLine_returnPressed()
{
    QString searchName = ui->titleSearchLine->text();

    UmamiDB_interface db;

    QVector<TitleBrowserElement> searchResults;

    if(!searchName.isEmpty())
        searchResults = db.getTitleBrowserByName(searchName);
    else
        searchResults = db.getTitleBrowser();

    updateTitleBrowser(searchResults);

}

void MainWindow::on_franchiseSearchLine_returnPressed()
{
    QString searchName = ui->franchiseSearchLine->text();

    UmamiDB_interface db;

    QVector<FranchiseBrowserElement> searchResults;

    if(!searchName.isEmpty())
        searchResults = db.getFranchiseBrowserByName(searchName);
    else
        searchResults = db.getFranchiseBrowser();

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
    if(editTitleButtonState == noAction)
    {
        editTitleButtonState = edit;
        ui->editTitleButton->setText("Изменить");
        titleBrowserSetEnabled(false);

        ui->cancelTitleChangeButton->show();

        titleEditorSetReadOnly(false);
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
        {
            QMessageBox::critical(0, "Ошибка", "Указан несуществующий статус");
            return;
        }

        QString type = ui->titleTypeLine->text();
        int typeID = db.getTypeIdByName(type);
        if(typeID == -1)
        {
            QMessageBox::critical(0, "Ошибка", "Указан несуществующий тип");
            return;
        }

        QString Franchise = ui->titleFranchiseLine->text();
        int franchiseID = db.getFranchiseIdByName(Franchise);
        if(franchiseID == -1)
        {
            QMessageBox::critical(0, "Ошибка", "Указан несуществующая франшиза");
            return;
        }

        QString studio = ui->titleStudioLine->text();
        int studioID = db.getStudioIdByName(studio);
        if(studioID == -1)
        {
            QMessageBox::critical(0, "Ошибка", "Указан несуществующая студия");
            return;
        }

        if(editTitleButtonState == edit)
            db.updateTitle(updatedId, updatedName, updatedReleaseDate, updatedEndingDate, updatedDescription, franchiseID, studioID, statusID, typeID);
        else
            db.createTitle(db.lastTitleId()+1, updatedName, updatedReleaseDate, updatedEndingDate, updatedDescription, franchiseID, studioID, statusID, typeID);


        editTitleButtonState = noAction;
        ui->editTitleButton->setText("Изменить данные о тайтле");
        QVector<TitleBrowserElement> temp = db.getTitleBrowser();
        updateTitleBrowser(temp);
        titleBrowserSetEnabled(true);

        TitleItem displayed = db.getTitleById(workingTitleId);
        displayTitle(displayed);

        ui->cancelTitleChangeButton->hide();

        titleEditorSetReadOnly(true);

    }
}

void MainWindow::titleBrowserSetEnabled(bool sw)
{
    ui->titleSearchLine->setEnabled(sw);
    ui->titlesTable->setEnabled(sw);

    if(sw)
    {
        ui->newTitleButton->show();
        ui->deleteTitleButton->show();
    }
    else
    {
        ui->newTitleButton->hide();
        ui->deleteTitleButton->hide();
    }

}

void MainWindow::titleEditorSetReadOnly(bool sw)
{
    ui->titleNameLine->setReadOnly(sw);
    ui->titleStudioLine->setReadOnly(sw);
    ui->titleReleaseDateline->setReadOnly(sw);
    ui->titleEndingDateLine->setReadOnly(sw);
    ui->titleStatusLine->setReadOnly(sw);
    ui->titleTypeLine->setReadOnly(sw);
    ui->titleFranchiseLine->setReadOnly(sw);
    ui->titleDescText->setReadOnly(sw);
}

void MainWindow::displayTitle(TitleItem displayed)
{
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

void MainWindow::on_cancelTitleChangeButton_clicked()
{
    editTitleButtonState = noAction;
    ui->editTitleButton->setText("Изменить данные о тайтле");

    UmamiDB_interface db;

    TitleItem displayed = db.getTitleById(workingTitleId);

    displayTitle(displayed);

    titleBrowserSetEnabled(true);

    ui->cancelTitleChangeButton->hide();

    titleEditorSetReadOnly(true);
}

void MainWindow::on_newTitleButton_clicked()
{
    if(editTitleButtonState == noAction)
    {
        editTitleButtonState = creation;
        ui->editTitleButton->setText("Создать");
        titleBrowserSetEnabled(false);

        TitleItem displayed;

        displayTitle(displayed);

        ui->cancelTitleChangeButton->show();

        titleEditorSetReadOnly(false);
    }
}

void MainWindow::on_deleteTitleButton_clicked()
{
    UmamiDB_interface db;

    db.deleteTitle(workingTitleId);

    TitleItem displayed;
    displayTitle(displayed);
    ui->titleNameLabel->setText("Тайтл не выбран");

    QVector<TitleBrowserElement> temp = db.getTitleBrowser();
    updateTitleBrowser(temp);
}

void MainWindow::on_franchisesTable_cellClicked(int row, int column)
{
    int id = ui->franchisesTable->item(row, 0)->data(Qt::UserRole).toInt();

    UmamiDB_interface db;

    FranchiseItem displayed = db.getFranchiseById(id);

    workingFranchiseId = id;
    displayFranchise(displayed);
}

void MainWindow::on_editFranchiseButton_clicked()
{
    if(editFranchiseButtonState == noAction)
    {
        editFranchiseButtonState = edit;
        ui->editFranchiseButton->setText("Изменить");
        ui->franchiseTitlesLabel->setText("Тайтлы франшизы напрямую изменить нельзя");
        franchiseBrowserSetEnabled(false);

        ui->cancelFranchiseChangeButton->show();

        franchiseEditorSetReadOnly(false);
    }
    else
    {
        int updatedId = workingFranchiseId;
        QString updatedName = ui->franchiseNameLine->text();
        QString updatedDescription = ui->franchiseDescText->toPlainText();

        UmamiDB_interface db;

        if(editFranchiseButtonState == edit)
            db.updateFranchise(updatedId, updatedName, updatedDescription);
        else
            db.createFranchise(db.lastFranchiseId()+1, updatedName, updatedDescription);


        editFranchiseButtonState = noAction;
        ui->editFranchiseButton->setText("Изменить данные франшизы");
        ui->franchiseTitlesLabel->setText("Тайтлы франшизы");
        QVector<FranchiseBrowserElement> temp = db.getFranchiseBrowser();
        updateFranchiseBrowser(temp);
        franchiseBrowserSetEnabled(true);

        FranchiseItem displayed = db.getFranchiseById(workingFranchiseId);
        displayFranchise(displayed);

        ui->cancelFranchiseChangeButton->hide();
        ui->deleteFranchiseButton->setEnabled(true);

        franchiseEditorSetReadOnly(true);

    }
}

void MainWindow::on_deleteFranchiseButton_clicked()
{
    UmamiDB_interface db;

    db.deleteFranchise(workingFranchiseId);

    ui->franchiseNameLabel->setText("Франшиза не выбрана");
    ui->franchiseNameLine->clear();
    ui->franchiseDescText->clear();
    ui->franchiseTitlesTable->clearContents();

    QVector<FranchiseBrowserElement> temp = db.getFranchiseBrowser();
    updateFranchiseBrowser(temp);

}

void MainWindow::on_newFranchiseButton_clicked()
{
    if(editFranchiseButtonState == noAction)
    {
        editFranchiseButtonState = creation;
        ui->editFranchiseButton->setText("Создать");
        ui->franchiseTitlesLabel->setText("Тайтлы франшизы напрямую изменить нельзя");
        franchiseBrowserSetEnabled(false);

        ui->franchiseNameLine->clear();
        ui->franchiseDescText->clear();
        ui->franchiseTitlesTable->clearContents();
        ui->franchiseTitlesTable->setRowCount(0);

        ui->cancelFranchiseChangeButton->show();

        franchiseEditorSetReadOnly(false);
    }
}

void MainWindow::on_cancelFranchiseChangeButton_clicked()
{

    UmamiDB_interface db;

    editFranchiseButtonState = noAction;
    ui->editFranchiseButton->setText("Изменить данные франшизы");
    ui->franchiseTitlesLabel->setText("Тайтлы франшизы");
    franchiseBrowserSetEnabled(true);

    FranchiseItem displayed = db.getFranchiseById(workingFranchiseId);
    displayFranchise(displayed);

    ui->cancelFranchiseChangeButton->hide();

    franchiseEditorSetReadOnly(true);
}
