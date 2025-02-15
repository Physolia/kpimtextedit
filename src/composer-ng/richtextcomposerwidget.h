/*
   SPDX-FileCopyrightText: 2015-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "kpimtextedit_export.h"
#include <QWidget>
namespace KPIMTextEdit
{
class RichTextComposerWidgetPrivate;
class RichTextComposer;
/**
 * @brief The RichTextComposerWidget class
 * @author Laurent Montel <montel@kde.org>
 * @since 5.2
 */
class KPIMTEXTEDIT_EXPORT RichTextComposerWidget : public QWidget
{
    Q_OBJECT
public:
    explicit RichTextComposerWidget(QWidget *parent = nullptr);
    ~RichTextComposerWidget() override;

    [[nodiscard]] KPIMTextEdit::RichTextComposer *richTextComposer() const;

private:
    std::unique_ptr<RichTextComposerWidgetPrivate> const d;
};
}
