/*
  Copyright (c) 2013-2020 Laurent Montel <montel@kde.org>

  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful, but
  WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  General Public License for more details.

  You should have received a copy of the GNU General Public License along
  with this program; if not, write to the Free Software Foundation, Inc.,
  51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
*/

#ifndef MAILCOMMON_FILTERSELECTIONDIALOG_H
#define MAILCOMMON_FILTERSELECTIONDIALOG_H

#include <QDialog>

#include <QVector>

class QPushButton;

class QListWidget;
class QWidget;

namespace MailCommon {
class MailFilter;

class FilterSelectionDialog : public QDialog
{
    Q_OBJECT

public:
    explicit FilterSelectionDialog(QWidget *parent = nullptr);
    ~FilterSelectionDialog() override;
    void setFilters(const QVector<MailFilter *> &filters);
    QVector<MailFilter *> selectedFilters() const;

public Q_SLOTS:
    void slotUnselectAllButton();
    void slotSelectAllButton();
    void reject() override;

private:
    void writeConfig();
    void readConfig();
    QListWidget *filtersListWidget = nullptr;
    QVector<MailFilter *> originalFilters;
    QPushButton *selectAllButton = nullptr;
    QPushButton *unselectAllButton = nullptr;
    QPushButton *mOkButton = nullptr;
};
}
#endif // FILTERSELECTIONDIALOG_H
