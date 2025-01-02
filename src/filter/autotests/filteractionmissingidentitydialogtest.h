/*
   SPDX-FileCopyrightText: 2016-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include <QObject>

class FilterActionMissingIdentityDialogTest : public QObject
{
    Q_OBJECT
public:
    explicit FilterActionMissingIdentityDialogTest(QObject *parent = nullptr);
    ~FilterActionMissingIdentityDialogTest() override;
private Q_SLOTS:
    void initTestCase();
    void shouldHaveDefaultValue();
};
