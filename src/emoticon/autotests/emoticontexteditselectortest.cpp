/*
  SPDX-FileCopyrightText: 2021 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "emoticontexteditselectortest.h"
#include "emoticon/emoticontexteditselector.h"
#include "emoticon/emoticonunicodetab.h"
#include <QHBoxLayout>
#include <QTest>
QTEST_MAIN(EmoticonTextEditSelectorTest)

EmoticonTextEditSelectorTest::EmoticonTextEditSelectorTest(QObject *parent)
    : QObject(parent)
{
}

void EmoticonTextEditSelectorTest::shouldHaveDefaultValues()
{
    KPIMTextEdit::EmoticonTextEditSelector w;
    auto mainLayout = w.findChild<QHBoxLayout *>(QStringLiteral("mainLayout"));
    QVERIFY(mainLayout);
    QCOMPARE(mainLayout->contentsMargins(), {});

    auto mUnicodeTab = w.findChild<KPIMTextEdit::EmoticonUnicodeTab *>(QStringLiteral("mUnicodeTab"));
    QVERIFY(mUnicodeTab);
}
