/*
  Copyright (c) 2015 Montel Laurent <montel@kde.org>

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


#include <KLocalizedString>
#include <QApplication>
#include <KAboutData>
#include <QCommandLineParser>
#include <QStandardPaths>
#include <QToolBar>
#include <KActionCollection>

#include <../richtextcomposer.h>

#include <kpimtextedit/richtexteditorwidget.h>

int main(int argc, char **argv)
{
    QApplication app(argc, argv);
    QStandardPaths::setTestModeEnabled(true);

    KAboutData aboutData(QStringLiteral("richtextcomposerwidget_gui"), i18n("RichTextComposerWidgetTest_Gui"), QStringLiteral("1.0"));
    aboutData.setShortDescription(i18n("Test for richtextcomposertest widget"));
    QCommandLineParser parser;
    KAboutData::setApplicationData(aboutData);
    parser.addVersionOption();
    parser.addHelpOption();
    aboutData.setupCommandLine(&parser);
    parser.process(app);
    aboutData.processCommandLine(&parser);
    KPIMTextEdit::RichTextComposer *richtextcomposerwidget = new KPIMTextEdit::RichTextComposer;
    KActionCollection *ac = new KActionCollection(richtextcomposerwidget);
    richtextcomposerwidget->createActions(ac);
    KPIMTextEdit::RichTextEditorWidget *editor = new KPIMTextEdit::RichTextEditorWidget(richtextcomposerwidget);
    editor->resize(800, 600);
    editor->show();

    QToolBar bar;
    foreach (QAction *action, richtextcomposerwidget->richTextActionList()) {
        bar.addAction(action);
    }
    bar.show();
    richtextcomposerwidget->switchToPlainText();
    richtextcomposerwidget->activateRichText();

    app.exec();
    delete editor;
    return 0;
}

