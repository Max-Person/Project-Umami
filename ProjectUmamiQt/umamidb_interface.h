#ifndef UMAMIDB_INTERFACE_H
#define UMAMIDB_INTERFACE_H

#include <QVector>
#include <QVariant>
#include <QDate>
#include <QtSql/QSql>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlError>
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

    QVector<TitleBrowserElement> getTitleBrowser();
    QVector<TitleBrowserElement> getTitleBrowserByName(QString name);
    QVector<TitleBrowserElement> getTitleBrowserByFranchise(QString franchise);

    TitleItem getTitleById(int id);

    QVector<FranchiseBrowserElement> getFranchiseBrowser();
    QVector<FranchiseBrowserElement> getFranchiseBrowserByName(QString name);
};

#endif // UMAMIDB_INTERFACE_H
