/*
  Copyright (C) 2010 Klarälvdalens Datakonsult AB, a KDAB Group company, info@kdab.net,
    Author: Tobias Koenig <tokoe@kdab.com>

  This library is free software; you can redistribute it and/or modify it
  under the terms of the GNU Library General Public License as published by
  the Free Software Foundation; either version 2 of the License, or (at your
  option) any later version.

  This library is distributed in the hope that it will be useful, but WITHOUT
  ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
  FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Library General Public
  License for more details.

  You should have received a copy of the GNU Library General Public License
  along with this library; see the file COPYING.LIB.  If not, write to the
  Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
  02110-1301, USA.
*/

#include "snippetsmodel.h"

#include <KMessageBox>
#include <KLocalizedString>
#include <QMimeData>
#include <QDataStream>
#include <QStringList>

using namespace MailCommon;

class MailCommon::SnippetItem
{
public:
    SnippetItem(bool isGroup = false, SnippetItem *parent = nullptr);
    ~SnippetItem();

    bool isGroup() const;

    void setName(const QString &name);
    QString name() const;

    void setText(const QString &text);
    QString text() const;

    void setKeySequence(const QString &sequence);
    QString keySequence() const;

    void appendChild(SnippetItem *child);
    void removeChild(SnippetItem *child);
    SnippetItem *child(int row) const;
    int childCount() const;
    int row() const;
    SnippetItem *parent() const;

private:
    QList<SnippetItem *> mChildItems;
    SnippetItem *mParentItem = nullptr;

    bool mIsGroup = false;
    QString mName;
    QString mText;
    QString mKeySequence;
};

SnippetItem::SnippetItem(bool isGroup, SnippetItem *parent)
    : mParentItem(parent)
    , mIsGroup(isGroup)
{
}

SnippetItem::~SnippetItem()
{
    qDeleteAll(mChildItems);
    mChildItems.clear();
}

bool SnippetItem::isGroup() const
{
    return mIsGroup;
}

void SnippetItem::setName(const QString &name)
{
    mName = name;
}

QString SnippetItem::name() const
{
    return mName;
}

void SnippetItem::setText(const QString &text)
{
    mText = text;
}

QString SnippetItem::text() const
{
    return mText;
}

void SnippetItem::setKeySequence(const QString &sequence)
{
    mKeySequence = sequence;
}

QString SnippetItem::keySequence() const
{
    return mKeySequence;
}

void SnippetItem::appendChild(SnippetItem *item)
{
    mChildItems.append(item);
}

void SnippetItem::removeChild(SnippetItem *item)
{
    mChildItems.removeAll(item);
    delete item;
}

SnippetItem *SnippetItem::child(int row) const
{
    return mChildItems.value(row);
}

int SnippetItem::childCount() const
{
    return mChildItems.count();
}

SnippetItem *SnippetItem::parent() const
{
    return mParentItem;
}

int SnippetItem::row() const
{
    if (mParentItem) {
        return mParentItem->mChildItems.indexOf(const_cast<SnippetItem *>(this));
    }

    return 0;
}

SnippetsModel *SnippetsModel::instance()
{
    static SnippetsModel s_self;
    return &s_self;
}

SnippetsModel::SnippetsModel(QObject *parent)
    : QAbstractItemModel(parent)
{
    mRootItem = new SnippetItem(true);
}

SnippetsModel::~SnippetsModel()
{
    delete mRootItem;
}

int SnippetsModel::columnCount(const QModelIndex &) const
{
    return 1;
}

bool SnippetsModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (!index.isValid()) {
        return false;
    }

    SnippetItem *item = static_cast<SnippetItem *>(index.internalPointer());
    Q_ASSERT(item);

    switch (role) {
    case NameRole:
        item->setName(value.toString());
        Q_EMIT dataChanged(index, index);
        return true;
    case TextRole:
        item->setText(value.toString());
        Q_EMIT dataChanged(index, index);
        return true;
    case KeySequenceRole:
        item->setKeySequence(value.toString());
        Q_EMIT dataChanged(index, index);
        return true;
    default:
        return false;
    }

    return false;
}

QVariant SnippetsModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid()) {
        return QVariant();
    }

    SnippetItem *item = static_cast<SnippetItem *>(index.internalPointer());

    switch (role) {
    case Qt::DisplayRole:
        return item->name();
    case IsGroupRole:
        return item->isGroup();
    case NameRole:
        return item->name();
    case TextRole:
        return item->text();
    case KeySequenceRole:
        return item->keySequence();
    }

    return QVariant();
}

Qt::ItemFlags SnippetsModel::flags(const QModelIndex &index) const
{
    Qt::ItemFlags defaultFlags = QAbstractItemModel::flags(index);

    if (index.isValid()) {
        const SnippetItem *item = static_cast<SnippetItem *>(index.internalPointer());
        if (!item->isGroup()) {
            return Qt::ItemIsDragEnabled | Qt::ItemIsDropEnabled | defaultFlags;
        }
    }

    return Qt::ItemIsDropEnabled | defaultFlags;
}

