#include "NoteTreeModel.h"

NoteTreeModel::NoteTreeModel(NoteTreeItem* rootItem, QObject* parent)
    : QAbstractItemModel(parent),
      rootItem(rootItem)
{}

NoteTreeModel::~NoteTreeModel()
{
    delete rootItem;
}

int NoteTreeModel::columnCount(const QModelIndex& parent) const
{
    Q_UNUSED(parent)
    return 1;
}

QVariant NoteTreeModel::data(const QModelIndex& index, int role) const
{
    if (!index.isValid())
        return QVariant();

    if (role != Qt::DisplayRole)
        return QVariant();

    NoteTreeItem* item = static_cast<NoteTreeItem*>(index.internalPointer());

    //return item->note;
    return "";
}

Qt::ItemFlags NoteTreeModel::flags(const QModelIndex& index) const
{
    if (!index.isValid())
        return 0;

    return Qt::ItemIsDragEnabled | Qt::ItemIsDropEnabled | QAbstractItemModel::flags(index);
}

//NoteTreeItem* NoteTreeModel::getItem(const QModelIndex& index) const
//{
//    if (index.isValid()) {
//        NoteTreeItem* item = static_cast<NoteTreeItem*>(index.internalPointer());
//        if (item)
//            return item;
//    }
//    return rootItem;
//}

QVariant NoteTreeModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    Q_UNUSED(section)
//    if (orientation == Qt::Horizontal && role == Qt::DisplayRole)
//        return rootItem->note;

    return QVariant();
}

QModelIndex NoteTreeModel::index(int row, int column, const QModelIndex& parent) const
{
    if (!hasIndex(row, column, parent))
        return QModelIndex();

    NoteTreeItem* parentItem;

    if (!parent.isValid()) {
        parentItem = rootItem;
    } else {
        parentItem = static_cast<NoteTreeItem*>(parent.internalPointer());
    }

    NoteTreeItem* childItem = parentItem->child(row);
    if (childItem) {
        return createIndex(row, column, childItem);
    } else {
        return QModelIndex();
    }
}

bool NoteTreeModel::insertColumns(int position, int columns, const QModelIndex& parent)
{
    Q_UNUSED(position)
    Q_UNUSED(columns)
    Q_UNUSED(parent)
    return false;
}

bool NoteTreeModel::removeColumns(int position, int columns, const QModelIndex& parent)
{
    Q_UNUSED(position)
    Q_UNUSED(columns)
    Q_UNUSED(parent)
    return false;
}

bool NoteTreeModel::insertRows(int position, int rows, const QModelIndex &parent)
{
    NoteTreeItem* parentItem = static_cast<NoteTreeItem*>(parent.internalPointer());

    beginInsertRows(parent, position, position + rows - 1);
    bool success = parentItem->insertChildren(position, rows, rootItem->columnCount());
    endInsertRows();

    return success;
}

bool NoteTreeModel::removeRows(int position, int rows, const QModelIndex& parent)
{
    NoteTreeItem* parentItem = static_cast<NoteTreeItem*>(parent.internalPointer());

    beginRemoveRows(parent, position, position + rows - 1);
    bool success = parentItem->removeChildren(position, rows);
    endRemoveRows();

    return success;
}


QModelIndex NoteTreeModel::parent(const QModelIndex& index) const
{
    if (!index.isValid())
        return QModelIndex();

    NoteTreeItem* childItem = static_cast<NoteTreeItem*>(index.internalPointer());
    NoteTreeItem* parentItem = childItem->parentItem;

    if (parentItem == rootItem)
        return QModelIndex();

    return createIndex(parentItem->childNumber(), 0, parentItem);
}

int NoteTreeModel::rowCount(const QModelIndex& parent) const
{
    if (parent.column() > 0)
        return 0;

    NoteTreeItem* parentItem;
    if (!parent.isValid()) {
        parentItem = rootItem;
    } else {
        parentItem = static_cast<NoteTreeItem*>(parent.internalPointer());
    }

    return parentItem->subnotes.count();
}

bool NoteTreeModel::setData(const QModelIndex& index,
                            const QVariant& value, int role)
{
    Q_UNUSED(index)
    Q_UNUSED(value)
    Q_UNUSED(role)
    return false;
}

bool NoteTreeModel::setHeaderData(int section, Qt::Orientation orientation,
                                  const QVariant& value, int role)
{
    Q_UNUSED(section)
    Q_UNUSED(orientation)
    Q_UNUSED(value)
    Q_UNUSED(role)
    return false;
}

Qt::DropActions NoteTreeModel::supportedDropActions() const
{
    return Qt::MoveAction;
}
