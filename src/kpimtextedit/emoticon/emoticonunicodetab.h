/*
  SPDX-FileCopyrightText: 2019-2022 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include <QTabWidget>

#include "emoticonunicodeutils.h"
#include "kpimtextedit_export.h"

namespace KPIMTextEdit
{
class EmoticonUnicodeProxyModel;
class EmoticonRecentUsedFilterProxyModel;
/**
 * @brief The EmoticonUnicodeTab class
 * @author Laurent Montel <montel@kde.org>
 */
class KPIMTEXTEDIT_EXPORT EmoticonUnicodeTab : public QTabWidget
{
    Q_OBJECT
public:
    explicit EmoticonUnicodeTab(QWidget *parent = nullptr);
    ~EmoticonUnicodeTab() override;

    void loadEmoticons();

    void searchUnicode(const QString &str);
Q_SIGNALS:
    void itemSelected(const QString &);

private:
    void createEmoticonTab(const QString &str, const QVector<EmoticonUnicodeUtils::EmoticonStruct> &emoticons);
    void createSearchTab();
    void createRecentTab();
    void slotInsertEmoticons(const QString &identifier);
    void slotUsedIdentifierChanged(const QStringList &lst);
    EmoticonUnicodeProxyModel *const mEmoticonUnicodeSearchProxyModel;
    EmoticonRecentUsedFilterProxyModel *const mEmoticonUnicodeRecentProxyModel;
    int mSearchTabIndex = -1;
    int mRecentTabIndex = -1;
};
}