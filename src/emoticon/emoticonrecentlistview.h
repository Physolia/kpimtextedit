/*
   SPDX-FileCopyrightText: 2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#pragma once

#include "emoticonlistview.h"
#include "kpimtextedit_private_export.h"
namespace KPIMTextEdit
{
class KPIMTEXTEDIT_TESTS_EXPORT EmoticonRecentListView : public EmoticonListView
{
    Q_OBJECT
public:
    explicit EmoticonRecentListView(QWidget *parent = nullptr);
    ~EmoticonRecentListView() override;
};
}