QModelIndex SnippetsModel::index(int row, int column, const QModelIndex &parent) const
{
    if (!hasIndex(row, column, parent)) {
        return QModelIndex();
    }

    SnippetItem *parentItem = nullptr;

    if (!parent.isValid()) {
        parentItem = mRootItem;
    } else {
        parentItem = static_cast<SnippetItem *>(parent.internalPointer());
    }

    SnippetItem *childItem = parentItem->child(row);
    if (childItem) {
        return createIndex(row, column, childItem);
    } else {
        return QModelIndex();
    }
}

QModelIndex SnippetsModel::parent(const QModelIndex &index) const
{
    if (!index.isValid()) {
        return QModelIndex();
    }

    SnippetItem *childItem = static_cast<SnippetItem *>(index.internalPointer());
    SnippetItem *parentItem = childItem->parent();

    if (parentItem == mRootItem) {
        return QModelIndex();
    }

    return createIndex(parentItem->row(), 0, parentItem);
}

int SnippetsModel::rowCount(const QModelIndex &parent) const
{
    SnippetItem *parentItem = nullptr;
    if (parent.column() > 0) {
        return 0;
    }

    if (!parent.isValid()) {
        parentItem = mRootItem;
    } else {
        parentItem = static_cast<SnippetItem *>(parent.internalPointer());
    }

    return parentItem->childCount();
}

bool SnippetsModel::insertRows(int row, int count, const QModelIndex &parent)
{
    SnippetItem *parentItem = nullptr;

    if (!parent.isValid()) {
        parentItem = mRootItem;
    } else {
        parentItem = static_cast<SnippetItem *>(parent.internalPointer());
    }

    beginInsertRows(parent, row, row + count - 1);
    for (int i = 0; i < count; ++i) {
        SnippetItem *snippet = new SnippetItem(!parent.isValid(), parentItem);
        parentItem->appendChild(snippet);
    }
    endInsertRows();

    return true;
}

bool SnippetsModel::removeRows(int row, int count, const QModelIndex &parent)
{
    SnippetItem *parentItem = nullptr;

    if (!parent.isValid()) {
        parentItem = mRootItem;
    } else {
        parentItem = static_cast<SnippetItem *>(parent.internalPointer());
    }

    beginRemoveRows(parent, row, row + count - 1);
    for (int i = 0; i < count; ++i) {
        parentItem->removeChild(parentItem->child(row));
    }
    endRemoveRows();

    return true;
}

QStringList SnippetsModel::mimeTypes() const
{
    return
        QStringList() << QStringLiteral("text/x-kmail-textsnippet")
                      << QStringLiteral("text/plain");
}

QMimeData *SnippetsModel::mimeData(const QModelIndexList &indexes) const
{
    if (indexes.isEmpty()) {
        return nullptr;
    }

    const QModelIndex index = indexes.first();

    SnippetItem *item = static_cast<SnippetItem *>(index.internalPointer());
    if (item->isGroup()) {
        return nullptr;
    }

    QMimeData *mimeData = new QMimeData();

    QByteArray encodedData;
    QDataStream stream(&encodedData, QIODevice::WriteOnly);
    stream << index.parent().internalId() << item->name() << item->text() << item->keySequence();

    mimeData->setData(QStringLiteral("text/x-kmail-textsnippet"), encodedData);
    mimeData->setText(item->text());

    return mimeData;
}

bool SnippetsModel::dropMimeData(const QMimeData *data, Qt::DropAction action, int row, int column, const QModelIndex &parent)
{
    Q_UNUSED(row);

    if (action == Qt::IgnoreAction) {
        return true;
    }

    if (data->hasFormat(QStringLiteral("text/plain"))) {
        if (column > 1) {
            return false;
        }
        const QString encodedData = QString::fromUtf8(data->data(QStringLiteral("text/plain")));
        if (!parent.isValid()) {
            Q_EMIT addNewDndSnippset(encodedData);
            return false;
        }
        SnippetItem *item = static_cast<SnippetItem *>(parent.internalPointer());

        if (item->isGroup()) {
            Q_EMIT addNewDndSnippset(encodedData);
        } else {
            if (KMessageBox::Yes == KMessageBox::questionYesNo(nullptr, i18n("Do you want to update snippet?"), i18n("Update snippet"))) {
                item->setText(encodedData);
            }
        }
        return false;
    } else {
        if (!parent.isValid()) {
            return false;
        }

        if (!data->hasFormat(QStringLiteral("text/x-kmail-textsnippet"))) {
            return false;
        }

        if (column > 1) {
            return false;
        }

        SnippetItem *item = static_cast<SnippetItem *>(parent.internalPointer());

        if (!item->isGroup()) {
            return false;
        }

        QByteArray encodedData = data->data(QStringLiteral("text/x-kmail-textsnippet"));
        QDataStream stream(&encodedData, QIODevice::ReadOnly);

        quintptr id;
        QString name;
        QString text;
        QString keySequence;
        stream >> id >> name >> text >> keySequence;

        if (parent.internalId() == id) {
            return false;
        }

        insertRow(rowCount(parent), parent);

        const QModelIndex idx = index(rowCount(parent) - 1, 0, parent);

        setData(idx, name, SnippetsModel::NameRole);
        setData(idx, text, SnippetsModel::TextRole);
        setData(idx, keySequence, SnippetsModel::KeySequenceRole);
        Q_EMIT dndDone();
        return true;
    }
}

Qt::DropActions SnippetsModel::supportedDropActions() const
{
    return Qt::CopyAction | Qt::MoveAction;
}
