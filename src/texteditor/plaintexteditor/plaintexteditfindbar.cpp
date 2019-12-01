/*
   Copyright (C) 2012-2019 Laurent Montel <montel@kde.org>

   This library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Library General Public
   License as published by the Free Software Foundation; either
   version 2 of the License, or (at your option) any later version.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.

   You should have received a copy of the GNU Library General Public License
   along with this library; see the file COPYING.LIB.  If not, write to
   the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.
*/

#include "plaintexteditfindbar.h"
#include "texteditor/commonwidget/textfindreplacewidget.h"

#include <kmessagebox.h>
#include <KColorScheme>

#include <QLineEdit>
#include <QRegularExpression>
#include <QPlainTextEdit>

using namespace KPIMTextEdit;

class KPIMTextEdit::PlainTextEditFindBarPrivate
{
public:
    explicit PlainTextEditFindBarPrivate(QPlainTextEdit *view)
        : mView(view)
    {
    }

    QPlainTextEdit *mView = nullptr;
};

PlainTextEditFindBar::PlainTextEditFindBar(QPlainTextEdit *view, QWidget *parent)
    : TextEditFindBarBase(parent)
    , d(new KPIMTextEdit::PlainTextEditFindBarPrivate(view))
{
}

PlainTextEditFindBar::~PlainTextEditFindBar()
{
    delete d;
}

void PlainTextEditFindBar::slotSearchText(bool backward, bool isAutoSearch)
{
    d->mView->moveCursor(QTextCursor::Start);
    searchText(backward, isAutoSearch);
}

bool PlainTextEditFindBar::viewIsReadOnly() const
{
    return d->mView->isReadOnly();
}

bool PlainTextEditFindBar::documentIsEmpty() const
{
    return d->mView->document()->isEmpty();
}

bool PlainTextEditFindBar::searchInDocument(const QString &text, QTextDocument::FindFlags searchOptions)
{
    const bool found = d->mView->find(text, searchOptions);
    mFindWidget->setFoundMatch(found);
    return found;
}

#if QT_VERSION < QT_VERSION_CHECK(5, 13, 0)
bool PlainTextEditFindBar::searchInDocument(const QRegExp &regExp, QTextDocument::FindFlags searchOptions)
{
    const bool found = d->mView->find(regExp, searchOptions);
    mFindWidget->setFoundMatch(found);
    return found;
}

#else
bool PlainTextEditFindBar::searchInDocument(const QRegularExpression &regExp, QTextDocument::FindFlags searchOptions)
{
    const bool found = d->mView->find(regExp, searchOptions);
    mFindWidget->setFoundMatch(found);
    return found;
}

#endif

void PlainTextEditFindBar::autoSearchMoveCursor()
{
    QTextCursor cursor = d->mView->textCursor();
    cursor.setPosition(cursor.selectionStart());
    d->mView->setTextCursor(cursor);
}

void PlainTextEditFindBar::slotReplaceText()
{
    if (d->mView->textCursor().hasSelection()) {
        if (mFindWidget->isRegularExpression()) {
            if (d->mView->textCursor().selectedText().contains(mFindWidget->searchRegExp())) {
                d->mView->textCursor().insertText(mReplaceWidget->replaceLineEdit()->text());
                //search next after replace text.
                searchText(false, false);
            }
        } else {
            if (d->mView->textCursor().selectedText() == mFindWidget->searchText()) {
                d->mView->textCursor().insertText(mReplaceWidget->replaceLineEdit()->text());
                //search next after replace text.
                searchText(false, false);
            }
        }
    } else {
        searchText(false, false);
    }
}

void PlainTextEditFindBar::slotReplaceAllText()
{
    QString newText;
    if (mFindWidget->isRegularExpression()) {
        newText = d->mView->toPlainText().replace(mFindWidget->searchRegExp(), mReplaceWidget->replaceLineEdit()->text());
    } else {
        newText = d->mView->toPlainText().replace(mFindWidget->searchText(), mReplaceWidget->replaceLineEdit()->text());
    }
    d->mView->selectAll();
    d->mView->insertPlainText(newText);
}
