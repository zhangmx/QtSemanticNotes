#include "Tag.h"

#include "../database/Database.h"

#include <QSqlQuery>
#include <QSqlRecord>

#include <QDebug>

using std::make_unique;

inline Tag::Tag(const Id& id,
                const QString& name,
                const QDateTime& createdAt,
                const QDateTime& updatedAt)
    : id(id),
      name(name),
      createdAt(createdAt),
      updatedAt(updatedAt)
{}

vector<unique_ptr<Tag>> Tag::getAll()
{
    QSqlQuery q;
    q.prepare("SELECT id, name, created_at FROM tags");
    Database::safeExecPreparedQuery(q);

    vector<unique_ptr<Tag>> tags;
    while (q.next()) {
        tags.emplace_back(
                make_unique<Tag>(
                    Tag(q.value(0).toUInt(),
                        q.value(1).toString(),
                        q.value(2).toDateTime(),
                        q.value(3).toDateTime())));
    }
    return tags;
}

optional<unique_ptr<Tag>> Tag::getById(const Id& id)
{
    QSqlQuery q;
    q.prepare("SELECT id, name, created_at, updated_at FROM tags "
              "WHERE id = :id");
    q.bindValue(":id", id);
    Database::safeExecPreparedQuery(q);

    if (q.next()) {
        return make_unique<Tag>(
                    Tag(q.value(0).toUInt(),
                        q.value(1).toString(),
                        q.value(2).toDateTime(),
                        q.value(3).toDateTime()));
    } else {
        return {};
    }
}

Id Tag::getId() const
{
    return id;
}

QString Tag::getName() const
{
    return name;
}

void Tag::setName(const QString& value)
{
    name = value;
}

QDateTime Tag::getCreatedAt() const
{
    return createdAt;
}

QDateTime Tag::getUpdatedAt() const
{
    return updatedAt;
}

QString Tag::toString() const
{
    return QString("id: %1, name: %2, created_at: %4, updated_at: %5")
            .arg(QString::number(id))
            .arg(name)
            .arg(createdAt.toString())
            .arg(updatedAt.toString());
}

unique_ptr<Tag> Tag::create(const QString& name)
{
    QDateTime now = QDateTime::currentDateTime();
    QString now_s = now.toString(Qt::ISODateWithMs);

    Id id;

    QSqlDatabase::database().transaction();
    try {
        QSqlQuery insertTagQuery;
        insertTagQuery.prepare("INSERT INTO tags (name, created_at, updated_at) "
                  "VALUES (:name, :created_at, :updated_at)");
        insertTagQuery.bindValue(":name", name);
        insertTagQuery.bindValue(":created_at", now_s);
        insertTagQuery.bindValue(":updated_at", now_s);
        Database::safeExecPreparedQuery(insertTagQuery);

        QSqlQuery rowIdQuery;
        rowIdQuery.prepare("SELECT last_insert_rowid()");
        Database::safeExecPreparedQuery(rowIdQuery);
        id = rowIdQuery.value(0).toUInt();

        QSqlDatabase::database().commit();
    } catch (QueryException e) {
        QSqlDatabase::database().rollback();
        throw e;
    }
    return make_unique<Tag>(Tag(id, name, now, now));
}

void Tag::update()
{
    QDateTime now = QDateTime::currentDateTime();
    QString now_s = now.toString(Qt::ISODateWithMs);

    QSqlQuery updateTagsQuery;
    updateTagsQuery.prepare("UPDATE tags "
              "SET name = :tname, "
              "updated_at = :updated_at "
              "WHERE id = :id");
    updateTagsQuery.bindValue(":id", id);
    updateTagsQuery.bindValue(":title", name);
    updateTagsQuery.bindValue(":updated_at", now_s);

    updatedAt = now;
}

void Tag::remove()
{
    QSqlQuery q;
    q.prepare("DELETE FROM tags "
              "WHERE id = :id");
    q.bindValue(":id", id);
    Database::safeExecPreparedQuery(q);
}
