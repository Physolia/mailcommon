/*
  Copyright (c) 2015-2019 Montel Laurent <montel@kde.org>

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

#ifndef INVALIDFILTERINFOWIDGET_H
#define INVALIDFILTERINFOWIDGET_H

#include <KMessageWidget>
#include "mailcommon_private_export.h"
namespace MailCommon {
class MAILCOMMON_TESTS_EXPORT InvalidFilterInfoWidget : public KMessageWidget
{
    Q_OBJECT
public:
    explicit InvalidFilterInfoWidget(QWidget *parent = nullptr);
    ~InvalidFilterInfoWidget();
public Q_SLOTS:
    void slotShowDetails(const QString &details);
};
}

#endif // INVALIDFILTERINFOWIDGET_H
