/*
   SPDX-FileCopyrightText: 2014-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "kpimtextedit_export.h"
#include "texttospeech.h"
#include <QWidget>
namespace KPIMTextEdit
{
class AbstractTextToSpeechInterface;
class TextToSpeechWidgetPrivate;
/**
 * @brief The TextToSpeechWidget class
 * @author Laurent Montel <montel@kde.org>
 */
class KPIMTEXTEDIT_EXPORT TextToSpeechWidget : public QWidget
{
    Q_OBJECT
public:
    explicit TextToSpeechWidget(QWidget *parent = nullptr);
    ~TextToSpeechWidget() override;

    enum State { Stop = 0, Play, Pause };
    Q_ENUM(State)

    Q_REQUIRED_RESULT State state() const;
    void setState(KPIMTextEdit::TextToSpeechWidget::State state);

    void setTextToSpeechInterface(AbstractTextToSpeechInterface *interface);

    Q_REQUIRED_RESULT bool isReady() const;

public Q_SLOTS:
    void say(const QString &text);

    void slotStateChanged(KPIMTextEdit::TextToSpeech::State state);

Q_SIGNALS:
    void stateChanged(KPIMTextEdit::TextToSpeechWidget::State state);

private:
    void slotVolumeChanged(int value);
    void slotCloseTextToSpeechWidget();
    void slotConfigure();
    std::unique_ptr<TextToSpeechWidgetPrivate> const d;
};
}
