/*
  Copyright (c) 2009-2019 Montel Laurent <montel@kde.org>

  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful, but
  WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  General Public License for more details.

  You should have received a copy of the GNU General Public License along
  with this program; if not, write to the Free Software Foundation, Inc.,
  51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
*/

#ifndef MAILCOMMON_FOLDERSETTINGS_H
#define MAILCOMMON_FOLDERSETTINGS_H

#include "mailcommon_export.h"
#include <MessageViewer/Viewer>

#include <MessageCore/MailingList>
using MessageCore::MailingList;

#include <Collection>
#include <CollectionStatistics>

#include <KSharedConfig>
#include <QKeySequence>
#include <KIO/Job>

namespace MailCommon {
class MAILCOMMON_EXPORT FolderSettings : public QObject
{
    Q_OBJECT

public:
    static QSharedPointer<FolderSettings> forCollection(
        const Akonadi::Collection &coll, bool writeConfig = true);

    ~FolderSettings();

    void setCollection(const Akonadi::Collection &collection);

    static QString configGroupName(const Akonadi::Collection &col);
    static void clearCache();
    static void resetHtmlFormat();

    Q_REQUIRED_RESULT bool isWriteConfig() const;
    void setWriteConfig(bool writeConfig);

    void writeConfig() const;
    void readConfig();

    Q_REQUIRED_RESULT QString name() const;

    Q_REQUIRED_RESULT bool isReadOnly() const;

    Q_REQUIRED_RESULT bool isStructural() const;

    Q_REQUIRED_RESULT bool isSystemFolder() const;

    Q_REQUIRED_RESULT qint64 count() const;

    Q_REQUIRED_RESULT bool canDeleteMessages() const;

    Q_REQUIRED_RESULT bool canCreateMessages() const;

    Q_REQUIRED_RESULT bool isValid() const;

    Q_REQUIRED_RESULT Akonadi::Collection::Rights rights() const;

    Q_REQUIRED_RESULT Akonadi::CollectionStatistics statistics() const;

    void setShortcut(const QKeySequence &);
    const QKeySequence &shortcut() const;

    /**
     *  Get / set whether the default identity should be used instead of the
     *  identity specified by setIdentity().
     */
    void setUseDefaultIdentity(bool useDefaultIdentity);
    Q_REQUIRED_RESULT bool useDefaultIdentity() const;

    void setIdentity(uint identity);
    Q_REQUIRED_RESULT uint identity() const;

    /**
     * Returns true if this folder is associated with a mailing-list.
     */
    void setMailingListEnabled(bool enabled);
    Q_REQUIRED_RESULT bool isMailingListEnabled() const;

    void setMailingList(const MailingList &mlist);

    MailingList mailingList() const;

    /**
     * Returns true if the replies to mails from this folder should be
     * put in the same folder.
     */
    Q_REQUIRED_RESULT bool putRepliesInSameFolder() const;
    void setPutRepliesInSameFolder(bool b);

    /**
     * Returns true if this folder should be hidden from all folder selection dialogs
     */
    Q_REQUIRED_RESULT bool hideInSelectionDialog() const;
    void setHideInSelectionDialog(bool hide);

    Q_REQUIRED_RESULT QString mailingListPostAddress() const;

    Q_REQUIRED_RESULT uint fallBackIdentity() const;
    Q_REQUIRED_RESULT MessageViewer::Viewer::DisplayFormatMessage formatMessage() const;
    void setFormatMessage(MessageViewer::Viewer::DisplayFormatMessage formatMessage);

protected Q_SLOTS:
    void slotIdentitiesChanged();

private:
    explicit FolderSettings(const Akonadi::Collection &col, bool writeconfig);
    QString resource() const;

    Akonadi::Collection mCollection;

    /** Mailing list attributes */
    bool mMailingListEnabled;
    MailingList mMailingList;

    bool mUseDefaultIdentity;
    uint mIdentity;

    MessageViewer::Viewer::DisplayFormatMessage mFormatMessage;
    /** Should replies to messages in this folder be put in here? */
    bool mPutRepliesInSameFolder;

    /** Should this folder be hidden in the folder selection dialog? */
    bool mHideInSelectionDialog;

    /** shortcut associated with this folder or null, if none is configured. */
    QKeySequence mShortcut;
    bool mWriteConfig;
};
}

#endif
