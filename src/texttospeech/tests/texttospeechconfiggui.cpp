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

#include "texttospeechconfiggui.h"
#include "texttospeech/texttospeechconfigwidget.h"
#include "texttospeech/texttospeech.h"
#include <QApplication>
#include <QCommandLineParser>
#include <QStandardPaths>
#include <QHBoxLayout>

TextToSpeechConfigGui::TextToSpeechConfigGui(QWidget *parent)
    : QWidget(parent)
{
    QHBoxLayout *hbox = new QHBoxLayout(this);
    KPIMTextEdit::TextToSpeechConfigWidget *widget = new KPIMTextEdit::TextToSpeechConfigWidget(this);
    hbox->addWidget(widget);
}

TextToSpeechConfigGui::~TextToSpeechConfigGui()
{
}

int main(int argc, char **argv)
{
    QApplication app(argc, argv);
    QStandardPaths::setTestModeEnabled(true);
    QCommandLineParser parser;
    parser.addVersionOption();
    parser.addHelpOption();
    parser.process(app);

    TextToSpeechConfigGui *w = new TextToSpeechConfigGui;

    w->show();
    app.exec();
    delete w;
    return 0;
}
