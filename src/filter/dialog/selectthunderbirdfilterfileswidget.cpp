/*
  SPDX-FileCopyrightText: 2012-2025 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "selectthunderbirdfilterfileswidget.h"
#include "ui_selectthunderbirdfilterfileswidget.h"
#include <MailImporter/FilterThunderbird>
#include <QAbstractButton>
#include <QListWidgetItem>

#include <KLocalizedString>
using namespace MailCommon;

SelectThunderbirdFilterFilesWidget::SelectThunderbirdFilterFilesWidget(const QString &defaultSettingPath, QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::SelectThunderbirdFilterFilesWidget)
{
    ui->setupUi(this);
    connect(ui->buttonGroup, &QButtonGroup::buttonClicked, this, &SelectThunderbirdFilterFilesWidget::slotButtonClicked);
    connect(ui->profiles, &KComboBox::currentIndexChanged, this, &SelectThunderbirdFilterFilesWidget::slotProfileChanged);

    ui->listFiles->setSelectionMode(QAbstractItemView::MultiSelection);
    QString defaultProfile;
    QMap<QString, QString> listProfile = MailImporter::FilterThunderbird::listProfile(defaultProfile, defaultSettingPath);
    QMap<QString, QString>::const_iterator i = listProfile.constBegin();
    while (i != listProfile.constEnd()) {
        QString name = i.key();
        if (i.value() == defaultProfile) {
            name += i18n(" (default)");
        }
        ui->profiles->addItem(name, i.value());
        ++i;
    }
    ui->fileUrl->setEnabled(true);
    ui->profiles->setEnabled(false);
    ui->listFiles->setEnabled(false);
    slotProfileChanged(0);
    connect(ui->fileUrl, &KUrlRequester::textChanged, this, &SelectThunderbirdFilterFilesWidget::slotUrlChanged);
    connect(ui->listFiles, &QListWidget::itemSelectionChanged, this, &SelectThunderbirdFilterFilesWidget::slotItemSelectionChanged);
}

SelectThunderbirdFilterFilesWidget::~SelectThunderbirdFilterFilesWidget()
{
    delete ui;
}

void SelectThunderbirdFilterFilesWidget::slotItemSelectionChanged()
{
    Q_EMIT enableOkButton(!ui->listFiles->selectedItems().isEmpty());
}

void SelectThunderbirdFilterFilesWidget::slotUrlChanged(const QString &path)
{
    Q_EMIT enableOkButton(!path.isEmpty());
}

void SelectThunderbirdFilterFilesWidget::slotButtonClicked(QAbstractButton *button)
{
    if (button == ui->selectFile) {
        ui->fileUrl->setEnabled(true);
        ui->profiles->setEnabled(false);
        ui->listFiles->setEnabled(false);
        Q_EMIT enableOkButton(!ui->fileUrl->text().isEmpty());
    } else {
        ui->fileUrl->setEnabled(false);
        ui->profiles->setEnabled(true);
        ui->listFiles->setEnabled(true);
        slotItemSelectionChanged();
    }
}

void SelectThunderbirdFilterFilesWidget::slotProfileChanged(int index)
{
    if (index >= ui->profiles->count()) {
        return;
    }

    QStringList listFilterFiles;
    const QString path(MailImporter::FilterThunderbird::defaultSettingsPath() + ui->profiles->itemData(index).toString());
    QDir dir(path);
    const QStringList subDir = dir.entryList(QDir::AllDirs | QDir::NoDotAndDotDot, QDir::Name);
    for (const QString &mailPath : subDir) {
        const QString subMailPath(path + QLatin1Char('/') + mailPath);
        QDir dirMail(subMailPath);
        const QStringList subDirMail = dirMail.entryList(QDir::AllDirs | QDir::NoDotAndDotDot, QDir::Name);
        for (const QString &file : subDirMail) {
            const QString filterFile(subMailPath + QLatin1Char('/') + file + QLatin1StringView("/msgFilterRules.dat"));
            if (QFile(filterFile).exists()) {
                listFilterFiles << filterFile;
            }
        }
    }
    ui->listFiles->clear();
    ui->listFiles->addItems(listFilterFiles);
}

QStringList SelectThunderbirdFilterFilesWidget::selectedFiles() const
{
    QStringList listFiles;
    if (ui->selectFile->isChecked()) {
        listFiles << ui->fileUrl->url().path();
    } else {
        const QList<QListWidgetItem *> list = ui->listFiles->selectedItems();
        listFiles.reserve(list.count());
        for (QListWidgetItem *item : list) {
            listFiles << item->text();
        }
    }
    return listFiles;
}

void SelectThunderbirdFilterFilesWidget::setStartDir(const QUrl &url)
{
    ui->fileUrl->setNameFilter(i18n("Thunderbird File (*.dat)"));
    ui->fileUrl->setStartDir(url);
}

#include "moc_selectthunderbirdfilterfileswidget.cpp"
