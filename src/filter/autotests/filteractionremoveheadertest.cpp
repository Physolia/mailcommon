/*
  Copyright (c) 2015-2020 Laurent Montel <montel@kde.org>

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

#include "filteractionremoveheadertest.h"
#include "../filteractions/filteractionremoveheader.h"
#include <QTest>
#include <QComboBox>

FilterActionRemoveHeaderTest::FilterActionRemoveHeaderTest(QObject *parent)
    : QObject(parent)
{
}

FilterActionRemoveHeaderTest::~FilterActionRemoveHeaderTest()
{
}

void FilterActionRemoveHeaderTest::shouldHaveDefaultValue()
{
    MailCommon::FilterActionRemoveHeader filter;
    QWidget *w = filter.createParamWidget(nullptr);
    const auto comboBox = dynamic_cast<QComboBox *>(w);
    QVERIFY(comboBox);
    QVERIFY(comboBox->isEditable());
    QVERIFY(comboBox->count() > 0);
}

void FilterActionRemoveHeaderTest::shouldHaveSieveRequires()
{
    MailCommon::FilterActionRemoveHeader filter;
    QCOMPARE(filter.sieveRequires(), QStringList() << QStringLiteral("editheader"));
}

void FilterActionRemoveHeaderTest::shouldBeEmpty()
{
    MailCommon::FilterActionRemoveHeader filter;
    QVERIFY(filter.isEmpty());
    filter.argsFromString(QStringLiteral("bla"));
    QVERIFY(!filter.isEmpty());
}

void FilterActionRemoveHeaderTest::shouldNotExecuteActionWhenParameterIsEmpty()
{
    MailCommon::FilterActionRemoveHeader filter(this);
    KMime::Message::Ptr msgPtr = KMime::Message::Ptr(new KMime::Message());
    Akonadi::Item item;
    item.setPayload<KMime::Message::Ptr>(msgPtr);
    MailCommon::ItemContext context(item, true);

    filter.argsFromString(QString());
    QCOMPARE(filter.process(context, false), MailCommon::FilterAction::ErrorButGoOn);
    QCOMPARE(context.needsPayloadStore(), false);
}

void FilterActionRemoveHeaderTest::shouldRemoveHeader()
{
    const QByteArray data = "From: foo@kde.org\n"
                            "To: foo@kde.org\n"
                            "Subject: test\n"
                            "testheader: foo\n"
                            "Date: Wed, 01 Apr 2015 09:33:01 +0200\n"
                            "MIME-Version: 1.0\n"
                            "\n"
                            "test";
    const QByteArray output = "From: foo@kde.org\n"
                              "To: foo@kde.org\n"
                              "Subject: test\n"
                              "Date: Wed, 01 Apr 2015 09:33:01 +0200\n"
                              "MIME-Version: 1.0\n"
                              "\n"
                              "test";

    MailCommon::FilterActionRemoveHeader filter(this);
    KMime::Message::Ptr msgPtr = KMime::Message::Ptr(new KMime::Message());
    msgPtr->setContent(data);
    msgPtr->parse();
    Akonadi::Item item;
    item.setPayload<KMime::Message::Ptr>(msgPtr);
    MailCommon::ItemContext context(item, true);

    filter.argsFromString(QStringLiteral("testheader"));
    QCOMPARE(filter.process(context, false), MailCommon::FilterAction::GoOn);
    QCOMPARE(context.needsPayloadStore(), true);
    QCOMPARE(msgPtr->encodedContent(), output);
}

void FilterActionRemoveHeaderTest::shouldNotTryToRemoveHeaderWhenItDoesntExist()
{
    const QByteArray data = "From: foo@kde.org\n"
                            "To: foo@kde.org\n"
                            "Subject: test\n"
                            "testheader2: foo\n"
                            "Date: Wed, 01 Apr 2015 09:33:01 +0200\n"
                            "MIME-Version: 1.0\n"
                            "\n"
                            "test";

    MailCommon::FilterActionRemoveHeader filter(this);
    KMime::Message::Ptr msgPtr = KMime::Message::Ptr(new KMime::Message());
    msgPtr->setContent(data);
    msgPtr->parse();
    Akonadi::Item item;
    item.setPayload<KMime::Message::Ptr>(msgPtr);
    MailCommon::ItemContext context(item, true);

    filter.argsFromString(QStringLiteral("testheader"));
    QCOMPARE(filter.process(context, false), MailCommon::FilterAction::GoOn);
    QCOMPARE(context.needsPayloadStore(), false);
    QCOMPARE(msgPtr->encodedContent(), data);
}

void FilterActionRemoveHeaderTest::shouldRemoveMultiHeader()
{
    const QByteArray data = "From: foo@kde.org\n"
                            "To: foo@kde.org\n"
                            "Subject: test\n"
                            "testheader: foo\n"
                            "testheader: foo\n"
                            "testheader: bla\n"
                            "Date: Wed, 01 Apr 2015 09:33:01 +0200\n"
                            "MIME-Version: 1.0\n"
                            "\n"
                            "test";
    const QByteArray output = "From: foo@kde.org\n"
                              "To: foo@kde.org\n"
                              "Subject: test\n"
                              "Date: Wed, 01 Apr 2015 09:33:01 +0200\n"
                              "MIME-Version: 1.0\n"
                              "\n"
                              "test";

    MailCommon::FilterActionRemoveHeader filter(this);
    KMime::Message::Ptr msgPtr = KMime::Message::Ptr(new KMime::Message());
    msgPtr->setContent(data);
    msgPtr->parse();
    Akonadi::Item item;
    item.setPayload<KMime::Message::Ptr>(msgPtr);
    MailCommon::ItemContext context(item, true);

    filter.argsFromString(QStringLiteral("testheader"));
    QCOMPARE(filter.process(context, false), MailCommon::FilterAction::GoOn);
    QCOMPARE(context.needsPayloadStore(), true);
    QCOMPARE(msgPtr->encodedContent(), output);
}

void FilterActionRemoveHeaderTest::shouldHaveRequiredPart()
{
    MailCommon::FilterActionRemoveHeader filter;
    QCOMPARE(filter.requiredPart(), MailCommon::SearchRule::CompleteMessage);
}

QTEST_MAIN(FilterActionRemoveHeaderTest)
