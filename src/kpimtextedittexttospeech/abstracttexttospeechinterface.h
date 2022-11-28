/*
   SPDX-FileCopyrightText: 2014-2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once
#include "kpimtextedittexttospeech_export.h"
#include <QObject>
namespace KPIMTextEditTextToSpeech
{
/**
 * @brief The AbstractTextToSpeechInterface class
 * @author Laurent Montel <montel@kde.org>
 */
class KPIMTEXTEDITTEXTTOSPEECH_EXPORT AbstractTextToSpeechInterface : public QObject
{
    Q_OBJECT
public:
    explicit AbstractTextToSpeechInterface(QObject *parent = nullptr);
    ~AbstractTextToSpeechInterface() override;

    Q_REQUIRED_RESULT virtual bool isReady() const;
    virtual void say(const QString &text);
    Q_REQUIRED_RESULT virtual int volume() const;
    virtual void setVolume(int value);
    virtual void reloadSettings();
};
}