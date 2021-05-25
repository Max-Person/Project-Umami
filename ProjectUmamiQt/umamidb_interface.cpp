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
        left join genres_to_titles \
            on titles.idTitles=genres_to_titles.idTitles \
        left join genres  \
            on genres_to_titles.idGenres=genres.idGenres \
        left join title_types \
            on titles.TypeID = title_types.idTitleTypes \
        left join release_Statuses \
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

QVector<TitleBrowserElement> UmamiDB_interface::getTitleBrowserByName(QString name)
{
    QSqlQuery query(umamiDB);
    query.exec(
    "select titles.idTitles, titles.Name, group_concat(genres.Name) as Genres, title_types.Name as Type, release_statuses.Name as Status \
    from titles \
        left join genres_to_titles \
            on titles.idTitles=genres_to_titles.idTitles \
        left join genres  \
            on genres_to_titles.idGenres=genres.idGenres \
        left join title_types \
            on titles.TypeID = title_types.idTitleTypes \
        left join release_Statuses \
            on titles.StatusID = release_Statuses.idReleaseStatuses \
    where titles.Name like \"%" + name + "%\" \
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

TitleItem UmamiDB_interface::getTitleById(int id)
{
    QSqlQuery query(umamiDB);
    query.exec(
    "select titles.idTitles, titles.Name, titles.DateOfRelease, titles.EndingDate, titles.Description, \
        group_concat(genres.Name) as Genres, franchises.Name as Franchise, studios.Name as Studio, title_types.Name as Type, release_statuses.Name as Status \
    from titles \
        left join genres_to_titles \
            on titles.idTitles=genres_to_titles.idTitles \
        left join genres  \
            on genres_to_titles.idGenres=genres.idGenres \
        left join franchises \
            on titles.FranchiseID = franchises.idfranchises \
        left join studios \
            on titles.StudioID = studios.idStudios \
        left join title_types \
            on titles.TypeID = title_types.idTitleTypes \
        left join release_Statuses \
            on titles.StatusID = release_Statuses.idReleaseStatuses \
    where titles.idTitles = " + QString::number(id) + " \
    group by titles.idTitles;");

    query.next();

    TitleItem returned;
    returned.id = query.value("idTitles").toInt();
    returned.name = query.value("Name").toString();
    returned.description = query.value("Description").toString();
    returned.releaseDate = query.value("DateOfRelease").toDate();
    returned.endingDate = query.value("EndingDate").toDate();
    returned.genres = query.value("Genres").toString();
    returned.type = query.value("Type").toString();
    returned.status = query.value("Status").toString();
    returned.franchise = query.value("Franchise").toString();
    returned.studio = query.value("Studio").toString();

    return returned;
}

int UmamiDB_interface::getStatusIdByName(QString name)
{
    QSqlQuery query(umamiDB);
    query.exec("select release_statuses.idReleaseStatuses from release_statuses where release_statuses.Name = \"" + name + "\" limit 1;");

    if(!query.next())
        return -1;
    else
        return query.value("idReleaseStatuses").toInt();
}

int UmamiDB_interface::getTypeIdByName(QString name)
{
    QSqlQuery query(umamiDB);
    query.exec("select title_types.idTitleTypes from title_types where title_types.Name = \"" + name + "\" limit 1;");

    if(!query.next())
        return -1;
    else
        return query.value("idTitleTypes").toInt();
}

int UmamiDB_interface::getStudioIdByName(QString name)
{
    QSqlQuery query(umamiDB);
    query.exec("select studios.idStudios from studios where studios.Name = \"" + name + "\" limit 1;");

    if(!query.next())
        return -1;
    else
        return query.value("idStudios").toInt();
}

int UmamiDB_interface::getFranchiseIdByName(QString name)
{
    QSqlQuery query(umamiDB);
    query.exec("select franchises.idfranchises from franchises where franchises.Name = \"" + name + "\" limit 1;");

    if(!query.next())
        return -1;
    else
        return query.value("idfranchises").toInt();
}

void UmamiDB_interface::updateTitle(int id, QString name, QDate release, QDate ending, QString desc, int franchiseID, int studioID, int statusID, int typeID)
{
    QSqlQuery query(umamiDB);
    query.exec(
    "update titles \
    set Name = \"" + name + "\", \
        DateOfRelease = \"" + release.toString(Qt::ISODate) + "\", \
        EndingDate = \"" + ending.toString(Qt::ISODate) + "\", \
        Description = \"" + desc + "\", \
        FranchiseID = " + QString::number(franchiseID) + ", \
        StudioID = " + QString::number(studioID) + ", \
        StatusID = " + QString::number(statusID) + ", \
        TypeID = " + QString::number(typeID) + " \
    where idTitles = " + QString::number(id) + ";");

    qDebug() << query.lastQuery() << "\n" << query.lastError();
}

void UmamiDB_interface::createStudio(QString name)
{
    QSqlQuery query(umamiDB);
    query.prepare("insert into studios(Name) values (:name);");
    query.bindValue(":name", name);

    query.exec();
}

void UmamiDB_interface::createFranchise(QString name, QString description)
{
    QSqlQuery query(umamiDB);
    query.prepare("insert into franchises(Name, Description) values (:name, :desc);");
    query.bindValue(":name", name);
    query.bindValue(":desc", description);

    query.exec();
}


QVector<FranchiseBrowserElement> UmamiDB_interface::getFranchiseBrowser()
{
    QSqlQuery query(umamiDB);
    query.exec(
    "select franchises.idfranchises, franchises.Name, group_concat(distinct titles.Name) as Titles \
    from franchises \
        left join titles \
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

QVector<FranchiseBrowserElement> UmamiDB_interface::getFranchiseBrowserByName(QString name)
{
    QSqlQuery query(umamiDB);
    query.exec(
    "select franchises.idfranchises, franchises.Name, group_concat(distinct titles.Name) as Titles \
    from franchises \
        left join titles \
            on franchises.idfranchises=titles.FranchiseID \
    where franchises.Name like \"%" + name + "%\" \
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
