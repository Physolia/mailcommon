/*
  Copyright (C) 2010 Klarälvdalens Datakonsult AB, a KDAB Group company, info@kdab.net,
    Author Tobias Koenig <tokoe@kdab.com>

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

#ifndef MAILCOMMON_SENDMDNHANDLER_H
#define MAILCOMMON_SENDMDNHANDLER_H

#include "mailcommon_export.h"

#include <MessageViewer/Viewer>

#include <QObject>

namespace MailCommon {
class IKernel;

/**
 * @short A viewer handler to send MDN for viewed messages.
 */
class MAILCOMMON_EXPORT SendMdnHandler : public QObject, public MessageViewer::AbstractMessageLoadedHandler
{
    Q_OBJECT

public:
    /**
     * Creates a new send mnd handler.
     *
     * @param kernel The mail kernel object that will be used.
     * @param parent The parent object.
     */
    explicit SendMdnHandler(IKernel *kernel, QObject *parent = nullptr);

    /**
     * Destroys the send mdn handler.
     */
    ~SendMdnHandler() override;

    /**
     * @copydoc AbstractMessageLoadedHandler::setItem()
     */
    void setItem(const Akonadi::Item &item) override;

private:
    //@cond PRIVATE
    class Private;
    Private *const d;
    //@endcond
};
}

#endif
