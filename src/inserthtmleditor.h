/*
  SPDX-FileCopyrightText: 2016-2022 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: LGPL-2.0-or-later

*/

#pragma once

#include <QWidget>

#include "kpimtextedit/plaintexteditor.h"
#include <KSyntaxHighlighting/Repository>

namespace KPIMTextEdit
{
class TextEditorCompleter;
class InsertHtmlEditor : public KPIMTextEdit::PlainTextEditor
{
    Q_OBJECT
public:
    explicit InsertHtmlEditor(QWidget *parent = nullptr);
    ~InsertHtmlEditor() override;

protected:
    void keyPressEvent(QKeyEvent *e) override;

private:
    KPIMTextEdit::TextEditorCompleter *const mTextEditorCompleter;
    KSyntaxHighlighting::Repository mRepo;
};
}
