/*
  Copyright (c) 2013-2018 Montel Laurent <montel@kde.org>

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

#include "filterconverttosieve.h"
#include "filterconverttosieveresultdialog.h"
#include "filter/mailfilter.h"

#include <QPointer>

using namespace MailCommon;

FilterConvertToSieve::FilterConvertToSieve(const QList<MailFilter *> &filters)
    : mListFilters(filters)
{
}

FilterConvertToSieve::~FilterConvertToSieve()
{
}

void FilterConvertToSieve::convert()
{
    QString result;
    if (!mListFilters.isEmpty()) {
        QStringList requiresModule;
        QString code;
        for (MailFilter *filter : qAsConst(mListFilters)) {
            filter->generateSieveScript(requiresModule, code);
            code += QLatin1Char('\n');
        }
        QString requireStr;
        for (const QString &require : qAsConst(requiresModule)) {
            requireStr += QStringLiteral("require \"%1\";").arg(require);
            requireStr += QLatin1Char('\n');
        }
        result = requireStr + code;
    }
    QPointer<FilterConvertToSieveResultDialog> dlg = new FilterConvertToSieveResultDialog;
    dlg->setCode(result);
    dlg->exec();
    delete dlg;
}
