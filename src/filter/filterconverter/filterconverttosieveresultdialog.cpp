/*
  SPDX-FileCopyrightText: 2013-2023 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "filterconverttosieveresultdialog.h"
#include "filterconverttosievepurposemenuwidget.h"
#include <KPIMTextEdit/PlainTextEditor>
#include <KPIMTextEdit/PlainTextEditorWidget>
#include <PimCommon/PimUtil>

#include <KLocalizedString>
#include <KSyntaxHighlighting/Definition>
#include <KSyntaxHighlighting/SyntaxHighlighter>
#include <KSyntaxHighlighting/Theme>

#include <KConfigGroup>
#include <KSharedConfig>
#include <KWindowConfig>
#include <QDialogButtonBox>
#include <QPushButton>
#include <QVBoxLayout>
#include <QWindow>

using namespace MailCommon;
namespace
{
static const char myFilterConvertToSieveResultDialogName[] = "FilterConvertToSieveResultDialog";
}
FilterConvertToSieveResultDialog::FilterConvertToSieveResultDialog(QWidget *parent)
    : QDialog(parent)
    , mEditor(new KPIMTextEdit::PlainTextEditorWidget(this))
{
    setWindowTitle(i18nc("@title:window", "Convert to Sieve Script"));
    auto topLayout = new QVBoxLayout(this);
    auto buttonBox = new QDialogButtonBox(QDialogButtonBox::Close, this);
    buttonBox->setObjectName(QStringLiteral("buttonbox"));
    auto saveButton = new QPushButton(this);
    buttonBox->addButton(saveButton, QDialogButtonBox::ActionRole);
    connect(buttonBox, &QDialogButtonBox::rejected, this, &FilterConvertToSieveResultDialog::reject);
    saveButton->setText(i18n("Save..."));
    saveButton->setIcon(QIcon::fromTheme("document-save"));
    saveButton->setObjectName(QStringLiteral("savebutton"));
    saveButton->setDefault(true);
    setModal(true);
    connect(saveButton, &QPushButton::clicked, this, &FilterConvertToSieveResultDialog::slotSave);

    mEditor->editor()->setSpellCheckingSupport(false);
    mEditor->setObjectName(QStringLiteral("editor"));
    auto syntaxHighlighter = new KSyntaxHighlighting::SyntaxHighlighter(mEditor->editor()->document());
    syntaxHighlighter->setDefinition(mSyntaxRepo.definitionForName(QStringLiteral("Sieve")));
    syntaxHighlighter->setTheme((palette().color(QPalette::Base).lightness() < 128) ? mSyntaxRepo.defaultTheme(KSyntaxHighlighting::Repository::DarkTheme)
                                                                                    : mSyntaxRepo.defaultTheme(KSyntaxHighlighting::Repository::LightTheme));
    topLayout->addWidget(mEditor);
    topLayout->addWidget(buttonBox);

    auto purposeMenu = new FilterconverttosievePurposeMenuWidget(this, this);
    auto shareButton = new QPushButton(i18n("Share..."), this);
    shareButton->setMenu(purposeMenu->menu());
    shareButton->setIcon(QIcon::fromTheme(QStringLiteral("document-share")));
    purposeMenu->setEditorWidget(mEditor->editor());
    buttonBox->addButton(shareButton, QDialogButtonBox::ActionRole);

    readConfig();
}

FilterConvertToSieveResultDialog::~FilterConvertToSieveResultDialog()
{
    writeConfig();
}

void FilterConvertToSieveResultDialog::slotSave()
{
    const QString filter = i18n("Sieve Files (*.siv);;All Files (*)");
    PimCommon::Util::saveTextAs(mEditor->editor()->toPlainText(), filter, this, QUrl(), i18nc("@title:window", "Convert to Script Sieve"));
}

void FilterConvertToSieveResultDialog::setCode(const QString &code)
{
    mEditor->editor()->setPlainText(code);
}

void FilterConvertToSieveResultDialog::readConfig()
{
    create(); // ensure a window is created
    windowHandle()->resize(QSize(500, 300));
    KConfigGroup group(KSharedConfig::openStateConfig(), myFilterConvertToSieveResultDialogName);
    KWindowConfig::restoreWindowSize(windowHandle(), group);
    resize(windowHandle()->size()); // workaround for QTBUG-40584
}

void FilterConvertToSieveResultDialog::writeConfig()
{
    KConfigGroup group(KSharedConfig::openStateConfig(), myFilterConvertToSieveResultDialogName);
    KWindowConfig::saveWindowSize(windowHandle(), group);
    group.sync();
}
