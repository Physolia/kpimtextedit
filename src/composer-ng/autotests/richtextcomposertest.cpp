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

#include "richtextcomposertest.h"
#include "../richtextcomposer.h"
#include <qtest.h>
#include <qsignalspy.h>
#include <KActionCollection>
#include <KCodecs/KCodecs>

#include <KIconLoader>
#include <KActionCollection>

#include <QTextCursor>
#include <qtestevent.h>
#include <QBuffer>
#include <QTextBlock>
#include <kpimtextedit/richtextcomposercontroler.h>
#include <kpimtextedit/richtextcomposerimages.h>
#include <kpimtextedit/richtextcomposer.h>

using namespace KPIMTextEdit;

Q_DECLARE_METATYPE(KPIMTextEdit::RichTextComposer::Mode)
RichTextComposerTest::RichTextComposerTest(QObject *parent)
    : QObject(parent)
{
    qRegisterMetaType<KPIMTextEdit::RichTextComposer::Mode>();
}

RichTextComposerTest::~RichTextComposerTest()
{

}

void RichTextComposerTest::testFormattingUsed()
{
    // This method tries to test everything that krichtextedit makes available, so
    // we can sure that in KMail, when the user uses some formatting, the mail is actually
    // sent as HTML mail

    KPIMTextEdit::RichTextComposer textEdit;
    textEdit.createActions(new KActionCollection(this));

    QVERIFY(!textEdit.composerControler()->isFormattingUsed());

    // Insert some text.
    QTextCursor cursor(textEdit.document());
    cursor.insertText(QStringLiteral("Hello World!!"));
    QVERIFY(!textEdit.composerControler()->isFormattingUsed());
    cursor.setPosition(1);
    textEdit.setTextCursor(cursor);

    //
    // Test link
    //
    QString someUrl = QStringLiteral("www.test.de");
    QString altText = QStringLiteral("Hello");
    textEdit.composerControler()->updateLink(someUrl, altText);
    QVERIFY(textEdit.composerControler()->isFormattingUsed());
    QCOMPARE(textEdit.composerControler()->currentLinkUrl(), someUrl);
    QCOMPARE(textEdit.composerControler()->currentLinkText(), altText);

    cursor.setPosition(1);
    textEdit.setTextCursor(cursor);
    textEdit.composerControler()->updateLink(QString(), QStringLiteral("Hello"));
    QVERIFY(textEdit.composerControler()->currentLinkUrl().isEmpty());
    QVERIFY(!textEdit.composerControler()->currentLinkText().isEmpty());
    QVERIFY(!textEdit.composerControler()->isFormattingUsed());

    //
    // Test alignment
    //
    cursor.setPosition(1);
    textEdit.setTextCursor(cursor);
    textEdit.composerControler()->alignRight();
    QVERIFY(textEdit.composerControler()->isFormattingUsed());
    QCOMPARE(textEdit.alignment(), Qt::AlignRight);
    textEdit.composerControler()->alignLeft();
    QVERIFY(!textEdit.composerControler()->isFormattingUsed());
    textEdit.composerControler()->alignCenter();
    QCOMPARE(textEdit.alignment(), Qt::AlignHCenter);
    QVERIFY(textEdit.composerControler()->isFormattingUsed());
    textEdit.composerControler()->alignJustify();
    QCOMPARE(textEdit.alignment(), Qt::AlignJustify);
    QVERIFY(textEdit.composerControler()->isFormattingUsed());
    textEdit.composerControler()->alignLeft();
    QCOMPARE(textEdit.alignment(), Qt::AlignLeft);
    QVERIFY(!textEdit.composerControler()->isFormattingUsed());

    //
    // Test layout direction
    //
    textEdit.selectAll();
    QTextCharFormat direction;
    direction.setLayoutDirection(Qt::RightToLeft);
    textEdit.mergeCurrentCharFormat(direction);
    QVERIFY(textEdit.composerControler()->isFormattingUsed());
    direction.setLayoutDirection(Qt::LeftToRight);
    textEdit.mergeCurrentCharFormat(direction);
    QVERIFY(textEdit.composerControler()->isFormattingUsed());

    //
    // Test lists
    //
    textEdit.composerControler()->setListStyle(QTextListFormat::ListCircle);
    QVERIFY(textEdit.composerControler()->isFormattingUsed());
    textEdit.composerControler()->setListStyle(0);
    QVERIFY(textEdit.composerControler()->isFormattingUsed());

    //
    // Test font attributes
    //
    textEdit.setFontFamily(QStringLiteral("Times"));
    QVERIFY(textEdit.composerControler()->isFormattingUsed());
    textEdit.setFontFamily(textEdit.document()->defaultFont().family());
    QVERIFY(textEdit.composerControler()->isFormattingUsed());
    textEdit.composerControler()->setFontSize(48);
    QVERIFY(textEdit.composerControler()->isFormattingUsed());
    textEdit.composerControler()->setFontSize(textEdit.document()->defaultFont().pointSize());
    QVERIFY(textEdit.composerControler()->isFormattingUsed());
    QFont myFont = textEdit.document()->defaultFont();
    myFont.setStyle(QFont::StyleOblique);
    textEdit.composerControler()->setFont(myFont);
    QVERIFY(textEdit.composerControler()->isFormattingUsed());
    textEdit.composerControler()->setFont(textEdit.document()->defaultFont());
    QVERIFY(textEdit.composerControler()->isFormattingUsed());

    //
    // Test bold, italic, underline and strikeout
    //
    textEdit.composerControler()->setTextBold(true);
    QVERIFY(textEdit.composerControler()->isFormattingUsed());
    textEdit.composerControler()->setTextBold(false);
    QVERIFY(textEdit.composerControler()->isFormattingUsed());
    textEdit.composerControler()->setTextUnderline(true);
    QVERIFY(textEdit.composerControler()->isFormattingUsed());
    textEdit.composerControler()->setTextUnderline(false);
    QVERIFY(textEdit.composerControler()->isFormattingUsed());
    textEdit.composerControler()->setTextItalic(true);
    QVERIFY(textEdit.composerControler()->isFormattingUsed());
    textEdit.composerControler()->setTextItalic(false);
    QVERIFY(textEdit.composerControler()->isFormattingUsed());
    textEdit.composerControler()->setTextStrikeOut(true);
    QVERIFY(textEdit.composerControler()->isFormattingUsed());
    textEdit.composerControler()->setTextStrikeOut(false);
    QVERIFY(textEdit.composerControler()->isFormattingUsed());

    //
    // Color
    //
    QColor oldForeground = textEdit.document()->firstBlock().charFormat().foreground().color();
    textEdit.composerControler()->setTextForegroundColor(Qt::red);
    QVERIFY(textEdit.composerControler()->isFormattingUsed());
    textEdit.composerControler()->setTextForegroundColor(oldForeground);
    QVERIFY(textEdit.composerControler()->isFormattingUsed());
    QColor oldBackground = textEdit.document()->firstBlock().charFormat().background().color();
    textEdit.composerControler()->setTextBackgroundColor(Qt::red);
    QVERIFY(textEdit.composerControler()->isFormattingUsed());
    textEdit.composerControler()->setTextBackgroundColor(oldBackground);
    QVERIFY(textEdit.composerControler()->isFormattingUsed());

    //
    // Horizontal rule
    //
    textEdit.composerControler()->insertHorizontalRule();
    QVERIFY(textEdit.composerControler()->isFormattingUsed());
    // No way to easily remove the horizontal line, so clear the text edit and start over
    textEdit.clear();
    cursor.insertText(QStringLiteral("Hello World!!"));
    QVERIFY(!textEdit.composerControler()->isFormattingUsed());
    cursor.setPosition(1);
    textEdit.setTextCursor(cursor);

    //
    // Sub and superscript
    //
    textEdit.composerControler()->setTextSuperScript(true);
    QVERIFY(textEdit.composerControler()->isFormattingUsed());
    textEdit.composerControler()->setTextSuperScript(false);
    QVERIFY(!textEdit.composerControler()->isFormattingUsed());
    textEdit.composerControler()->setTextSubScript(true);
    QVERIFY(textEdit.composerControler()->isFormattingUsed());
    textEdit.composerControler()->setTextSubScript(false);
    QVERIFY(!textEdit.composerControler()->isFormattingUsed());

    //
    // Image
    //
    const QString imagePath = KIconLoader::global()->iconPath(QStringLiteral("folder-new"), KIconLoader::Small, false);
    textEdit.composerControler()->composerImages()->addImage(QUrl::fromLocalFile(imagePath));
    QVERIFY(textEdit.composerControler()->isFormattingUsed());
    cursor = textEdit.textCursor();
    cursor.movePosition(QTextCursor::Left, QTextCursor::KeepAnchor, 1);
    cursor.removeSelectedText();
    QVERIFY(!textEdit.composerControler()->isFormattingUsed());
}

