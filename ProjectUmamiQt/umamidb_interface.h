#ifndef UMAMIDB_INTERFACE_H
#define UMAMIDB_INTERFACE_H

#include <QVector>
#include <QVariant>
#include <QDate>
#include <QtSql/QSql>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlError>
#include <QCoreApplication>
#include <QMessageBox>
#include <QDebug>

struct TitleItem
{
    int id;
    QString name;
    QString description;
    QDate releaseDate;
    QDate endingDate;
    QString genres;
    QString type;
    QString status;
    QString franchise;
    QString studio;
};

struct FranchiseItem
{
    int id;
    QString name;
    QString description;
};

struct TitleBrowserElement
{
    int id;
    QString name;
    QString genres;
    QString type;
    QString status;
};

struct FranchiseBrowserElement
{
    int id;
    QString name;
    QString titles;
};

class UmamiDB_interface
{
private:

    QSqlDatabase umamiDB;

public:
    UmamiDB_interface();
    ~UmamiDB_interface();

    int lastTitleId();

    QVector<TitleBrowserElement> getTitleBrowser();
    QVector<TitleBrowserElement> getTitleBrowserByName(QString name);
    QVector<TitleBrowserElement> getTitleBrowserByFranchise(int franchiseID);

    TitleItem getTitleById(int id);

    int getStatusIdByName(QString name);
    int getTypeIdByName(QString name);
    int getStudioIdByName(QString name);
    int getFranchiseIdByName(QString name);

    void updateTitle(int id, QString name, QDate release, QDate ending, QString desc, int franchiseID, int studioID, int statusID, int typeID);
    void createTitle(int id, QString name, QDate release, QDate ending, QString desc, int franchiseID, int studioID, int statusID, int typeID);
    void deleteTitle(int id);

    void createStudio(QString name);


    int lastFranchiseId();

    QVector<FranchiseBrowserElement> getFranchiseBrowser();
    QVector<FranchiseBrowserElement> getFranchiseBrowserByName(QString name);

    FranchiseItem getFranchiseById(int id);

    void updateFranchise(int id, QString name, QString description);
    void createFranchise(int id, QString name, QString description);
    void deleteFranchise(int id);
};

#endif // UMAMIDB_INTERFACE_H
