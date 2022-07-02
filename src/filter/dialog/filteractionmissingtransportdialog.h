/*
   SPDX-FileCopyrightText: 2016-2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include <QDialog>
namespace MailTransport
{
class TransportComboBox;
}

namespace MailCommon
{
class FilterActionMissingTransportDialog : public QDialog
{
    Q_OBJECT
public:
    explicit FilterActionMissingTransportDialog(const QString &filtername, QWidget *parent = nullptr);
    ~FilterActionMissingTransportDialog() override;
    Q_REQUIRED_RESULT int selectedTransport() const;

private:
    void writeConfig();
    void readConfig();
    MailTransport::TransportComboBox *const mComboBoxTransport;
};
}
