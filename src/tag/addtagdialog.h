/*
  SPDX-FileCopyrightText: 2012-2023 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once

#include "mailcommon_export.h"
#include "tag.h"
#include <Akonadi/Tag>
#include <Akonadi/TagCreateJob>
#include <QDialog>
#include <QVector>

class KActionCollection;

namespace MailCommon
{
class AddTagDialogPrivate;
class MAILCOMMON_EXPORT AddTagDialog : public QDialog
{
    Q_OBJECT
public:
    explicit AddTagDialog(const QList<KActionCollection *> &actions, QWidget *parent = nullptr);
    ~AddTagDialog() override;

    void setTags(const QVector<MailCommon::Tag::Ptr> &tags);
    Q_REQUIRED_RESULT QString label() const;
    Q_REQUIRED_RESULT Akonadi::Tag tag() const;

private:
    void slotSave();
    void slotTagNameChanged(const QString &text);
    void onTagCreated(KJob *job);
    std::unique_ptr<AddTagDialogPrivate> const d;
};
}