void RichTextComposerTest::testQuoting()
{
    KPIMTextEdit::RichTextComposer edit;
    edit.createActions(new KActionCollection(this));
    QVERIFY(edit.isLineQuoted(QStringLiteral("> Hello")));
    QVERIFY(edit.isLineQuoted(QStringLiteral(">Hello")));
    QVERIFY(!edit.isLineQuoted(QStringLiteral("Hello")));
    QCOMPARE(edit.quoteLength(QStringLiteral("Hello")), 0);
    QCOMPARE(edit.quoteLength(QStringLiteral(">Hello")), 1);
    QCOMPARE(edit.quoteLength(QStringLiteral("> Hello")), 2);
    QCOMPARE(edit.quoteLength(QStringLiteral(">>>Hello")), 3);
    QCOMPARE(edit.quoteLength(QStringLiteral("> > > Hello")), 6);
    QCOMPARE(edit.quoteLength(QStringLiteral("|Hello")), 1);
    QCOMPARE(edit.quoteLength(QStringLiteral("| |Hello")), 3);
}

void RichTextComposerTest::testCleanText()
{
    KPIMTextEdit::RichTextComposer edit;
    edit.createActions(new KActionCollection(this));
    QString html(QStringLiteral("<html><head></head><body>Heelllo&nbsp;World<br>Bye!</body></html>"));
    QString plain(QStringLiteral("Heelllo World\nBye!"));
    edit.setTextOrHtml(html);
    edit.composerControler()->composerImages()->addImage(QUrl::fromLocalFile(KIconLoader::global()->iconPath(QStringLiteral("folder-new"), KIconLoader::Small, false)));
    QVERIFY(edit.textMode() == KPIMTextEdit::RichTextComposer::Rich);
    QCOMPARE(edit.composerControler()->toCleanPlainText(), plain);

    edit.show(); // < otherwise toWrappedPlainText can't work, it needs a layout
    QCOMPARE(edit.composerControler()->toWrappedPlainText(), plain);
}

