#include "umamidb_interface.h"

UmamiDB_interface::UmamiDB_interface()
{
    umamiDB = QSqlDatabase::addDatabase("QSQLITE", "umamiConnection");
    umamiDB.setDatabaseName("S:/engRoute/Databases/Project-Umami/umami-SQLite3.db");
    bool ok = umamiDB.open();
}

UmamiDB_interface::~UmamiDB_interface()
{
    umamiDB.close();
    QSqlDatabase::removeDatabase("umamiConnection");
}

QVector<TitleBrowserElement> UmamiDB_interface::getTitleBrowser()
{
    QSqlQuery query(umamiDB);
    query.exec(
    "select titles.idTitles, titles.Name, group_concat(genres.Name) as Genres, title_types.Name as Type, release_statuses.Name as Status \
    from titles \
        natural join genres_to_titles \
        join genres  \
            on genres_to_titles.idGenres=genres.idGenres \
        join title_types \
            on titles.TypeID = title_types.idTitleTypes \
        join release_Statuses \
            on titles.StatusID = release_Statuses.idReleaseStatuses \
    group by titles.idTitles \
    order by titles.Name asc;");

    QVector<TitleBrowserElement> browser;
    while (query.next()) {
        TitleBrowserElement currentElement;
        currentElement.id = query.value("idTitles").toInt();
        currentElement.name = query.value("Name").toString();
        currentElement.genres = query.value("Genres").toString();
        currentElement.type = query.value("Type").toString();
        currentElement.status = query.value("Status").toString();
        browser.append(currentElement);
    }

    return browser;
}

QVector<FranchiseBrowserElement> UmamiDB_interface::getFranchiseBrowser()
{
    QSqlQuery query(umamiDB);
    query.exec(
    "select franchises.idfranchises, franchises.Name, group_concat(distinct quote(titles.Name)) as Titles \
    from franchises \
        join titles \
            on franchises.idfranchises=titles.FranchiseID \
    group by franchises.idfranchises \
    order by franchises.Name asc;");

    QVector<FranchiseBrowserElement> browser;
    while (query.next()) {
        FranchiseBrowserElement currentElement;
        currentElement.id = query.value("idfranchises").toInt();
        currentElement.name = query.value("Name").toString();
        currentElement.titles = query.value("Titles").toString();
        browser.append(currentElement);
    }

    return browser;

}
