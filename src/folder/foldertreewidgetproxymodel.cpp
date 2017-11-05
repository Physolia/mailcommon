/*
  Copyright (c) 2009-2017 Laurent Montel <montel@kde.org>

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

#include "foldertreewidgetproxymodel.h"
#include "foldersettings.h"
#include "kernel/mailkernel.h"
#include "util/mailutil.h"

#include <MessageCore/MessageCoreSettings>

#include <AgentInstance>
#include <AgentManager>
#include <Collection>
#include <EntityTreeModel>
#include <MimeTypeChecker>

#include <KColorScheme>
#include <KLocalizedString>

#include <QPalette>

namespace MailCommon {
class FolderTreeWidgetProxyModel::Private
{
public:
    Private()
        : enableCheck(false)
        , hideVirtualFolder(false)
        , hideSpecificFolder(false)
        , hideOutboxFolder(false)
    {
        const KColorScheme scheme(QPalette::Active, KColorScheme::View);
        brokenAccountColor = scheme.foreground(KColorScheme::NegativeText).color();
    }

    QSet<QString> includedMimeTypes;
    Akonadi::MimeTypeChecker checker;

    QColor brokenAccountColor;
    bool enableCheck;
    bool hideVirtualFolder;
    bool hideSpecificFolder;
    bool hideOutboxFolder;
};

FolderTreeWidgetProxyModel::FolderTreeWidgetProxyModel(QObject *parent, FolderTreeWidgetProxyModelOptions option)
    : Akonadi::EntityRightsFilterModel(parent)
    , d(new Private)
{
    setDynamicSortFilter(true);
    setFilterCaseSensitivity(Qt::CaseInsensitive);

    if (option & HideVirtualFolder) {
        d->hideVirtualFolder = true;
    }
    if (option & HideSpecificFolder) {
        d->hideSpecificFolder = true;
    }
    if (option & HideOutboxFolder) {
        d->hideOutboxFolder = true;
    }
    readConfig();
}

FolderTreeWidgetProxyModel::~FolderTreeWidgetProxyModel()
{
    delete d;
}

void FolderTreeWidgetProxyModel::readConfig()
{
    invalidate();
}

Qt::ItemFlags FolderTreeWidgetProxyModel::flags(const QModelIndex &index) const
{
    if (d->enableCheck) {
        const QModelIndex sourceIndex = mapToSource(index);
        const QModelIndex rowIndex = sourceIndex.sibling(sourceIndex.row(), 0);
        const Akonadi::Collection collection = sourceModel()->data(rowIndex, Akonadi::EntityTreeModel::CollectionRole).value<Akonadi::Collection>();
        if (!MailCommon::Util::isVirtualCollection(collection)) {
            const Akonadi::AgentInstance instance = Akonadi::AgentManager::self()->instance(collection.resource());
            if (instance.status() == Akonadi::AgentInstance::Broken) {
                return KRecursiveFilterProxyModel::flags(sourceIndex) & ~(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
            }
        }
        return Akonadi::EntityRightsFilterModel::flags(index);
    }
    return QSortFilterProxyModel::flags(index);
}

void FolderTreeWidgetProxyModel::setEnabledCheck(bool enable)
{
    if (d->enableCheck == enable) {
        return;
    }

    d->enableCheck = enable;
    if (enable) {
        setAccessRights(Akonadi::Collection::CanCreateItem | Akonadi::Collection::CanCreateCollection);
    }
}

bool FolderTreeWidgetProxyModel::enabledCheck() const
{
    return d->enableCheck;
}

void FolderTreeWidgetProxyModel::setHideVirtualFolder(bool exclude)
{
    if (d->hideVirtualFolder != exclude) {
        d->hideVirtualFolder = exclude;
        invalidate();
    }
}

bool FolderTreeWidgetProxyModel::hideVirtualFolder() const
{
    return d->hideVirtualFolder;
}

void FolderTreeWidgetProxyModel::setHideSpecificFolder(bool hide)
{
    if (d->hideSpecificFolder != hide) {
        d->hideSpecificFolder = hide;
        invalidate();
    }
}

bool FolderTreeWidgetProxyModel::hideSpecificFolder() const
{
    return d->hideSpecificFolder;
}

void FolderTreeWidgetProxyModel::setHideOutboxFolder(bool hide)
{
    if (d->hideOutboxFolder != hide) {
        d->hideOutboxFolder = hide;
        invalidate();
    }
}

bool FolderTreeWidgetProxyModel::hideOutboxFolder() const
{
    return d->hideOutboxFolder;
}

bool FolderTreeWidgetProxyModel::acceptRow(int sourceRow, const QModelIndex &sourceParent) const
{
    const QModelIndex modelIndex = sourceModel()->index(sourceRow, 0, sourceParent);

    const Akonadi::Collection collection
        = sourceModel()->data(
        modelIndex, Akonadi::EntityTreeModel::CollectionRole).value<Akonadi::Collection>();
    if (!d->checker.isWantedCollection(collection)) {
        return false;
    }

    if (d->hideVirtualFolder) {
        if (Util::isVirtualCollection(collection)) {
            return false;
        }
    }

    if (d->hideSpecificFolder) {
        const QSharedPointer<FolderSettings> col
            = FolderSettings::forCollection(collection, false);
        if (col && col->hideInSelectionDialog()) {
            return false;
        }
    }

    if (d->hideOutboxFolder) {
        if (collection == Kernel::self()->outboxCollectionFolder()) {
            return false;
        }
    }

    return KRecursiveFilterProxyModel::acceptRow(sourceRow, sourceParent);
}

QVariant FolderTreeWidgetProxyModel::data(const QModelIndex &index, int role) const
{
    if (role == Qt::TextColorRole) {
        const QModelIndex sourceIndex = mapToSource(index);
        const QModelIndex rowIndex = sourceIndex.sibling(sourceIndex.row(), 0);
        const Akonadi::Collection collection
            = sourceModel()->data(
            rowIndex, Akonadi::EntityTreeModel::CollectionRole).value<Akonadi::Collection>();

        if (!MailCommon::Util::isVirtualCollection(collection)) {
            const Akonadi::AgentInstance instance
                = Akonadi::AgentManager::self()->instance(collection.resource());

            if (instance.status() == Akonadi::AgentInstance::Broken) {
                return d->brokenAccountColor;
            }
        }
    } else if (role == Qt::DisplayRole) {
        const QModelIndex sourceIndex = mapToSource(index);
        const QModelIndex rowIndex = sourceIndex.sibling(sourceIndex.row(), 0);
        const Akonadi::Collection collection
            = sourceModel()->data(
            rowIndex, Akonadi::EntityTreeModel::CollectionRole).value<Akonadi::Collection>();
        if (!MailCommon::Util::isVirtualCollection(collection)) {
            const Akonadi::AgentInstance instance
                = Akonadi::AgentManager::self()->instance(collection.resource());
            if (collection.parentCollection() == Akonadi::Collection::root()) {
                if (!instance.isOnline()) {
                    return i18n("%1 (Offline)", Akonadi::EntityRightsFilterModel::data(index, role).toString());
                }
            }
        }
    }
    return Akonadi::EntityRightsFilterModel::data(index, role);
}

void FolderTreeWidgetProxyModel::updatePalette()
{
    if (MessageCore::MessageCoreSettings::self()->useDefaultColors()) {
        KColorScheme scheme(QPalette::Active, KColorScheme::View);
        d->brokenAccountColor = scheme.foreground(KColorScheme::NegativeText).color();
        invalidate();
    }
}

void FolderTreeWidgetProxyModel::addContentMimeTypeInclusionFilter(const QString &mimeType)
{
    d->includedMimeTypes << mimeType;
    d->checker.setWantedMimeTypes(d->includedMimeTypes.toList());
    invalidateFilter();
}
}
