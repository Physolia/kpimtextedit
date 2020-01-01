/*
   Copyright (C) 2015-2020 Laurent Montel <montel@kde.org>

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

#include "richtextcomposer.h"
#include "richtextcomposercontroler.h"
#include "richtextcomposerimages.h"
#include "klinkdialog_p.h"
#include "nestedlisthelper_p.h"
#include "inserthtmldialog.h"
#include <QApplication>
#include <QRegularExpression>

#include <KColorScheme>
#include <KMessageBox>
#include <KLocalizedString>
#include <QColorDialog>
#include <QTextBlock>
#include <QTimer>
#include <QPointer>
#include <QClipboard>
#include <QIcon>
#include <QTextDocumentFragment>
#include "textutils.h"
#include "insertimagedialog.h"

using namespace KPIMTextEdit;

class Q_DECL_HIDDEN RichTextComposerControler::RichTextComposerControlerPrivate
{
public:
    RichTextComposerControlerPrivate(RichTextComposer *composer, RichTextComposerControler *qq)
        : richtextComposer(composer)
        , q(qq)
    {
        nestedListHelper = new NestedListHelper(composer);
        richTextImages = new RichTextComposerImages(richtextComposer, q);
    }

    ~RichTextComposerControlerPrivate()
    {
        delete nestedListHelper;
    }

    QColor linkColor()
    {
        if (mLinkColor.isValid()) {
            return mLinkColor;
        }
        mLinkColor = KColorScheme(QPalette::Active, KColorScheme::View).foreground(KColorScheme::LinkText).color();
        return mLinkColor;
    }

    void selectLinkText(QTextCursor *cursor) const;
    void fixupTextEditString(QString &text) const;
    void mergeFormatOnWordOrSelection(const QTextCharFormat &format);
    QString addQuotesToText(const QString &inputText, const QString &defaultQuoteSign);
    void updateLink(const QString &linkUrl, const QString &linkText);
    QFont saveFont;
    QColor mLinkColor;
    QTextCharFormat painterFormat;
    NestedListHelper *nestedListHelper = nullptr;
    RichTextComposer *richtextComposer = nullptr;
    RichTextComposerImages *richTextImages = nullptr;
    RichTextComposerControler *q = nullptr;
    bool painterActive = false;
};

void RichTextComposerControler::RichTextComposerControlerPrivate::selectLinkText(QTextCursor *cursor) const
{
    // If the cursor is on a link, select the text of the link.
    if (cursor->charFormat().isAnchor()) {
        QString aHref = cursor->charFormat().anchorHref();

        // Move cursor to start of link
        while (cursor->charFormat().anchorHref() == aHref) {
            if (cursor->atStart()) {
                break;
            }
            cursor->setPosition(cursor->position() - 1);
        }
        if (cursor->charFormat().anchorHref() != aHref) {
            cursor->setPosition(cursor->position() + 1, QTextCursor::KeepAnchor);
        }

        // Move selection to the end of the link
        while (cursor->charFormat().anchorHref() == aHref) {
            if (cursor->atEnd()) {
                break;
            }
            const int oldPosition = cursor->position();
            cursor->movePosition(QTextCursor::NextCharacter, QTextCursor::KeepAnchor);
            //Wordaround Qt Bug. when we have a table.
            //FIXME selection url
            if (oldPosition == cursor->position()) {
                break;
            }
        }
        if (cursor->charFormat().anchorHref() != aHref) {
            cursor->setPosition(cursor->position() - 1, QTextCursor::KeepAnchor);
        }
    } else if (cursor->hasSelection()) {
        // Nothing to do. Using the currently selected text as the link text.
    } else {
        // Select current word
        cursor->movePosition(QTextCursor::StartOfWord);
        cursor->movePosition(QTextCursor::EndOfWord, QTextCursor::KeepAnchor);
    }
}

void RichTextComposerControler::RichTextComposerControlerPrivate::mergeFormatOnWordOrSelection(const QTextCharFormat &format)
{
    QTextCursor cursor = richtextComposer->textCursor();
    QTextCursor wordStart(cursor);
    QTextCursor wordEnd(cursor);

    wordStart.movePosition(QTextCursor::StartOfWord);
    wordEnd.movePosition(QTextCursor::EndOfWord);

    cursor.beginEditBlock();
    if (!cursor.hasSelection() && cursor.position() != wordStart.position() && cursor.position() != wordEnd.position()) {
        cursor.select(QTextCursor::WordUnderCursor);
    }
    cursor.mergeCharFormat(format);
    richtextComposer->mergeCurrentCharFormat(format);
    cursor.endEditBlock();
}

RichTextComposerControler::RichTextComposerControler(RichTextComposer *richtextComposer, QObject *parent)
    : QObject(parent)
    , d(new RichTextComposerControlerPrivate(richtextComposer, this))
{
}

RichTextComposerControler::~RichTextComposerControler()
{
    delete d;
}

bool RichTextComposerControler::painterActive() const
{
    return d->painterActive;
}

void RichTextComposerControler::setFontForWholeText(const QFont &font)
{
    QTextCharFormat fmt;
    fmt.setFont(font);
    QTextCursor cursor(richTextComposer()->document());
    cursor.movePosition(QTextCursor::End, QTextCursor::KeepAnchor);
    cursor.mergeCharFormat(fmt);
    richTextComposer()->document()->setDefaultFont(font);
}

void RichTextComposerControler::disablePainter()
{
    // If the painter is active, paint the selection with the
    // correct format.
    if (richTextComposer()->textCursor().hasSelection()) {
        QTextCursor cursor = richTextComposer()->textCursor();
        cursor.setCharFormat(d->painterFormat);
        richTextComposer()->setTextCursor(cursor);
    }
    d->painterActive = false;
}

RichTextComposerImages *RichTextComposerControler::composerImages() const
{
    return d->richTextImages;
}

NestedListHelper *RichTextComposerControler::nestedListHelper() const
{
    return d->nestedListHelper;
}

void RichTextComposerControler::ensureCursorVisibleDelayed()
{
    d->richtextComposer->ensureCursorVisible();
}

RichTextComposer *RichTextComposerControler::richTextComposer() const
{
    return d->richtextComposer;
}

void RichTextComposerControler::insertHorizontalRule()
{
    QTextCursor cursor = richTextComposer()->textCursor();
    QTextBlockFormat bf = cursor.blockFormat();
    QTextCharFormat cf = cursor.charFormat();

    cursor.beginEditBlock();
    cursor.insertHtml(QStringLiteral("<hr>"));
    cursor.insertBlock(bf, cf);
    richTextComposer()->setTextCursor(cursor);
    richTextComposer()->activateRichText();
    cursor.endEditBlock();
}

void RichTextComposerControler::alignLeft()
{
    richTextComposer()->setAlignment(Qt::AlignLeft);
    richTextComposer()->setFocus();
    richTextComposer()->activateRichText();
}

void RichTextComposerControler::alignCenter()
{
    richTextComposer()->setAlignment(Qt::AlignHCenter);
    richTextComposer()->setFocus();
    richTextComposer()->activateRichText();
}

void RichTextComposerControler::alignRight()
{
    richTextComposer()->setAlignment(Qt::AlignRight);
    richTextComposer()->setFocus();
    richTextComposer()->activateRichText();
}

void RichTextComposerControler::alignJustify()
{
    richTextComposer()->setAlignment(Qt::AlignJustify);
    richTextComposer()->setFocus();
    richTextComposer()->activateRichText();
}

void RichTextComposerControler::makeRightToLeft()
{
    QTextBlockFormat format;
    format.setLayoutDirection(Qt::RightToLeft);
    QTextCursor cursor = richTextComposer()->textCursor();
    cursor.mergeBlockFormat(format);
    richTextComposer()->setTextCursor(cursor);
    richTextComposer()->setFocus();
    richTextComposer()->activateRichText();
}

void RichTextComposerControler::makeLeftToRight()
{
    QTextBlockFormat format;
    format.setLayoutDirection(Qt::LeftToRight);
    QTextCursor cursor = richTextComposer()->textCursor();
    cursor.mergeBlockFormat(format);
    richTextComposer()->setTextCursor(cursor);
    richTextComposer()->setFocus();
    richTextComposer()->activateRichText();
}

void RichTextComposerControler::setTextBold(bool bold)
{
    QTextCharFormat fmt;
    fmt.setFontWeight(bold ? QFont::Bold : QFont::Normal);
    d->mergeFormatOnWordOrSelection(fmt);
    richTextComposer()->setFocus();
    richTextComposer()->activateRichText();
}

void RichTextComposerControler::setTextItalic(bool italic)
{
    QTextCharFormat fmt;
    fmt.setFontItalic(italic);
    d->mergeFormatOnWordOrSelection(fmt);
    richTextComposer()->setFocus();
    richTextComposer()->activateRichText();
}

void RichTextComposerControler::setTextUnderline(bool underline)
{
    QTextCharFormat fmt;
    fmt.setFontUnderline(underline);
    d->mergeFormatOnWordOrSelection(fmt);
    richTextComposer()->setFocus();
    richTextComposer()->activateRichText();
}

void RichTextComposerControler::setTextStrikeOut(bool strikeOut)
{
    QTextCharFormat fmt;
    fmt.setFontStrikeOut(strikeOut);
    d->mergeFormatOnWordOrSelection(fmt);
    richTextComposer()->setFocus();
    richTextComposer()->activateRichText();
}

void RichTextComposerControler::setTextForegroundColor(const QColor &color)
{
    QTextCharFormat fmt;
    fmt.setForeground(color);
    d->mergeFormatOnWordOrSelection(fmt);
    richTextComposer()->setFocus();
    richTextComposer()->activateRichText();
}

void RichTextComposerControler::setTextBackgroundColor(const QColor &color)
{
    QTextCharFormat fmt;
    fmt.setBackground(color);
    d->mergeFormatOnWordOrSelection(fmt);
    richTextComposer()->setFocus();
    richTextComposer()->activateRichText();
}

void RichTextComposerControler::setFontFamily(const QString &fontFamily)
{
    QTextCharFormat fmt;
    fmt.setFontFamily(fontFamily);
    d->mergeFormatOnWordOrSelection(fmt);
    richTextComposer()->setFocus();
    richTextComposer()->activateRichText();
}

void RichTextComposerControler::setFontSize(int size)
{
    QTextCharFormat fmt;
    fmt.setFontPointSize(size);
    d->mergeFormatOnWordOrSelection(fmt);
    richTextComposer()->setFocus();
    richTextComposer()->activateRichText();
}

void RichTextComposerControler::setFont(const QFont &font)
{
    QTextCharFormat fmt;
    fmt.setFont(font);
    d->mergeFormatOnWordOrSelection(fmt);
    richTextComposer()->setFocus();
    richTextComposer()->activateRichText();
}

void RichTextComposerControler::setTextSuperScript(bool superscript)
{
    QTextCharFormat fmt;
    fmt.setVerticalAlignment(superscript ? QTextCharFormat::AlignSuperScript : QTextCharFormat::AlignNormal);
    d->mergeFormatOnWordOrSelection(fmt);
    richTextComposer()->setFocus();
    richTextComposer()->activateRichText();
}

void RichTextComposerControler::setTextSubScript(bool subscript)
{
    QTextCharFormat fmt;
    fmt.setVerticalAlignment(subscript ? QTextCharFormat::AlignSubScript : QTextCharFormat::AlignNormal);
    d->mergeFormatOnWordOrSelection(fmt);
    richTextComposer()->setFocus();
    richTextComposer()->activateRichText();
}

void RichTextComposerControler::setChangeTextForegroundColor()
{
    const QColor currentColor = richTextComposer()->textColor();
    const QColor defaultColor = KColorScheme(QPalette::Active, KColorScheme::View).foreground().color();

    const QColor selectedColor = QColorDialog::getColor(currentColor.isValid() ? currentColor : defaultColor, richTextComposer());

    if (!selectedColor.isValid() && !currentColor.isValid()) {
        setTextForegroundColor(defaultColor);
    } else if (selectedColor.isValid()) {
        setTextForegroundColor(selectedColor);
    }
}

void RichTextComposerControler::setChangeTextBackgroundColor()
{
    QTextCharFormat fmt = richTextComposer()->textCursor().charFormat();
    const QColor currentColor = fmt.background().color();
    const QColor defaultColor = KColorScheme(QPalette::Active, KColorScheme::View).foreground().color();

    const QColor selectedColor = QColorDialog::getColor(currentColor.isValid() ? currentColor : defaultColor, richTextComposer());

    if (!selectedColor.isValid() && !currentColor.isValid()) {
        setTextBackgroundColor(defaultColor);
    } else if (selectedColor.isValid()) {
        setTextBackgroundColor(selectedColor);
    }
}

QString RichTextComposerControler::currentLinkUrl() const
{
    return richTextComposer()->textCursor().charFormat().anchorHref();
}

QString RichTextComposerControler::currentLinkText() const
{
    QTextCursor cursor = richTextComposer()->textCursor();
    d->selectLinkText(&cursor);
    return cursor.selectedText();
}

void RichTextComposerControler::selectLinkText() const
{
    QTextCursor cursor = richTextComposer()->textCursor();
    d->selectLinkText(&cursor);
    richTextComposer()->setTextCursor(cursor);
}

void RichTextComposerControler::manageLink()
{
    selectLinkText();
    QPointer<KLinkDialog> linkDialog = new KLinkDialog(richTextComposer());
    linkDialog->setLinkText(currentLinkText());
    linkDialog->setLinkUrl(currentLinkUrl());

    if (linkDialog->exec()) {
        d->updateLink(linkDialog->linkUrl(), linkDialog->linkText());
    }

    delete linkDialog;
}

void RichTextComposerControler::updateLink(const QString &linkUrl, const QString &linkText)
{
    d->updateLink(linkUrl, linkText);
}

void RichTextComposerControler::RichTextComposerControlerPrivate::updateLink(const QString &linkUrl, const QString &linkText)
{
    q->selectLinkText();

    QTextCursor cursor = richtextComposer->textCursor();
    cursor.beginEditBlock();

    if (!cursor.hasSelection()) {
        cursor.select(QTextCursor::WordUnderCursor);
    }

    QTextCharFormat format = cursor.charFormat();
    // Save original format to create an extra space with the existing char
    // format for the block
    const QTextCharFormat originalFormat = format;
    if (!linkUrl.isEmpty()) {
        // Add link details
        format.setAnchor(true);
        format.setAnchorHref(linkUrl);
        // Workaround for QTBUG-1814:
        // Link formatting does not get applied immediately when setAnchor(true)
        // is called.  So the formatting needs to be applied manually.
        format.setUnderlineStyle(QTextCharFormat::SingleUnderline);
        format.setUnderlineColor(linkColor());
        format.setForeground(linkColor());
        richtextComposer->activateRichText();
    } else {
        // Remove link details
        format.setAnchor(false);
        format.setAnchorHref(QString());
        // Workaround for QTBUG-1814:
        // Link formatting does not get removed immediately when setAnchor(false)
        // is called. So the formatting needs to be applied manually.
        QTextDocument defaultTextDocument;
        QTextCharFormat defaultCharFormat = defaultTextDocument.begin().charFormat();

        format.setUnderlineStyle(defaultCharFormat.underlineStyle());
        format.setUnderlineColor(defaultCharFormat.underlineColor());
        format.setForeground(defaultCharFormat.foreground());
    }

    // Insert link text specified in dialog, otherwise write out url.
    QString _linkText;
    if (!linkText.isEmpty()) {
        _linkText = linkText;
    } else {
        _linkText = linkUrl;
    }
    cursor.insertText(_linkText, format);

    // Insert a space after the link if at the end of the block so that
    // typing some text after the link does not carry link formatting
    if (!linkUrl.isEmpty() && cursor.atBlockEnd()) {
        cursor.setPosition(cursor.selectionEnd());
        cursor.setCharFormat(originalFormat);
        cursor.insertText(QStringLiteral(" "));
    }

    cursor.endEditBlock();
}

QString RichTextComposerControler::toCleanHtml() const
{
    QString result = richTextComposer()->toHtml();

    static const QString EMPTYLINEHTML = QStringLiteral(
        "<p style=\"-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; "
        "margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px; \">&nbsp;</p>");

    // Qt inserts various style properties based on the current mode of the editor (underline,
    // bold, etc), but only empty paragraphs *also* have qt-paragraph-type set to 'empty'.
    static const QString EMPTYLINEREGEX = QStringLiteral(
        "<p style=\"-qt-paragraph-type:empty;(.*)</p>");

    static const QString OLLISTPATTERNQT = QStringLiteral(
        "<ol style=\"margin-top: 0px; margin-bottom: 0px; margin-left: 0px;");

    static const QString ULLISTPATTERNQT = QStringLiteral(
        "<ul style=\"margin-top: 0px; margin-bottom: 0px; margin-left: 0px;");

    static const QString ORDEREDLISTHTML = QStringLiteral(
        "<ol style=\"margin-top: 0px; margin-bottom: 0px;");

    static const QString UNORDEREDLISTHTML = QStringLiteral(
        "<ul style=\"margin-top: 0px; margin-bottom: 0px;");

    // fix 1 - empty lines should show as empty lines - MS Outlook treats margin-top:0px; as
    // a non-existing line.
    // Although we can simply remove the margin-top style property, we still get unwanted results
    // if you have three or more empty lines. It's best to replace empty <p> elements with <p>&nbsp;</p>.

    QRegExp emptyLineFinder(EMPTYLINEREGEX);
    emptyLineFinder.setMinimal(true);

    // find the first occurrence
    int offset = emptyLineFinder.indexIn(result, 0);
    while (offset != -1) {
        // replace all the matching text with the new line text
        result.replace(offset, emptyLineFinder.matchedLength(), EMPTYLINEHTML);
        // advance the search offset to just beyond the last replace
        offset += EMPTYLINEHTML.length();
        // find the next occurrence
        offset = emptyLineFinder.indexIn(result, offset);
    }

    // fix 2a - ordered lists - MS Outlook treats margin-left:0px; as
    // a non-existing number; e.g: "1. First item" turns into "First Item"
    result.replace(OLLISTPATTERNQT, ORDEREDLISTHTML);

    // fix 2b - unordered lists - MS Outlook treats margin-left:0px; as
    // a non-existing bullet; e.g: "* First bullet" turns into "First Bullet"
    result.replace(ULLISTPATTERNQT, UNORDEREDLISTHTML);

    return result;
}

bool RichTextComposerControler::canIndentList() const
{
    return d->nestedListHelper->canIndent();
}

bool RichTextComposerControler::canDedentList() const
{
    return d->nestedListHelper->canDedent();
}

void RichTextComposerControler::indentListMore()
{
    d->nestedListHelper->handleOnIndentMore();
    richTextComposer()->activateRichText();
}

void RichTextComposerControler::indentListLess()
{
    d->nestedListHelper->handleOnIndentLess();
}

void RichTextComposerControler::setListStyle(int styleIndex)
{
    d->nestedListHelper->handleOnBulletType(-styleIndex);
    richTextComposer()->setFocus();
    richTextComposer()->activateRichText();
}

void RichTextComposerControler::insertLink(const QString &url)
{
    if (url.isEmpty()) {
        return;
    }
    if (richTextComposer()->textMode() == RichTextComposer::Rich) {
        QTextCursor cursor = richTextComposer()->textCursor();
        cursor.beginEditBlock();

        QTextCharFormat format = cursor.charFormat();
        // Save original format to create an extra space with the existing char
        // format for the block
        const QTextCharFormat originalFormat = format;
        // Add link details
        format.setAnchor(true);
        format.setAnchorHref(url);
        // Workaround for QTBUG-1814:
        // Link formatting does not get applied immediately when setAnchor(true)
        // is called.  So the formatting needs to be applied manually.
        format.setUnderlineStyle(QTextCharFormat::SingleUnderline);
        format.setUnderlineColor(d->linkColor());
        format.setForeground(d->linkColor());
        // Insert link text specified in dialog, otherwise write out url.
        cursor.insertText(url, format);

        cursor.setPosition(cursor.selectionEnd());
        cursor.setCharFormat(originalFormat);
        cursor.insertText(QStringLiteral(" \n"));
        cursor.endEditBlock();
    } else {
        richTextComposer()->textCursor().insertText(url + QLatin1Char('\n'));
    }
}

void RichTextComposerControler::insertShareLink(const QString &url)
{
    if (url.isEmpty()) {
        return;
    }
    const QString msg = i18n("I've linked 1 file to this email:");
    if (richTextComposer()->textMode() == RichTextComposer::Rich) {
        QTextCursor cursor = richTextComposer()->textCursor();

        cursor.beginEditBlock();
        cursor.insertText(QLatin1Char('\n') + msg + QLatin1Char('\n'));

        QTextCharFormat format = cursor.charFormat();
        // Save original format to create an extra space with the existing char
        // format for the block
        const QTextCharFormat originalFormat = format;
        // Add link details
        format.setAnchor(true);
        format.setAnchorHref(url);
        // Workaround for QTBUG-1814:
        // Link formatting does not get applied immediately when setAnchor(true)
        // is called.  So the formatting needs to be applied manually.
        format.setUnderlineStyle(QTextCharFormat::SingleUnderline);
        format.setUnderlineColor(d->linkColor());
        format.setForeground(d->linkColor());
        // Insert link text specified in dialog, otherwise write out url.
        cursor.insertText(url, format);

        cursor.setPosition(cursor.selectionEnd());
        cursor.setCharFormat(originalFormat);
        cursor.insertText(QStringLiteral(" \n"));
        cursor.endEditBlock();
    } else {
        richTextComposer()->textCursor().insertText(QLatin1Char('\n') + msg + QLatin1Char('\n') + url + QLatin1Char('\n'));
    }
}

void RichTextComposerControler::setCursorPositionFromStart(unsigned int pos)
{
    if (pos > 0) {
        QTextCursor cursor = richTextComposer()->textCursor();
        //Fix html pos cursor
        cursor.setPosition(qMin(pos, (unsigned int)cursor.document()->characterCount() - 1));
        richTextComposer()->setTextCursor(cursor);
        ensureCursorVisible();
    }
}

void RichTextComposerControler::ensureCursorVisible()
{
    // Hack: In KMail, the layout of the composer changes again after
    //       creating the editor (the toolbar/menubar creation is delayed), so
    //       the size of the editor changes as well, possibly hiding the cursor
    //       even though we called ensureCursorVisible() before the layout phase.
    //
    //       Delay the actual call to ensureCursorVisible() a bit to work around
    //       the problem.
    QTimer::singleShot(500, richTextComposer()->composerControler(), &RichTextComposerControler::ensureCursorVisibleDelayed);
}

void RichTextComposerControler::RichTextComposerControlerPrivate::fixupTextEditString(QString &text) const
{
    // Remove line separators. Normal \n chars are still there, so no linebreaks get lost here
    text.remove(QChar::LineSeparator);

    // Get rid of embedded images, see QTextImageFormat documentation:
    // "Inline images are represented by an object replacement character (0xFFFC in Unicode) "
    text.remove(0xFFFC);

    // In plaintext mode, each space is non-breaking.
    text.replace(QChar::Nbsp, QChar::fromLatin1(' '));
}

bool RichTextComposerControler::isFormattingUsed() const
{
    if (richTextComposer()->textMode() == RichTextComposer::Plain) {
        return false;
    }

    return KPIMTextEdit::TextUtils::containsFormatting(richTextComposer()->document());
}

void RichTextComposerControler::slotAddEmoticon(const QString &text)
{
    QTextCursor cursor = richTextComposer()->textCursor();
    cursor.insertText(text);
}

void RichTextComposerControler::slotInsertHtml()
{
    if (richTextComposer()->textMode() == RichTextComposer::Rich) {
        QPointer<KPIMTextEdit::InsertHtmlDialog> dialog = new KPIMTextEdit::InsertHtmlDialog(richTextComposer());
        const QTextDocumentFragment fragmentSelected = richTextComposer()->textCursor().selection();
        if (!fragmentSelected.isEmpty()) {
            dialog->setSelectedText(fragmentSelected.toHtml());
        }
        if (dialog->exec()) {
            const QString str = dialog->html();
            if (!str.isEmpty()) {
                QTextCursor cursor = richTextComposer()->textCursor();
                cursor.insertHtml(str);
            }
        }
        delete dialog;
    }
}

void RichTextComposerControler::slotAddImage()
{
    QPointer<KPIMTextEdit::InsertImageDialog> dlg = new KPIMTextEdit::InsertImageDialog(richTextComposer());
    if (dlg->exec() == QDialog::Accepted) {
        const QUrl url = dlg->imageUrl();
        int imageWidth = -1;
        int imageHeight = -1;
        if (!dlg->keepOriginalSize()) {
            imageWidth = dlg->imageWidth();
            imageHeight = dlg->imageHeight();
        }
        if (url.isLocalFile()) {
            d->richTextImages->addImage(url, imageWidth, imageHeight);
        } else {
            KMessageBox::error(richTextComposer(), i18n("Only local files are supported."));
        }
    }
    delete dlg;
}

void RichTextComposerControler::slotFormatReset()
{
    setTextBackgroundColor(richTextComposer()->palette().highlightedText().color());
    setTextForegroundColor(richTextComposer()->palette().text().color());
    richTextComposer()->setFont(d->saveFont);
}

void RichTextComposerControler::slotDeleteLine()
{
    if (richTextComposer()->hasFocus()) {
        QTextCursor cursor = richTextComposer()->textCursor();
        QTextBlock block = cursor.block();
        const QTextLayout *layout = block.layout();

        // The current text block can have several lines due to word wrapping.
        // Search the line the cursor is in, and then delete it.
        for (int lineNumber = 0; lineNumber < layout->lineCount(); ++lineNumber) {
            QTextLine line = layout->lineAt(lineNumber);
            const bool lastLineInBlock = (line.textStart() + line.textLength() == block.length() - 1);
            const bool oneLineBlock = (layout->lineCount() == 1);
            const int startOfLine = block.position() + line.textStart();
            int endOfLine = block.position() + line.textStart() + line.textLength();
            if (!lastLineInBlock) {
                endOfLine -= 1;
            }

            // Found the line where the cursor is in
            if (cursor.position() >= startOfLine && cursor.position() <= endOfLine) {
                int deleteStart = startOfLine;
                int deleteLength = line.textLength();
                if (oneLineBlock) {
                    deleteLength++; // The trailing newline
                }

                // When deleting the last line in the document,
                // remove the newline of the line before the last line instead
                if (deleteStart + deleteLength >= richTextComposer()->document()->characterCount()
                    && deleteStart > 0) {
                    deleteStart--;
                }

                cursor.beginEditBlock();
                cursor.setPosition(deleteStart);
                cursor.movePosition(QTextCursor::NextCharacter, QTextCursor::KeepAnchor, deleteLength);
                cursor.removeSelectedText();
                cursor.endEditBlock();
                return;
            }
        }
    }
}

void RichTextComposerControler::slotPasteAsQuotation()
{
#ifndef QT_NO_CLIPBOARD
    if (richTextComposer()->hasFocus()) {
        const QString s = QApplication::clipboard()->text();
        if (!s.isEmpty()) {
            richTextComposer()->insertPlainText(d->addQuotesToText(s, d->richtextComposer->defaultQuoteSign()));
        }
    }
#endif
}

void RichTextComposerControler::slotPasteWithoutFormatting()
{
#ifndef QT_NO_CLIPBOARD
    if (richTextComposer()->hasFocus()) {
        const QString s = QApplication::clipboard()->text();
        if (!s.isEmpty()) {
            richTextComposer()->insertPlainText(s);
        }
    }
#endif
}

void RichTextComposerControler::slotRemoveQuotes()
{
    QTextCursor cursor = richTextComposer()->textCursor();
    cursor.beginEditBlock();
    if (!cursor.hasSelection()) {
        cursor.select(QTextCursor::Document);
    }

    QTextBlock block = richTextComposer()->document()->findBlock(cursor.selectionStart());
    int selectionEnd = cursor.selectionEnd();
    while (block.isValid() && block.position() <= selectionEnd) {
        cursor.setPosition(block.position());
        const int length = richTextComposer()->quoteLength(block.text(), true);
        if (length > 0) {
            cursor.movePosition(QTextCursor::NextCharacter, QTextCursor::KeepAnchor, length);
            cursor.removeSelectedText();
            selectionEnd -= length;
        }
        block = block.next();
    }
    cursor.clearSelection();
    cursor.endEditBlock();
}

void RichTextComposerControler::slotAddQuotes()
{
    addQuotes(d->richtextComposer->defaultQuoteSign());
}

void RichTextComposerControler::addQuotes(const QString &defaultQuote)
{
    QTextCursor cursor = richTextComposer()->textCursor();
    cursor.beginEditBlock();
    QString selectedText;
    bool lastCharacterIsAParagraphChar = false;
    if (!cursor.hasSelection()) {
        cursor.select(QTextCursor::Document);
        selectedText = cursor.selectedText();
        cursor.removeSelectedText();
    } else {
        selectedText = cursor.selectedText();
        if (selectedText[selectedText.length() - 1] == QChar::ParagraphSeparator) {
            lastCharacterIsAParagraphChar = true;
        }
    }
    richTextComposer()->insertPlainText(d->addQuotesToText(selectedText, defaultQuote) + (lastCharacterIsAParagraphChar ? QChar::ParagraphSeparator : QChar()));
    cursor.endEditBlock();
}

QString RichTextComposerControler::RichTextComposerControlerPrivate::addQuotesToText(const QString &inputText, const QString &defaultQuoteSign)
{
    QString answer = inputText;
    answer.replace(QLatin1Char('\n'), QLatin1Char('\n') + defaultQuoteSign);
    //cursor.selectText() as QChar::ParagraphSeparator as paragraph separator.
    answer.replace(QChar::ParagraphSeparator, QLatin1Char('\n') + defaultQuoteSign);
    answer.prepend(defaultQuoteSign);
    answer += QLatin1Char('\n');
    return richtextComposer->smartQuote(answer);
}

void RichTextComposerControler::slotFormatPainter(bool active)
{
    if (active) {
        d->painterFormat = richTextComposer()->currentCharFormat();
        d->painterActive = true;
        richTextComposer()->viewport()->setCursor(QCursor(QIcon::fromTheme(QStringLiteral("draw-brush")).pixmap(32, 32), 0, 32));
    } else {
        d->painterFormat = QTextCharFormat();
        d->painterActive = false;
        richTextComposer()->viewport()->setCursor(Qt::IBeamCursor);
    }
}

void RichTextComposerControler::textModeChanged(KPIMTextEdit::RichTextComposer::Mode mode)
{
    if (mode == KPIMTextEdit::RichTextComposer::Rich) {
        d->saveFont = richTextComposer()->currentFont();
    }
}

QString RichTextComposerControler::toCleanPlainText(const QString &plainText) const
{
    QString temp = plainText.isEmpty() ? richTextComposer()->toPlainText() : plainText;
    d->fixupTextEditString(temp);
    return temp;
}

QString RichTextComposerControler::toWrappedPlainText() const
{
    QTextDocument *doc = richTextComposer()->document();
    return toWrappedPlainText(doc);
}

QString RichTextComposerControler::toWrappedPlainText(QTextDocument *doc) const
{
    QString temp;
    QRegularExpression rx(QStringLiteral("(http|ftp|ldap)s?\\S+-$"));
    QTextBlock block = doc->begin();
    while (block.isValid()) {
        QTextLayout *layout = block.layout();
        const int numberOfLine(layout->lineCount());
        bool urlStart = false;
        for (int i = 0; i < numberOfLine; ++i) {
            QTextLine line = layout->lineAt(i);
            QString lineText = block.text().mid(line.textStart(), line.textLength());

            if (lineText.contains(rx)
                || (urlStart && !lineText.contains(QLatin1Char(' '))
                    && lineText.endsWith(QLatin1Char('-')))) {
                // don't insert line break in URL
                temp += lineText;
                urlStart = true;
            } else {
                temp += lineText + QLatin1Char('\n');
            }
        }
        block = block.next();
    }

    // Remove the last superfluous newline added above
    if (temp.endsWith(QLatin1Char('\n'))) {
        temp.chop(1);
    }
    d->fixupTextEditString(temp);
    return temp;
}
