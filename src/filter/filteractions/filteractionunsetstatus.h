/*

  Copyright (c) 2012-2017 Montel Laurent <montel@kde.org>

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

#ifndef FILTERACTIONUNSETSTATUS_H
#define FILTERACTIONUNSETSTATUS_H

#include "filteractionstatus.h"
namespace MailCommon {
class FilterActionUnsetStatus : public FilterActionStatus
{
    Q_OBJECT
public:
    explicit FilterActionUnsetStatus(QObject *parent = nullptr);
    ReturnCode process(ItemContext &context, bool applyOnOutbound) const override;
    SearchRule::RequiredPart requiredPart() const override;

    static FilterAction *newAction();
    QString sieveCode() const override;
    QStringList sieveRequires() const override;
    bool isEmpty() const override;
    QString informationAboutNotValidAction() const override;
private:
    bool checkIsValid(int &index) const;
};
}

#endif // FILTERACTIONUNSETSTATUS_H
