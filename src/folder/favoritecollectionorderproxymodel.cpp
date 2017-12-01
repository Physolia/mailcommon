/*

  Copyright (c) 2017 David Faure <faure@kde.org>

  This program is free software; you can redistribute it and/or modify it
  under the terms of the GNU General Public License, version 2, as
  published by the Free Software Foundation.

  This program is distributed in the hope that it will be useful, but
  WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  General Public License for more details.

  You should have received a copy of the GNU General Public License along
  with this program; if not, write to the Free Software Foundation, Inc.,
  51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
*/

#include "favoritecollectionorderproxymodel.h"
#include "mailcommon_debug.h"
#include <Collection>
#include <EntityTreeModel>

using namespace MailCommon;

class Q_DECL_HIDDEN FavoriteCollectionOrderProxyModel::FavoriteCollectionOrderProxyModelPrivate
{
public:
    FavoriteCollectionOrderProxyModelPrivate()
    {
    }
};

FavoriteCollectionOrderProxyModel::FavoriteCollectionOrderProxyModel(QObject *parent)
    : EntityOrderProxyModel(parent)
    , d(nullptr) //, d(new FavoriteCollectionOrderProxyModelPrivate())
{
}

FavoriteCollectionOrderProxyModel::~FavoriteCollectionOrderProxyModel()
{
    delete d;
}

Qt::ItemFlags FavoriteCollectionOrderProxyModel::flags(const QModelIndex &index) const
{
    Qt::ItemFlags flags = KRecursiveFilterProxyModel::flags(index);
    // Don't allow dropping on folders
    if (index.isValid()) {
        flags &= ~Qt::ItemIsDropEnabled;
    }
    return flags;
}

Akonadi::Collection FavoriteCollectionOrderProxyModel::parentCollection(const QModelIndex &index) const
{
    Q_UNUSED(index);
    return {};
}
