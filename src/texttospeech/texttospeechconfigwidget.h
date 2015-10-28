/*
  Copyright (c) 2014-2015 Montel Laurent <montel@kde.org>

  This program is free software; you can redistribute it and/or modify it
  under the terms of the GNU General Public License, version 2, as
  published by the Free Software Foundation.

  This program is distributed in the hope that it will be useful, but
  WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  General Public License for more details.

  You should have received a copy of the GNU General Public License along
  with this program; if not, write to the Free Software Foundation, Inc.,
  51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
*/

#ifndef TEXTTOSPEECHCONFIGWIDGET_H
#define TEXTTOSPEECHCONFIGWIDGET_H
#include <QWidget>
#include "kpimtextedit_export.h"
class QSlider;
class QComboBox;
namespace KPIMTextEdit
{
class TextToSpeechLanguageComboBox;
class AbstractTextToSpeechConfigInterface;
class KPIMTEXTEDIT_EXPORT TextToSpeechConfigWidget : public QWidget
{
    Q_OBJECT
public:
    explicit TextToSpeechConfigWidget(QWidget *parent = Q_NULLPTR);
    ~TextToSpeechConfigWidget();

    void writeConfig();
    void readConfig();

    void setTextToSpeechConfigInterface(AbstractTextToSpeechConfigInterface *interface);

private Q_SLOTS:
    void valueChanged();
    void slotUpdateSettings();
    void slotEngineChanged();    
    void slotLanguageChanged();

Q_SIGNALS:
    void configChanged(bool state);

private:
    void updateVoice();
    void updateLocale();
    void updateEngine();
    void updateAvailableLocales();
    void updateAvailableEngine();
    void updateAvailableVoices();
    QSlider *mVolume;
    QSlider *mRate;
    QSlider *mPitch;
    TextToSpeechLanguageComboBox *mLanguage;
    AbstractTextToSpeechConfigInterface *mAbstractTextToSpeechConfigInterface;
    QComboBox *mAvailableEngine;
    QComboBox *mVoice;
};
}

#endif // TEXTTOSPEECHCONFIGWIDGET_H