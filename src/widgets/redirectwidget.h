/*
  Copyright (c) 2015-2017 Montel Laurent <montel@kde.org>

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

#ifndef REDIRECTWIDGET_H
#define REDIRECTWIDGET_H

#include <QWidget>

namespace MessageComposer {
class ComposerLineEdit;
}

namespace MailCommon {
class RedirectWidget : public QWidget
{
    Q_OBJECT
public:
    explicit RedirectWidget(QWidget *parent = nullptr);
    ~RedirectWidget();

    void setFocus();
    QString resend();

Q_SIGNALS:
    void addressChanged(const QString &);

private:
    void slotAddressSelection();
    MessageComposer::ComposerLineEdit *mEdit;
    QString mResendStr;
};
}

#endif // REDIRECTWIDGET_H
