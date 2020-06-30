/*
   SPDX-FileCopyrightText: 2014-2020 Laurent Montel <montel@kde.org>

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

#ifndef ABSTRACTTEXTTOSPEECHCONFIGINTERFACE_H
#define ABSTRACTTEXTTOSPEECHCONFIGINTERFACE_H

#include <QLocale>
#include <QVector>
#include <QObject>
#include "kpimtextedit_export.h"
namespace KPIMTextEdit {
/**
 * @brief The AbstractTextToSpeechConfigInterface class
 * @author Laurent Montel <montel@kde.org>
 */
class KPIMTEXTEDIT_EXPORT AbstractTextToSpeechConfigInterface : public QObject
{
    Q_OBJECT
public:
    explicit AbstractTextToSpeechConfigInterface(QObject *parent = nullptr);
    ~AbstractTextToSpeechConfigInterface();
    virtual Q_REQUIRED_RESULT QVector<QLocale> availableLocales() const;
    virtual Q_REQUIRED_RESULT QLocale locale() const;
    virtual void setLocale(const QLocale &locale);

    virtual Q_REQUIRED_RESULT QStringList availableEngines() const;
    virtual Q_REQUIRED_RESULT QStringList availableVoices() const;
    virtual void setEngine(const QString &engineName);
};
}

#endif // ABSTRACTTEXTTOSPEECHCONFIGINTERFACE_H
