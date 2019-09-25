/***************************************************************************
 *   snippet feature from kdevelop/plugins/snippet/                        *
 *                                                                         *
 *   Copyright (C) 2007 by Robert Gruber                                   *
 *   rgruber@users.sourceforge.net                                         *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "snippetwidget.h"
#include "snippetsmanager.h"

#include <kactioncollection.h>
#include <KLocalizedString>

#include <QMenu>
#include <QContextMenuEvent>
#include <QHeaderView>
using namespace MailCommon;
SnippetWidget::SnippetWidget(KActionCollection *actionCollection, QWidget *parent)
    : QTreeView(parent)
{
    header()->hide();
    setAcceptDrops(true);
    setDragEnabled(true);
    setRootIsDecorated(true);
    setAlternatingRowColors(true);
    mSnippetsManager = new MailCommon::SnippetsManager(actionCollection, this, this);
    connect(mSnippetsManager, &MailCommon::SnippetsManager::insertPlainText, this, &SnippetWidget::insertSnippetText);

    setModel(mSnippetsManager->model());
    setSelectionModel(mSnippetsManager->selectionModel());

    connect(this, &QAbstractItemView::activated,
            mSnippetsManager->insertSnippetAction(), &QAction::trigger);
    connect(mSnippetsManager->model(), &QAbstractItemModel::rowsInserted,
            this, &QTreeView::expandAll);
    connect(mSnippetsManager->model(), &QAbstractItemModel::rowsRemoved,
            this, &QTreeView::expandAll);

    expandAll();
}

SnippetWidget::~SnippetWidget()
{
}

void SnippetWidget::contextMenuEvent(QContextMenuEvent *event)
{
    QMenu popup;

    const bool itemSelected = mSnippetsManager->selectionModel()->hasSelection();

    bool canAddSnippet = true;
    if (itemSelected) {
        popup.setTitle(mSnippetsManager->selectedName());
        if (mSnippetsManager->snippetGroupSelected()) {
            popup.addAction(mSnippetsManager->editSnippetGroupAction());
            popup.addAction(mSnippetsManager->deleteSnippetGroupAction());
        } else {
            canAddSnippet = false; // subsnippets are not permitted
            popup.addAction(mSnippetsManager->addSnippetAction());
            popup.addAction(mSnippetsManager->editSnippetAction());
            popup.addAction(mSnippetsManager->deleteSnippetAction());
            popup.addAction(mSnippetsManager->insertSnippetAction());
        }
        popup.addSeparator();
    } else {
        popup.setTitle(i18n("Text Snippets"));
    }
    if (canAddSnippet) {
        popup.addAction(mSnippetsManager->addSnippetAction());
    }
    popup.addAction(mSnippetsManager->addSnippetGroupAction());

    popup.exec(event->globalPos());
}

void SnippetWidget::dropEvent(QDropEvent *event)
{
    if (event->source() == this) {
        event->setDropAction(Qt::MoveAction);
    }
    QTreeView::dropEvent(event);
}
