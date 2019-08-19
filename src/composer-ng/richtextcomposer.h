/*
   Copyright (C) 2015-2019 Laurent Montel <montel@kde.org>

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

#ifndef KPIMTEXTEDITRICHTEXTCOMPOSER_H
#define KPIMTEXTEDITRICHTEXTCOMPOSER_H

#include "kpimtextedit_export.h"

#include <kpimtextedit/richtexteditor.h>
class KActionCollection;

namespace KPIMTextEdit {
class RichTextComposerControler;
class RichTextComposerActions;
class RichTextExternalComposer;
class RichTextComposerEmailQuoteHighlighter;
/**
 * @brief The RichTextComposer class
 * @author Laurent Montel <montel@kde.org>
 */
class KPIMTEXTEDIT_EXPORT RichTextComposer : public KPIMTextEdit::RichTextEditor
{
    Q_OBJECT
public:
    explicit RichTextComposer(QWidget *parent = nullptr);
    ~RichTextComposer() override;

    enum Mode {
        Plain,    ///< Plain text mode
        Rich      ///< Rich text mode
    };

    /**
     * @return The current text mode
     */
    Q_REQUIRED_RESULT Mode textMode() const;

    /**
     * Enables word wrap. Words will be wrapped at the specified column.
     *
     * @param wrapColumn the column where words will be wrapped
     */
    void enableWordWrap(int wrapColumn);

    /**
     * Disables word wrap.
     * Note that words are still wrapped at the end of the editor; no scrollbar
     * will appear.
     */
    void disableWordWrap();

    /**
     * @return the line number where the cursor is. This takes word-wrapping
     *         into account. Line numbers start at 0.
     */
    Q_REQUIRED_RESULT int linePosition() const;

    /**
     * @return the column number where the cursor is.
     */
    Q_REQUIRED_RESULT int columnNumber() const;

    void forcePlainTextMarkup(bool force);

    void activateRichText();
    void switchToPlainText();

    void setTextOrHtml(const QString &text);
    Q_REQUIRED_RESULT QString textOrHtml() const;

    virtual void setHighlighterColors(KPIMTextEdit::RichTextComposerEmailQuoteHighlighter *highlighter);

    void setUseExternalEditor(bool use);
    void setExternalEditorPath(const QString &path);
    Q_REQUIRED_RESULT bool checkExternalEditorFinished();
    void killExternalEditor();

    //Redefine it for each apps
    virtual QString smartQuote(const QString &msg);    //need by kmail

    void setQuotePrefixName(const QString &quotePrefix);
    Q_REQUIRED_RESULT QString quotePrefixName() const;

    void setCursorPositionFromStart(unsigned int pos);
    Q_REQUIRED_RESULT int quoteLength(const QString &line, bool oneQuote = false) const;
    Q_REQUIRED_RESULT bool isLineQuoted(const QString &line) const;
    const QString defaultQuoteSign() const;
    void createActions(KActionCollection *ac);

    QVector<QAction *> richTextActionList() const;
    void setEnableActions(bool state);

    KPIMTextEdit::RichTextComposerControler *composerControler() const;
    KPIMTextEdit::RichTextExternalComposer *externalComposer() const;
    KPIMTextEdit::RichTextComposerActions *composerActions() const;
    void createHighlighter() override;
    virtual bool processAutoCorrection(QKeyEvent *event);

public Q_SLOTS:
    void insertPlainTextImplementation();
    void slotChangeInsertMode();

Q_SIGNALS:
    void insertModeChanged();
    /**
     * Emitted whenever the text mode is changed.
     *
     * @param mode The new text mode
     */
    void textModeChanged(KPIMTextEdit::RichTextComposer::Mode mode);
    /**
     * Emitted when the user uses the up arrow in the first line. The application
     * should then put the focus on the widget above the text edit.
     */
    void focusUp();

    void externalEditorStarted();
    void externalEditorClosed();
    void insertEmoticon(const QString &str);

protected:
    void keyPressEvent(QKeyEvent *event) override;
    Sonnet::SpellCheckDecorator *createSpellCheckDecorator() override;
    void insertFromMimeData(const QMimeData *source) override;
    bool canInsertFromMimeData(const QMimeData *source) const override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void clearDecorator() override;
    void updateHighLighter() override;
    bool processKeyEvent(QKeyEvent *e);

private:
    void slotTextModeChanged(KPIMTextEdit::RichTextComposer::Mode mode);
    void evaluateListSupport(QKeyEvent *event);
    void evaluateReturnKeySupport(QKeyEvent *event);
    class RichTextComposerPrivate;
    RichTextComposerPrivate *const d;
};
}
#endif // RICHTEXTCOMPOSER_H
