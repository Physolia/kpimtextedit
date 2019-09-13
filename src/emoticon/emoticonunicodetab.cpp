/*
  Copyright (c) 2019 Montel Laurent <montel@kde.org>

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

#include "emoticonunicodetab.h"
#include "emoticonlistwidgetselector.h"
#include "textutils.h"
#include "emoticonunicodeutils.h"
#include <KLocalizedString>

using namespace KPIMTextEdit;
EmoticonUnicodeTab::EmoticonUnicodeTab(QWidget *parent)
    : QTabWidget(parent)
{
    setTabBarAutoHide(true);
    loadEmoticons();
    QFont f;
    f.setPointSize(22);
    f.setFamily(QStringLiteral("NotoColorEmoji"));
    setFont(f);
}

EmoticonUnicodeTab::~EmoticonUnicodeTab()
{
}

void EmoticonUnicodeTab::loadEmoticons()
{
    createPlainTextEmoticonTab(i18n("Faces"), EmoticonUnicodeUtils::unicodeFaceEmoji());

    createPlainTextEmoticonTab(i18n("Animals"), EmoticonUnicodeUtils::unicodeAnimalsEmoji());

    createPlainTextEmoticonTab(i18n("Emotions"), TextUtils::unicodeEmotionEmoji());
    createPlainTextEmoticonTab(i18n("Body"), TextUtils::unicodeBodyEmoji());
    createPlainTextEmoticonTab(i18n("Transports"), EmoticonUnicodeUtils::unicodeTransportEmoji());
    createPlainTextEmoticonTab(i18n("Events"), EmoticonUnicodeUtils::unicodeEventEmoji());
    createPlainTextEmoticonTab(i18n("Flags"), EmoticonUnicodeUtils::unicodeFlagsEmoji());
    createPlainTextEmoticonTab(i18n("Weather"), EmoticonUnicodeUtils::unicodeWeatherEmoji());
    createPlainTextEmoticonTab(i18n("Foods"), EmoticonUnicodeUtils::unicodeFoodEmoji());
    createPlainTextEmoticonTab(i18n("Sports"), TextUtils::unicodeSportEmoji());
    createPlainTextEmoticonTab(i18n("Time"), EmoticonUnicodeUtils::unicodeTimeEmoji());
    createPlainTextEmoticonTab(i18n("Game"), EmoticonUnicodeUtils::unicodeGameEmoji());
    createPlainTextEmoticonTab(i18n("Clothing"), EmoticonUnicodeUtils::unicodeClothingEmoji());
    createPlainTextEmoticonTab(i18n("Music"), EmoticonUnicodeUtils::unicodeSoundMusicEmoji());
    createPlainTextEmoticonTab(i18n("Computer"), EmoticonUnicodeUtils::unicodeComputerEmoji());
    createPlainTextEmoticonTab(i18n("Symbols"), TextUtils::unicodeSymbolsEmoji());
    createPlainTextEmoticonTab(i18n("Fantasy"), TextUtils::unicodeFantasyEmoji());
    createPlainTextEmoticonTab(i18n("Plant"), EmoticonUnicodeUtils::unicodePlantEmoji());
    createPlainTextEmoticonTab(i18n("Travel"), TextUtils::unicodeTravelEmoji());
    createPlainTextEmoticonTab(i18n("Book"), EmoticonUnicodeUtils::unicodeBookPaperEmoji());
    createPlainTextEmoticonTab(i18n("Book Tools"), TextUtils::unicodeBookToolsEmoji());
    createPlainTextEmoticonTab(i18n("Science"), EmoticonUnicodeUtils::unicodeScienceEmoji());
    createPlainTextEmoticonTab(i18n("Person"), EmoticonUnicodeUtils::unicodePersonEmoji());
}

void EmoticonUnicodeTab::createPlainTextEmoticonTab(const QString &str, const QList<uint> &emoticons)
{
    if (!emoticons.isEmpty()) {
        EmoticonListWidgetSelector *selector = new EmoticonListWidgetSelector(this);
        connect(selector, &KPIMTextEdit::EmoticonListWidgetSelector::itemSelected, this, &EmoticonUnicodeTab::itemSelected);
        selector->setEmoticons(emoticons);
        const QString strTab = QString::fromUcs4(&emoticons.at(0), 1);
        const int index = addTab(selector, strTab);
        if (!str.isEmpty()) {
            setTabToolTip(index, str);
        }
    }
}

void EmoticonUnicodeTab::createPlainTextEmoticonTab(const QString &str, const QStringList &emoticons)
{
    if (!emoticons.isEmpty()) {
        EmoticonListWidgetSelector *selector = new EmoticonListWidgetSelector(this);
        connect(selector, &KPIMTextEdit::EmoticonListWidgetSelector::itemSelected, this, &EmoticonUnicodeTab::itemSelected);
        selector->setEmoticons(emoticons);
        const QString strTab = emoticons.first();
        const int index = addTab(selector, strTab);
        if (!str.isEmpty()) {
            setTabToolTip(index, str);
        }
    }
}

void EmoticonUnicodeTab::createPlainTextEmoticonTab(const QString &str, const QVector<EmoticonUnicodeUtils::EmoticonStruct> &emoticons)
{
    if (!emoticons.isEmpty()) {
        EmoticonListWidgetSelector *selector = new EmoticonListWidgetSelector(this);
        connect(selector, &KPIMTextEdit::EmoticonListWidgetSelector::itemSelected, this, &EmoticonUnicodeTab::itemSelected);
        QStringList lst;
        for (const EmoticonUnicodeUtils::EmoticonStruct &emoji : emoticons) {
            lst.append(emoji.emoticonCode);
        }
        selector->setEmoticons(lst);
        const QString strTab = lst.first();
        const int index = addTab(selector, strTab);
        if (!str.isEmpty()) {
            setTabToolTip(index, str);
        }
    }
}
