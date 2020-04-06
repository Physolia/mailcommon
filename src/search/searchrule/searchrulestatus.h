/*
  Copyright (c) 2015-2020 Laurent Montel <montel@kde.org>

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

#ifndef SEARCHRULESTATUS_H
#define SEARCHRULESTATUS_H

#include <AkonadiCore/Item>

#include "mailcommon/searchpattern.h"
#include <Akonadi/KMime/MessageStatus>
namespace MailCommon {
//TODO: Check if the below one is needed or not!
// The below are used in several places and here so they are accessible.
struct MessageStatusInfo {
    const char *context;
    const char *text;
    const char *icon;
};

// If you change the ordering here; also do it in the enum below
static const MessageStatusInfo StatusValues[] = {
    { I18NC_NOOP("message status", "Important"), "emblem-important"    },
    { I18NC_NOOP("message status", "Action Item"), "mail-task"           },
    { I18NC_NOOP("message status", "Unread"), "mail-unread"         },
    { I18NC_NOOP("message status", "Read"), "mail-read"           },
    { I18NC_NOOP("message status", "Deleted"), "mail-deleted"        },
    { I18NC_NOOP("message status", "Replied"), "mail-replied"        },
    { I18NC_NOOP("message status", "Forwarded"), "mail-forwarded"      },
    { I18NC_NOOP("message status", "Queued"), "mail-queued"         },
    { I18NC_NOOP("message status", "Sent"), "mail-sent"           },
    { I18NC_NOOP("message status", "Watched"), "mail-thread-watch"   },
    { I18NC_NOOP("message status", "Ignored"), "mail-thread-ignored" },
    { I18NC_NOOP("message status", "Spam"), "mail-mark-junk"      },
    { I18NC_NOOP("message status", "Ham"), "mail-mark-notjunk"   },
    { I18NC_NOOP("message status", "Has Attachment"), "mail-attachment"     }  //must be last
};

static const int StatusValueCount
    = sizeof(StatusValues) / sizeof(MessageStatusInfo);
// we want to show all status entries in the quick search bar, but only the
// ones up to attachment in the search/filter dialog, because there the
// attachment case is handled separately.
static const int StatusValueCountWithoutHidden = StatusValueCount - 1;

/**
 *  This class represents a search to be performed against the status of a
 *  messsage. The status is represented by a bitfield.
 *
 *  @short This class represents a search pattern rule operating on message
 *  status.
 */
class MAILCOMMON_EXPORT SearchRuleStatus : public SearchRule
{
public:
    explicit SearchRuleStatus(const QByteArray &field = QByteArray(), Function function = FuncContains, const QString &contents = QString());

    explicit SearchRuleStatus(Akonadi::MessageStatus status, Function function = FuncContains);

    bool isEmpty() const override;
    bool matches(const Akonadi::Item &item) const override;

    /**
     * @copydoc SearchRule::requiredPart()
     */
    RequiredPart requiredPart() const override;

    void addQueryTerms(Akonadi::SearchTerm &groupTerm, bool &emptyIsNotAnError) const override;

    //Not possible to implement optimized form for status searching
    using SearchRule::matches;

    static Akonadi::MessageStatus statusFromEnglishName(const QString &);

    QString informationAboutNotValidRules() const override;

private:
    Akonadi::MessageStatus mStatus;
};
}

#endif // SEARCHRULESTATUS_H