void RichTextComposerTest::testEnter_data()
{
    QTest::addColumn<QString>("initalText");
    QTest::addColumn<QString>("expectedText");
    QTest::addColumn<int>("cursorPos");

    QTest::newRow("") << QLatin1String("> Hello World")
                      << QLatin1String("> Hello \n> World")
                      << 8;
    QTest::newRow("") << QLatin1String("Hello World")
                      << QLatin1String("Hello \nWorld")
                      << 6;
    QTest::newRow("") << QLatin1String("> Hello World")
                      << QLatin1String("> Hello World\n")
                      << 13;
    QTest::newRow("") << QLatin1String(">Hello World")
                      << QLatin1String(">Hello \n>World")
                      << 7;
    QTest::newRow("") << QLatin1String("> > Hello World")
                      << QLatin1String("> > Hello \n> > World")
                      << 10;
    QTest::newRow("") << QLatin1String("| | Hello World")
                      << QLatin1String("| | Hello \n| | World")
                      << 10;
}

void RichTextComposerTest::testEnter()
{
    QFETCH(QString, initalText);
    QFETCH(QString, expectedText);
    QFETCH(int, cursorPos);

    KPIMTextEdit::RichTextComposer edit;
    edit.createActions(new KActionCollection(this));
    edit.setPlainText(initalText);
    QTextCursor textCursor(edit.document());
    textCursor.setPosition(cursorPos);
    edit.setTextCursor(textCursor);

    QTest::keyClick(&edit, Qt::Key_Return);
    QCOMPARE(edit.toPlainText(), expectedText);
}

