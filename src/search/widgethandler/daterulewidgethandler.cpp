/*
  SPDX-FileCopyrightText: 2013-2025 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "daterulewidgethandler.h"

#include <KDateComboBox>
#include <KLocalizedString>

#include <KLazyLocalizedString>
#include <QComboBox>
#include <QDate>
#include <QObject>
#include <QStackedWidget>
using namespace MailCommon;

static const struct {
    SearchRule::Function id;
    const KLazyLocalizedString displayName;
} DateFunctions[] = {{SearchRule::FuncEquals, kli18n("is equal to")},
                     {SearchRule::FuncNotEqual, kli18n("is not equal to")},
                     {SearchRule::FuncIsGreater, kli18n("is after")},
                     {SearchRule::FuncIsLessOrEqual, kli18n("is before or equal to")},
                     {SearchRule::FuncIsLess, kli18n("is before")},
                     {SearchRule::FuncIsGreaterOrEqual, kli18n("is after or equal to")}};
static const int DateFunctionCount = sizeof(DateFunctions) / sizeof(*DateFunctions);

//---------------------------------------------------------------------------

QWidget *DateRuleWidgetHandler::createFunctionWidget(int number, QStackedWidget *functionStack, const QObject *receiver, bool /*isBalooSearch*/) const
{
    if (number != 0) {
        return nullptr;
    }

    auto funcCombo = new QComboBox(functionStack);
    funcCombo->setMinimumWidth(50);
    funcCombo->setObjectName(QLatin1StringView("dateRuleFuncCombo"));
    for (int i = 0; i < DateFunctionCount; ++i) {
        funcCombo->addItem(DateFunctions[i].displayName.toString());
    }
    funcCombo->adjustSize();
    QObject::connect(funcCombo, SIGNAL(activated(int)), receiver, SLOT(slotFunctionChanged()));
    return funcCombo;
}

//---------------------------------------------------------------------------

QWidget *DateRuleWidgetHandler::createValueWidget(int number, QStackedWidget *valueStack, const QObject *receiver) const
{
    if (number != 0) {
        return nullptr;
    }

    auto dateCombo = new KDateComboBox(valueStack);
    dateCombo->setObjectName(QLatin1StringView("KDateComboBox"));
    dateCombo->setOptions(KDateComboBox::SelectDate | KDateComboBox::DatePicker | KDateComboBox::DateKeywords);
    QObject::connect(dateCombo, SIGNAL(dateChanged(QDate)), receiver, SLOT(slotValueChanged()));
    return dateCombo;
}

//---------------------------------------------------------------------------

SearchRule::Function DateRuleWidgetHandler::currentFunction(const QStackedWidget *functionStack) const
{
    const auto funcCombo = functionStack->findChild<QComboBox *>(QStringLiteral("dateRuleFuncCombo"));

    if (funcCombo && funcCombo->currentIndex() >= 0) {
        return DateFunctions[funcCombo->currentIndex()].id;
    }

    return SearchRule::FuncNone;
}

//---------------------------------------------------------------------------

SearchRule::Function DateRuleWidgetHandler::function(const QByteArray &field, const QStackedWidget *functionStack) const
{
    if (!handlesField(field)) {
        return SearchRule::FuncNone;
    }

    return currentFunction(functionStack);
}

//---------------------------------------------------------------------------

QString DateRuleWidgetHandler::currentValue(const QStackedWidget *valueStack) const
{
    const KDateComboBox *dateInput = valueStack->findChild<KDateComboBox *>(QStringLiteral("KDateComboBox"));

    if (dateInput) {
        return dateInput->date().toString(Qt::ISODate);
    }

    return {};
}

//---------------------------------------------------------------------------

QString DateRuleWidgetHandler::value(const QByteArray &field, const QStackedWidget *, const QStackedWidget *valueStack) const
{
    if (!handlesField(field)) {
        return {};
    }

    return currentValue(valueStack);
}

//---------------------------------------------------------------------------

QString DateRuleWidgetHandler::prettyValue(const QByteArray &field, const QStackedWidget *, const QStackedWidget *valueStack) const
{
    if (!handlesField(field)) {
        return {};
    }

    return currentValue(valueStack);
}

//---------------------------------------------------------------------------

bool DateRuleWidgetHandler::handlesField(const QByteArray &field) const
{
    return field == "<date>";
}

//---------------------------------------------------------------------------

void DateRuleWidgetHandler::reset(QStackedWidget *functionStack, QStackedWidget *valueStack) const
{
    // reset the function combo box
    const auto funcCombo = functionStack->findChild<QComboBox *>(QStringLiteral("dateRuleFuncCombo"));

    if (funcCombo) {
        funcCombo->blockSignals(true);
        funcCombo->setCurrentIndex(0);
        funcCombo->blockSignals(false);
    }

    // reset the value widget
    auto dateInput = valueStack->findChild<KDateComboBox *>(QStringLiteral("KDateComboBox"));

    if (dateInput) {
        dateInput->blockSignals(true);
        dateInput->setDate(QDate::currentDate());
        dateInput->blockSignals(false);
    }
}

//---------------------------------------------------------------------------

bool DateRuleWidgetHandler::setRule(QStackedWidget *functionStack, QStackedWidget *valueStack, const SearchRule::Ptr rule, bool /*isBalooSearch*/) const
{
    if (!rule || !handlesField(rule->field())) {
        reset(functionStack, valueStack);
        return false;
    }

    // set the function
    const SearchRule::Function func = rule->function();
    int funcIndex = 0;
    for (; funcIndex < DateFunctionCount; ++funcIndex) {
        if (func == DateFunctions[funcIndex].id) {
            break;
        }
    }

    const auto funcCombo = functionStack->findChild<QComboBox *>(QStringLiteral("dateRuleFuncCombo"));

    if (funcCombo) {
        funcCombo->blockSignals(true);
        if (funcIndex < DateFunctionCount) {
            funcCombo->setCurrentIndex(funcIndex);
        } else {
            funcCombo->setCurrentIndex(0);
        }
        funcCombo->blockSignals(false);
        functionStack->setCurrentWidget(funcCombo);
    }

    // set the value
    const QString value = rule->contents();

    auto dateInput = valueStack->findChild<KDateComboBox *>(QStringLiteral("KDateComboBox"));

    if (dateInput) {
        dateInput->blockSignals(true);
        dateInput->setDate(QDate::fromString(value, Qt::ISODate));
        dateInput->blockSignals(false);
        valueStack->setCurrentWidget(dateInput);
    }
    return true;
}

//---------------------------------------------------------------------------

bool DateRuleWidgetHandler::update(const QByteArray &field, QStackedWidget *functionStack, QStackedWidget *valueStack) const
{
    if (!handlesField(field)) {
        return false;
    }

    // raise the correct function widget
    functionStack->setCurrentWidget(functionStack->findChild<QWidget *>(QStringLiteral("dateRuleFuncCombo")));

    // raise the correct value widget
    auto dateInput = valueStack->findChild<KDateComboBox *>(QStringLiteral("KDateComboBox"));

    if (dateInput) {
        valueStack->setCurrentWidget(dateInput);
    }
    return true;
}
