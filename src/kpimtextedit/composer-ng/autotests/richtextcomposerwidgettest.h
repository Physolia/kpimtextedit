/*
   SPDX-FileCopyrightText: 2015-2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include <QObject>

class RichTextComposerWidgetTest : public QObject
{
    Q_OBJECT
public:
    explicit RichTextComposerWidgetTest(QObject *parent = nullptr);
    ~RichTextComposerWidgetTest() override;

private Q_SLOTS:
    void shouldHaveDefaultValue();
};