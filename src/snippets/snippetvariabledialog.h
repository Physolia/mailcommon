/*
  Copyright (C) 2010 Klarälvdalens Datakonsult AB, a KDAB Group company, info@kdab.net,
    Author: Tobias Koenig <tokoe@kdab.com>

  Copyright (c) 2019-2020 Laurent Montel <montel@kde.org>

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

#ifndef MAILCOMMON_SNIPPETVARIABLEDIALOG_P_H
#define MAILCOMMON_SNIPPETVARIABLEDIALOG_P_H

#include <QDialog>

class QCheckBox;
namespace KPIMTextEdit {
class PlainTextEditorWidget;
}
namespace MailCommon {
class SnippetVariableDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SnippetVariableDialog(const QString &variableName, QMap<QString, QString> *variables, QWidget *parent = nullptr);
    ~SnippetVariableDialog();

    Q_REQUIRED_RESULT QString variableValue() const;
    Q_REQUIRED_RESULT bool saveVariableIsChecked() const;

private:
    void writeConfig();
    void readConfig();
    void slotAccepted();
    QString mVariableName;
    QMap<QString, QString> *mVariables = nullptr;
    KPIMTextEdit::PlainTextEditorWidget *mVariableValueText = nullptr;
    QCheckBox *mSaveVariable = nullptr;
};
}

#endif
