/*
   SPDX-FileCopyrightText: 2016-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include <QObject>

class FilterActionMissingTemplateDialogTest : public QObject
{
    Q_OBJECT
public:
    explicit FilterActionMissingTemplateDialogTest(QObject *parent = nullptr);
    ~FilterActionMissingTemplateDialogTest() override;
private Q_SLOTS:
    void shouldHaveDefaultValue();
};