void RichTextComposerTest::testImages()
{
    KPIMTextEdit::RichTextComposer edit;
    edit.createActions(new KActionCollection(this));
    QString image1Path = KIconLoader::global()->iconPath(QStringLiteral("folder-new"), KIconLoader::Small, false);
    QString image2Path = KIconLoader::global()->iconPath(QStringLiteral("arrow-up"), KIconLoader::Small, false);

    // Add one image, check that embeddedImages() returns the right stuff
    edit.composerControler()->composerImages()->addImage(QUrl::fromLocalFile(image1Path));
    KPIMTextEdit::ImageList images = edit.composerControler()->composerImages()->embeddedImages();
    KPIMTextEdit::ImageWithNameList imagesWithNames = edit.composerControler()->composerImages()->imagesWithName();
    QCOMPARE(images.size(), 1);
    QCOMPARE(imagesWithNames.size(), 1);
    EmbeddedImage *image = images.first().data();
    ImageWithName *imageWithName = imagesWithNames.first().data();
    QCOMPARE(image->imageName, QString::fromLatin1("folder-new.png"));
    QCOMPARE(imageWithName->name, QString::fromLatin1("folder-new.png"));

    // Also check that it loads the correct image
    QImage diskImage(image1Path);
    QBuffer buffer;
    buffer.open(QIODevice::WriteOnly);
    diskImage.save(&buffer, "PNG");
    QBuffer imageWithNameBuffer;
    imageWithNameBuffer.open(QIODevice::WriteOnly);
    imageWithName->image.save(&imageWithNameBuffer, "PNG");
    QByteArray encodedImage = KCodecs::Codec::codecForName("base64")->encode(buffer.buffer());
    QCOMPARE(image->image, encodedImage);
    QCOMPARE(buffer.buffer(), imageWithNameBuffer.buffer());

    // No image should be there after clearing
    edit.clear();
    QVERIFY(edit.composerControler()->composerImages()->embeddedImages().isEmpty());
    QVERIFY(edit.composerControler()->composerImages()->imagesWithName().isEmpty());

    // Check that manually removing the image also empties the image list
    edit.composerControler()->composerImages()->addImage(QUrl::fromLocalFile(image1Path));
    QCOMPARE(edit.composerControler()->composerImages()->embeddedImages().size(), 1);
    QCOMPARE(edit.composerControler()->composerImages()->imagesWithName().size(), 1);
    QTextCursor cursor = edit.textCursor();
    cursor.setPosition(0, QTextCursor::MoveAnchor);
    cursor.movePosition(QTextCursor::Right, QTextCursor::KeepAnchor, 1);
    cursor.removeSelectedText();
    QVERIFY(edit.composerControler()->composerImages()->embeddedImages().isEmpty());
    QVERIFY(edit.composerControler()->composerImages()->imagesWithName().isEmpty());

    // Check that adding the identical image two times only adds the image once
    edit.composerControler()->composerImages()->addImage(QUrl::fromLocalFile(image1Path));
    edit.composerControler()->composerImages()->addImage(QUrl::fromLocalFile(image1Path));
    QCOMPARE(edit.composerControler()->composerImages()->embeddedImages().size(), 1);
    QCOMPARE(edit.composerControler()->composerImages()->imagesWithName().size(), 1);

    // Another different image added, and we should have two images
    edit.clear();
    edit.composerControler()->composerImages()->addImage(QUrl::fromLocalFile(image1Path));
    edit.composerControler()->composerImages()->addImage(QUrl::fromLocalFile(image2Path));
    images = edit.composerControler()->composerImages()->embeddedImages();
    imagesWithNames = edit.composerControler()->composerImages()->imagesWithName();
    QCOMPARE(images.size(), 2);
    QCOMPARE(imagesWithNames.size(), 2);
    KPIMTextEdit::EmbeddedImage *image1 = images.first().data();
    KPIMTextEdit::EmbeddedImage *image2 = images.last().data();
    KPIMTextEdit::ImageWithName *imageWithName1 = imagesWithNames.first().data();
    KPIMTextEdit::ImageWithName *imageWithName2 = imagesWithNames.last().data();
    QCOMPARE(image1->imageName, QString::fromLatin1("folder-new2.png"));     // ### FIXME: should be folder-new.png, but QTextEdit provides no way to remove cached resources!
    QCOMPARE(imageWithName1->name, QString::fromLatin1("folder-new2.png"));
    QCOMPARE(image2->imageName, QString::fromLatin1("arrow-up.png"));
    QCOMPARE(imageWithName2->name, QString::fromLatin1("arrow-up.png"));
    QVERIFY(image1->contentID != image2->contentID);
}

