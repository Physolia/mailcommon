/*
  SPDX-FileCopyrightText: 2015-2025 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once
#include "mailcommon_private_export.h"
#include "searchpattern.h"
#include <Akonadi/Item>

/**
 * @short This class represents a search pattern rule operating on a string.
 *
 * This class represents a search to be performed against a string.
 * The string can be either a message header, or a pseudo header, such
 * as \<body\>
 */
namespace MailCommon
{
class MAILCOMMON_TESTS_EXPORT SearchRuleString : public SearchRule
{
public:
    /**
     * Creates new new string search rule.
     *
     * @param field The field to search in.
     * @param function The function to use for searching.
     * @param contents The contents to search for.
     */
    explicit SearchRuleString(const QByteArray &field = QByteArray(), Function function = FuncContains, const QString &contents = QString());

    /**
     * Creates a new string search rule from an @p other rule.
     */
    SearchRuleString(const SearchRuleString &other);

    /**
     * Initializes this rule with an @p other rule.
     */
    const SearchRuleString &operator=(const SearchRuleString &other);

    /**
     * Destroys the string search rule.
     */
    ~SearchRuleString() override;

    /**
     * @copydoc SearchRule::isEmpty()
     */
    [[nodiscard]] bool isEmpty() const override;

    /**
     * @copydoc SearchRule::requiredPart()
     */
    [[nodiscard]] RequiredPart requiredPart() const override;

    /**
     * @copydoc SearchRule::matches()
     */
    [[nodiscard]] bool matches(const Akonadi::Item &item) const override;

    /**
     * A helper method for the main matches() method.
     * Does the actual comparing.
     */
    [[nodiscard]] bool matchesInternal(const QString &contents) const;

    /**
     * @copydoc SearchRule::addQueryTerms()
     */
    void addQueryTerms(Akonadi::SearchTerm &groupTerm, bool &emptyIsNotAnError) const override;
    [[nodiscard]] QString informationAboutNotValidRules() const override;
};
}
