/*
  SPDX-FileCopyrightText: 2013-2025 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once

#include "mailcommon_private_export.h"
#include <KSyntaxHighlighting/Repository>
#include <QDialog>
namespace TextCustomEditor
{
class PlainTextEditorWidget;
}
namespace PimCommon
{
class PurposeMenuMessageWidget;
}
namespace MailCommon
{
class MAILCOMMON_TESTS_EXPORT FilterConvertToSieveResultDialog : public QDialog
{
    Q_OBJECT
public:
    explicit FilterConvertToSieveResultDialog(QWidget *parent = nullptr);
    ~FilterConvertToSieveResultDialog() override;

    void setCode(const QString &code);

private:
    void slotSave();
    void readConfig();
    void writeConfig();
    TextCustomEditor::PlainTextEditorWidget *const mEditor;
    PimCommon::PurposeMenuMessageWidget *const mPurposeMenuMessageWidget;
    KSyntaxHighlighting::Repository mSyntaxRepo;
};
}
