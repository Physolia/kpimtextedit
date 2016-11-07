/*
  Copyright (c) 2016 Montel Laurent <montel@kde.org>

  This library is free software; you can redistribute it and/or modify it
  under the terms of the GNU Library General Public License as published by
  the Free Software Foundation; either version 2 of the License, or (at your
  option) any later version.

  This library is distributed in the hope that it will be useful, but WITHOUT
  ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
  FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Library General Public
  License for more details.

  You should have received a copy of the GNU Library General Public License
  along with this library; see the file COPYING.LIB.  If not, write to the
  Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
  02110-1301, USA.

*/

#ifndef RICHTEXTCOMPOSER_INSERTHTMLEDITOR_H
#define RICHTEXTCOMPOSER_INSERTHTMLEDITOR_H

#include "config-kpimtextedit.h"
#include <QWidget>

#include "kpimtextedit/plaintexteditor.h"
#ifdef KDEPIM_KF5SYNTAXHIGHLIGHTING_SUPPORT
#include <KSyntaxHighlighting/Repository>
#endif

namespace KPIMTextEdit
{
class TextEditorCompleter;
class InsertHtmlEditor : public KPIMTextEdit::PlainTextEditor
{
    Q_OBJECT
public:
    explicit InsertHtmlEditor(QWidget *parent = Q_NULLPTR);
    ~InsertHtmlEditor();
protected:
    void keyPressEvent(QKeyEvent *e) Q_DECL_OVERRIDE;
private:
    KPIMTextEdit::TextEditorCompleter *mTextEditorCompleter;
#ifdef KDEPIM_KF5SYNTAXHIGHLIGHTING_SUPPORT
    KSyntaxHighlighting::Repository mRepo;
#endif
};
}

#endif // INSERTHTMLEDITOR_H