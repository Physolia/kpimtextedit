/*
  SPDX-FileCopyrightText: 2012-2022 Laurent Montel <montel@kde.org>
  based on code from kopete

  SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "kpimtextedit_private_export.h"
#include <QWidget>
class QLineEdit;
namespace KPIMTextEdit
{
class EmoticonUnicodeTab;
class KPIMTEXTEDIT_TESTS_EXPORT EmoticonTextEditSelector : public QWidget
{
    Q_OBJECT
public:
    explicit EmoticonTextEditSelector(QWidget *parent = nullptr);
    ~EmoticonTextEditSelector() override;

public Q_SLOTS:
    void loadEmoticons();

Q_SIGNALS:
    void itemSelected(const QString &);

private:
    void slotItemSelected(const QString &str);
    void slotSearchUnicode(const QString &str);
    EmoticonUnicodeTab *const mUnicodeTab;
    QLineEdit *const mSearchUnicodeLineEdit;
};
}

