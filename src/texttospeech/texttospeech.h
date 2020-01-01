/*
   Copyright (C) 2014-2020 Laurent Montel <montel@kde.org>

   This library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Library General Public
   License as published by the Free Software Foundation; either
   version 2 of the License, or (at your option) any later version.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.

   You should have received a copy of the GNU Library General Public License
   along with this library; see the file COPYING.LIB.  If not, write to
   the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.
*/

#ifndef TEXTTOSPEECH_H
#define TEXTTOSPEECH_H

#include "kpimtextedit_export.h"
#include <QObject>

class QTextToSpeech;

namespace KPIMTextEdit {
/**
 * @brief The TextToSpeech class
 * @author Laurent Montel <montel@kde.org>
 */
class KPIMTEXTEDIT_EXPORT TextToSpeech : public QObject
{
    Q_OBJECT
public:
    static TextToSpeech *self();

    ~TextToSpeech();

    bool isReady() const;

    enum State {
        Ready = 0,
        Speaking,
        Paused,
        BackendError
    };

    double volume() const;
    QVector<QLocale> availableLocales() const;
    QStringList availableEngines() const;
    QStringList availableVoices() const;

    QLocale locale() const;

    void reloadSettings();
public Q_SLOTS:
    void say(const QString &text);
    void stop();
    void pause();
    void resume();

    void setRate(double rate);
    void setPitch(double pitch);
    void setVolume(double volume);
    void setLocale(const QLocale &locale) const;

Q_SIGNALS:
    void stateChanged(TextToSpeech::State);

private:
    void slotStateChanged();
    explicit TextToSpeech(QObject *parent = nullptr);
    friend class TextToSpeechPrivate;

    QString mDefaultEngine;
    QTextToSpeech *mTextToSpeech = nullptr;
};
}

#endif // TEXTTOSPEECH_H