void RichTextComposerTest::testImageHtmlCode()
{
    KPIMTextEdit::RichTextComposer edit;
    edit.createActions(new KActionCollection(this));
    QString image1Path = KIconLoader::global()->iconPath(QStringLiteral("folder-new"), KIconLoader::Small, false);
    QString image2Path = KIconLoader::global()->iconPath(QStringLiteral("arrow-up"), KIconLoader::Small, false);
    edit.composerControler()->composerImages()->addImage(QUrl::fromLocalFile(image1Path));
    edit.composerControler()->composerImages()->addImage(QUrl::fromLocalFile(image2Path));
    KPIMTextEdit::ImageList images = edit.composerControler()->composerImages()->embeddedImages();
    QCOMPARE(images.size(), 2);
    KPIMTextEdit::EmbeddedImage *image1 = images.first().data();
    KPIMTextEdit::EmbeddedImage *image2 = images.last().data();
    QString startHtml = QStringLiteral("<img src=\"arrow-up.png\"><img src=\"folder-new.png\">Bla<b>Blub</b>");
    QString endHtml = QStringLiteral("<img src=\"cid:%1\"><img src=\"cid:%2\">Bla<b>Blub</b>")
                      .arg(image2->contentID).arg(image1->contentID);
    QCOMPARE(KPIMTextEdit::RichTextComposerImages::imageNamesToContentIds(startHtml.toLatin1(), images), endHtml.toLatin1());
}

void RichTextComposerTest::testDeleteLine_data()
{
    QTest::addColumn<QString>("initalText");
    QTest::addColumn<QString>("expectedText");
    QTest::addColumn<int>("cursorPos");

    QTest::newRow("") << QLatin1String("line1\nline2\nline3")
                      << QLatin1String("line1\nline3")
                      << 6;
    QTest::newRow("") << QLatin1String("line1\nline2\nline3")
                      << QLatin1String("line2\nline3")
                      << 5;
    QTest::newRow("") << QLatin1String("line1\nline2\nline3")
                      << QLatin1String("line1\nline3")
                      << 11;
    QTest::newRow("") << QLatin1String("line1\nline2\nline3")
                      << QLatin1String("line2\nline3")
                      << 0;
    QTest::newRow("") << QLatin1String("line1\nline2\nline3")
                      << QLatin1String("line1\nline2")
                      << 17;
    QTest::newRow("") << QLatin1String("line1")
                      << QLatin1String("")
                      << 0;
    QTest::newRow("") << QLatin1String("line1")
                      << QLatin1String("")
                      << 5;

    // Now, test deletion with word wrapping. The line with the Ms is so long that it will get wrapped
    QTest::newRow("") << QLatin1String("line1\nMMMMMMM MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM\nline3")
                      << QLatin1String("line1\nMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM\nline3")
                      << 6;
    QTest::newRow("") << QLatin1String("line1\nMMMMMMM MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM\nline3")
                      << QLatin1String("line1\nMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM\nline3")
                      << 13;
}

void RichTextComposerTest::testDeleteLine()
{
#if 0
    QFETCH(QString, initalText);
    QFETCH(QString, expectedText);
    QFETCH(int, cursorPos);

    KPIMTextEdit::RichTextComposer edit;
    edit.createActions(new KActionCollection(this));
    edit.setPlainText(initalText);
    QTextCursor cursor = edit.textCursor();
    cursor.setPosition(cursorPos);
    edit.setTextCursor(cursor);

    edit.show(); // we need a layout for this to work

    edit.composerControler()->deleteCurrentLine();
    QCOMPARE(edit.toPlainText(), expectedText);
#endif
}

void RichTextComposerTest::testLoadImage()
{
    KPIMTextEdit::RichTextComposer edit;
    edit.createActions(new KActionCollection(this));
    QString image1Path = KIconLoader::global()->iconPath(QStringLiteral("folder-new"), KIconLoader::Small, false);
    QString image2Path = KIconLoader::global()->iconPath(QStringLiteral("arrow-up"), KIconLoader::Small, false);
    QImage image1, image2;
    QVERIFY(image1.load(image1Path));
    QVERIFY(image2.load(image1Path));

    edit.setHtml(QStringLiteral("Bla<img src=\"folder-new.png\">Bla"));

    // First try to load an image with a name that doesn't match, it should fail
    edit.composerControler()->composerImages()->loadImage(image1, QStringLiteral("doesntmatch"), QStringLiteral("folder-new"));
    QVERIFY(!edit.document()->resource(QTextDocument::ImageResource, QUrl(QStringLiteral("folder-new"))).isValid());

    // Now, load the image for real
    edit.composerControler()->composerImages()->loadImage(image1, QStringLiteral("folder-new.png"), QStringLiteral("folder-new"));
    QVERIFY(edit.document()->resource(QTextDocument::ImageResource, QUrl(QStringLiteral("folder-new"))).isValid());

    // New test with a new textedit (so that we don't use the cached resources
    // This example has two images in the same text block, make sure that doesn't crash (bug 204214)
    KPIMTextEdit::RichTextComposer edit2;
    edit2.createActions(new KActionCollection(this));
    edit2.setHtml(QStringLiteral("<img src=\"folder-new.png\"><img src=\"folder-new.png\">"));
    edit2.composerControler()->composerImages()->loadImage(image1, QStringLiteral("folder-new.png"), QStringLiteral("folder-new"));
    QVERIFY(edit.document()->resource(QTextDocument::ImageResource, QUrl(QStringLiteral("folder-new"))).isValid());
    QCOMPARE(edit.composerControler()->composerImages()->embeddedImages().size(), 1);
}

void RichTextComposerTest::testWrappedPlainText()
{
    KPIMTextEdit::RichTextComposer edit;
    edit.createActions(new KActionCollection(this));
    QString text(QStringLiteral("http://example.org/test-test-test-test-test-test-test-test-test-test-test-test-test\n  https://example.org/test-test-test-test-test-test-test-test-test-test-test-test-test\ntest ftp://example.org/test-test-test-test-test-test-test-test-test-test-test-test-test\nftps://example.org/test-test-test-test-test-test-test-test-test-test-test-test-test\n  ldap://example.org/test-test-test-test-test-test-test-test-test-test-test-test-test"));
    edit.setPlainText(text);

    edit.show(); // < otherwise toWrappedPlainText can't work, it needs a layout

    QCOMPARE(edit.composerControler()->toWrappedPlainText(), text);
}

void RichTextComposerTest::shouldHaveDefaultValue()
{
    KPIMTextEdit::RichTextComposer composer;
    KActionCollection *actionCollection = new KActionCollection(&composer);
    composer.createActions(actionCollection);
    QCOMPARE(composer.linePosition(), 0);
    QCOMPARE(composer.columnNumber(), 0);
    QCOMPARE(composer.textMode(), KPIMTextEdit::RichTextComposer::Plain);
    QVERIFY(!composer.acceptRichText());
    QVERIFY(!composer.quotePrefixName().isEmpty());
}

void RichTextComposerTest::shouldChangeMode()
{
    KPIMTextEdit::RichTextComposer composer;
    KActionCollection *actionCollection = new KActionCollection(&composer);
    composer.createActions(actionCollection);
    QSignalSpy spy(&composer, SIGNAL(textModeChanged(KPIMTextEdit::RichTextComposer::Mode)));
    composer.activateRichText();
    QCOMPARE(composer.textMode(), KPIMTextEdit::RichTextComposer::Rich);
    QVERIFY(composer.acceptRichText());
    QCOMPARE(spy.count(), 1);
}

QTEST_MAIN(RichTextComposerTest)