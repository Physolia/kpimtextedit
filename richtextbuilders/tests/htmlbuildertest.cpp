/*
    This file is part of KDE.

    Copyright (c) 2008 Stephen Kelly <steveire@gmail.com>

    This library is free software; you can redistribute it and/or modify it
    under the terms of the GNU Library General Public License as published by
    the Free Software Foundation; either version 2 of the License, or (at your
    option) any later version.

    This library is distributed in the hope that it will be useful, but WITHOUT
    ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
    FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Library General Public
    License for more details.

    You should have received a copy of the GNU Library General Public License
    along with this library; see the file COPYING.LIB.  If not, write to the
    Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
    02110-1301, USA.
*/

#include <QtTest>
#include <QtCore>
#include <qtest_kde.h>
#include <qtestevent.h>
#include <QRegExp>
#include <QTextDocument>
#include <QTextEdit>

#include <kdebug.h>
#include "../markupdirector.h"
#include "../htmlbuilder.h"

class TestHtmlOutput: public QObject
{
    Q_OBJECT
private slots:

// Test paragraph contents:
    void testSingleFormat();
    void testDoubleFormat();
    void testDoubleStartDifferentFinish();
    void testDoubleStartDifferentFinishReverseOrder();
    void testDifferentStartDoubleFinish();
    void testDifferentStartDoubleFinishReverseOrder();
    void testOverlap();
    void testAnchor();
    void testAnchorWithFormattedContent();
    void testAdjacentAnchors();
    void testNestedFormatting();
    void testSpan();
    void testDoubleSpan();
    void testSpanNesting();
    void testEdgeCaseLeft();
    void testEdgeCaseRight();
    void testImage();
    void testImageResized();
    void testEachFormatTagSingly();
    void testHorizontalRule();
    void testNewlines();
    void testEmptyParagraphs();
    void testNewlinesThroughQTextEdit();

};

void TestHtmlOutput::testSingleFormat()
{
    QTextDocument *doc = new QTextDocument();

    // One format
    doc->setHtml ( "This <b>text</b> is bold." );

    HTMLBuilder *hb = new HTMLBuilder();
    MarkupDirector *md = new MarkupDirector ( hb );
    md->constructContent ( doc );
    QString result = hb->getResult();
    QRegExp regex ( QString ( "^<p>This <strong>text</strong> is bold.</p>\\n$" ) );

    QVERIFY ( regex.exactMatch ( result ) );

}

void TestHtmlOutput::testDoubleFormat()
{
    QTextDocument *doc = new QTextDocument();

    // One format
    doc->setHtml ( "Some <b><i>formatted<i></b> text." );

    HTMLBuilder *hb = new HTMLBuilder();
    MarkupDirector *md = new MarkupDirector ( hb );
    md->constructContent ( doc );
    QString result = hb->getResult();
    QRegExp regex ( QString ( "^<p>Some (<strong><em>|<em><strong>)formatted(</em></strong>|</strong></em>) text.</p>\\n$" ) );

    QVERIFY ( regex.exactMatch ( result ) );

}

void TestHtmlOutput::testAnchor()
{
    QTextDocument *doc = new QTextDocument();
    doc->setHtml ( "A <a href=\"http://www.kde.org\">link</a> to KDE." );

    HTMLBuilder *hb = new HTMLBuilder();
    MarkupDirector *md = new MarkupDirector ( hb );
    md->constructContent ( doc );
    QString result = hb->getResult();

    QRegExp regex ( QString ( "^<p>A <a href=\"http://www.kde.org\">link</a> to KDE.</p>\\n$" ) );

    QVERIFY ( regex.exactMatch ( result ) );

}

void TestHtmlOutput::testAnchorWithFormattedContent()
{
    QTextDocument *doc = new QTextDocument();
    doc->setHtml ( "A <a href=\"http://www.kde.org\"><b>formatted</b> link</a> to KDE." );

    HTMLBuilder *hb = new HTMLBuilder();
    MarkupDirector *md = new MarkupDirector ( hb );
    md->constructContent ( doc );
    QString result = hb->getResult();

//     kDebug() << result;

    QRegExp regex ( QString ( "^<p>A <a href=\"http://www.kde.org\"><strong>formatted</strong> link</a> to KDE.</p>\\n$" ) );

    QVERIFY ( regex.exactMatch ( result ) );

}

void TestHtmlOutput::testAdjacentAnchors()
{
    QTextDocument *doc = new QTextDocument();
    doc->setHtml ( "Two <a href=\"http://www.kde.org\">links</a><a href=\"http://www.google.com\">next</a> to eachother." );

    HTMLBuilder *hb = new HTMLBuilder();
    MarkupDirector *md = new MarkupDirector ( hb );
    md->constructContent ( doc );
    QString result = hb->getResult();

    QRegExp regex ( QString ( "^<p>Two <a href=\"http://www.kde.org\">links</a><a href=\"http://www.google.com\">next</a> to eachother.</p>\\n$" ) );

    QVERIFY ( regex.exactMatch ( result ) );
}

void TestHtmlOutput::testNestedFormatting()
{
    QTextDocument *doc = new QTextDocument();
    doc->setHtml ( "This <b>text is <i>italic</i> and</b> bold." );

    HTMLBuilder *hb = new HTMLBuilder();
    MarkupDirector *md = new MarkupDirector ( hb );
    md->constructContent ( doc );
    QString result = hb->getResult();

    QRegExp regex ( QString ( "^<p>This <strong>text is <em>italic</em> and</strong> bold.</p>\\n$" ) );

    QVERIFY ( regex.exactMatch ( result ) );
}

void TestHtmlOutput::testSpan()
{
    QTextDocument *doc = new QTextDocument();
    doc->setHtml ( "Some <span style=\"color:#ff0000;\">formatted</span> text." );

    HTMLBuilder *hb = new HTMLBuilder();
    MarkupDirector *md = new MarkupDirector ( hb );
    md->constructContent ( doc );
    QString result = hb->getResult();

    QRegExp regex = QRegExp( "^<p>Some <span style=\"color:#ff0000;\">formatted</span> text.</p>\\n$" );

    QVERIFY ( regex.exactMatch ( result ) );
}

void TestHtmlOutput::testDoubleSpan()
{
    QTextDocument *doc = new QTextDocument();
    doc->setHtml ( "Some <span style=\"color:#ff0000;background-color:#00ff00;\">formatted</span> text." );

    HTMLBuilder *hb = new HTMLBuilder();
    MarkupDirector *md = new MarkupDirector ( hb );
    md->constructContent ( doc );
    QString result = hb->getResult();

    QRegExp regex = QRegExp( "^<p>Some <span style=\"(color:#ff0000|background-color:#00ff00);\"><span style=\"(color:#ff0000|background-color:#00ff00);\">formatted</span></span> text.</p>\\n$" );

    QVERIFY ( regex.exactMatch ( result ) );

}

void TestHtmlOutput::testSpanNesting()
{
    QTextDocument *doc = new QTextDocument();
    doc->setHtml ( "Paragraph <span style=\"background-color:#00ff00;\">with some <span style=\"color:#ff0000;\">formatted</span> nested</span> text." );

    HTMLBuilder *hb = new HTMLBuilder();
    MarkupDirector *md = new MarkupDirector ( hb );
    md->constructContent ( doc );
    QString result = hb->getResult();

    QRegExp regex = QRegExp( "^<p>Paragraph <span style=\"background-color:#00ff00;\">with some <span style=\"color:#ff0000;\">formatted</span> nested</span> text.</p>\\n$" );

    QVERIFY ( regex.exactMatch ( result ) );
}

void TestHtmlOutput::testDoubleStartDifferentFinish()
{
    QTextDocument *doc = new QTextDocument();
    doc->setHtml ( "Paragraph <i><b>with</b> some formatted</i> text." );

    HTMLBuilder *hb = new HTMLBuilder();
    MarkupDirector *md = new MarkupDirector ( hb );
    md->constructContent ( doc );
    QString result = hb->getResult();

    QRegExp regex = QRegExp( "^<p>Paragraph <em><strong>with</strong> some formatted</em> text.</p>\\n$" );

    QVERIFY ( regex.exactMatch ( result ) );
}

void TestHtmlOutput::testDoubleStartDifferentFinishReverseOrder()
{
    // The order of closing and opening tags can determine whether generated html is valid or not.
    // For example, when MarkupDirector processes a block, it first closes any open tags in a certain order,
    // Then opens any new tags. Part of this order is: bold > italic > underline > strikeout.
    // When processing a document with formatting which appears as '<b><i>Some</i> formatted<b> text',
    // the generated output will correctly contain '<strong><em>Some</em> formatted<strong> text'.
    // However, processing text which appears as '<i><b>Some</b> formatted<i> text' would be incorrectly rendered
    // as '<strong><em>Some</strong> formatted</em> text'. Tags are not nested properly, and the html would
    // not be valid or render correctly by unforgiving parsers (like QTextEdit).
    // One solution is to make the order of opening tags dynamic. In the above case, the em tag would be opened before
    // the strong tag '<em><strong>Some</strong> formatted</em> text'. That would require knowledge of
    // which tag is going to close first. That might be possible by examining the 'next' QTextFragment
    // while processing one.
    // The other option is to do pessimistic closing of tags.
    // In the above case, this means that if a fragment has two or more formats applied (bold and italic here),
    // and one of lower precedence in the list closes first, then all higher precedence tags should be
    // closed first. They will of course be reopened if necessary while processing the next fragment.
    // The above case would be rendered as '<strong><em>Some</em></strong><em> formatted</em> text'.
    //
    // The precedence list is the order tags are opened and closed in processFragment. It is also the order
    // of items in the OpenElementValues enum.

    QTextDocument *doc = new QTextDocument();
    doc->setHtml ( "Paragraph <b><i>with</i> some formatted</b> text." );

    HTMLBuilder *hb = new HTMLBuilder();
    MarkupDirector *md = new MarkupDirector ( hb );
    md->constructContent ( doc );
    QString result = hb->getResult();

    QRegExp regex = QRegExp( "^<p>Paragraph <strong><em>with</em> some formatted</strong> text.</p>\\n$" );

    QVERIFY ( regex.exactMatch ( result ) );

}

void TestHtmlOutput::testDifferentStartDoubleFinish()
{
    QTextDocument *doc = new QTextDocument();
    doc->setHtml ( "Paragraph <i>with some <b>formatted<b></i> text." );

    HTMLBuilder *hb = new HTMLBuilder();
    MarkupDirector *md = new MarkupDirector ( hb );
    md->constructContent ( doc );
    QString result = hb->getResult();

    QRegExp regex = QRegExp( "^<p>Paragraph <em>with some <strong>formatted</strong></em> text.</p>\\n$" );

    QVERIFY ( regex.exactMatch ( result ) );
}

void TestHtmlOutput::testDifferentStartDoubleFinishReverseOrder()
{
    QTextDocument *doc = new QTextDocument();
    doc->setHtml ( "Paragraph <b>with some <i>formatted</i></b> text." );

    HTMLBuilder *hb = new HTMLBuilder();
    MarkupDirector *md = new MarkupDirector ( hb );
    md->constructContent ( doc );
    QString result = hb->getResult();

    QRegExp regex = QRegExp( "^<p>Paragraph <strong>with some <em>formatted</em></strong> text.</p>\\n$" );

    QVERIFY ( regex.exactMatch ( result ) );

}

void TestHtmlOutput::testOverlap()
{
    QTextDocument *doc = new QTextDocument();
    doc->setHtml ( "Paragraph <b>with <i>some</i></b><i> formatted</i> text." );

    HTMLBuilder *hb = new HTMLBuilder();
    MarkupDirector *md = new MarkupDirector ( hb );
    md->constructContent ( doc );
    QString result = hb->getResult();

    QRegExp regex = QRegExp( "^<p>Paragraph <strong>with <em>some</em></strong><em> formatted</em> text.</p>\\n$" );

    QVERIFY ( regex.exactMatch ( result ) );

}

void TestHtmlOutput::testEdgeCaseLeft()
{
    QTextDocument *doc = new QTextDocument();
    doc->setHtml ( "Paragraph <b>with some formatted text.</b>" );

    HTMLBuilder *hb = new HTMLBuilder();
    MarkupDirector *md = new MarkupDirector ( hb );
    md->constructContent ( doc );
    QString result = hb->getResult();

    QRegExp regex = QRegExp( "^<p>Paragraph <strong>with some formatted text.</strong></p>\\n$" );

    QVERIFY ( regex.exactMatch ( result ) );
}

void TestHtmlOutput::testEdgeCaseRight()
{
    QTextDocument *doc = new QTextDocument();
    doc->setHtml ( "<b>Paragraph with some formatted</b> text." );

    HTMLBuilder *hb = new HTMLBuilder();
    MarkupDirector *md = new MarkupDirector ( hb );
    md->constructContent ( doc );
    QString result = hb->getResult();

    QRegExp regex = QRegExp( "^<p><strong>Paragraph with some formatted</strong> text.</p>\\n$" );

    QVERIFY ( regex.exactMatch ( result ) );
}

void TestHtmlOutput::testImage()
{
    QTextDocument *doc = new QTextDocument();
    doc->setHtml ( "Paragraph with an inline <img src=\"http://kde.org/img/kde41.png\" /> image." );

    HTMLBuilder *hb = new HTMLBuilder();
    MarkupDirector *md = new MarkupDirector ( hb );
    md->constructContent ( doc );
    QString result = hb->getResult();

    QRegExp regex = QRegExp( "^<p>Paragraph with an inline <img src=\"http://kde.org/img/kde41.png\" /> image.</p>\\n$" );

    QVERIFY ( regex.exactMatch ( result ) );
}

void TestHtmlOutput::testImageResized()
{
    QString result;
    QRegExp regex;
    HTMLBuilder *hb;
    MarkupDirector *md;
    QTextDocument *doc = new QTextDocument();

    // width
    doc->setHtml ( "Paragraph with an inline <img src=\"http://kde.org/img/kde41.png\" width=\"10\" /> image." );

    hb = new HTMLBuilder();
    md = new MarkupDirector ( hb );
    md->constructContent ( doc );
    result = hb->getResult();

    regex = QRegExp( "^<p>Paragraph with an inline <img src=\"http://kde.org/img/kde41.png\" width=\"10\" /> image.</p>\\n$" );
    QVERIFY ( regex.exactMatch ( result ) );

    // height
    doc->setHtml ( "Paragraph with an inline <img src=\"http://kde.org/img/kde41.png\" height=\"10\" /> image." );

    hb = new HTMLBuilder();
    md = new MarkupDirector ( hb );
    md->constructContent ( doc );
    result = hb->getResult();

    regex = QRegExp( "^<p>Paragraph with an inline <img src=\"http://kde.org/img/kde41.png\" height=\"10\" /> image.</p>\\n$" );
    QVERIFY ( regex.exactMatch ( result ) );

    // height and width
    doc->setHtml ( "Paragraph with an inline <img src=\"http://kde.org/img/kde41.png\" height=\"10\" width=\"10\" /> image." );

    hb = new HTMLBuilder();
    md = new MarkupDirector ( hb );
    md->constructContent ( doc );
    result = hb->getResult();

    regex = QRegExp( "^<p>Paragraph with an inline <img src=\"http://kde.org/img/kde41.png\" width=\"10\" height=\"10\" /> image.</p>\\n$" );

    QVERIFY ( regex.exactMatch ( result ) );
}

void TestHtmlOutput::testEachFormatTagSingly()
{
    QString result;
    QRegExp regex;
    HTMLBuilder *hb;
    MarkupDirector *md;
    QTextDocument *doc = new QTextDocument();


    // Test bold
    doc->setHtml ( "Some <b>formatted</b> text." );
    hb = new HTMLBuilder();
    md = new MarkupDirector ( hb );
    md->constructContent ( doc );
    result = hb->getResult();

    regex = QRegExp( "^<p>Some <strong>formatted</strong> text.</p>\\n$" );
    QVERIFY ( regex.exactMatch ( result ) );


    // Test Italic
    doc->setHtml ( "Some <i>formatted</i> text." );
    hb = new HTMLBuilder();
    md = new MarkupDirector ( hb );
    md->constructContent ( doc );
    result = hb->getResult();

    regex = QRegExp( "^<p>Some <em>formatted</em> text.</p>\\n$" );
    QVERIFY ( regex.exactMatch ( result ) );

    // Test Underline
    doc->setHtml ( "Some <u>formatted</u> text." );
    hb = new HTMLBuilder();
    md = new MarkupDirector ( hb );
    md->constructContent ( doc );
    result = hb->getResult();

    regex = QRegExp( "^<p>Some <u>formatted</u> text.</p>\\n$" );
    QVERIFY ( regex.exactMatch ( result ) );

    // Test Strikeout
    doc->setHtml ( "Some <s>formatted</s> text." );
    hb = new HTMLBuilder();
    md = new MarkupDirector ( hb );
    md->constructContent ( doc );
    result = hb->getResult();

    regex = QRegExp( "^<p>Some <s>formatted</s> text.</p>\\n$" );
    QVERIFY ( regex.exactMatch ( result ) );

    // Test Superscript
    doc->setHtml ( "Some <sup>formatted</sup> text." );
    hb = new HTMLBuilder();
    md = new MarkupDirector ( hb );
    md->constructContent ( doc );
    result = hb->getResult();

    regex = QRegExp( "^<p>Some <sup>formatted</sup> text.</p>\\n$" );
    QVERIFY ( regex.exactMatch ( result ) );

    // Test Subscript
    doc->setHtml ( "Some <sub>formatted</sub> text." );
    hb = new HTMLBuilder();
    md = new MarkupDirector ( hb );
    md->constructContent ( doc );
    result = hb->getResult();

    regex = QRegExp( "^<p>Some <sub>formatted</sub> text.</p>\\n$" );
    QVERIFY ( regex.exactMatch ( result ) );

    // Test Foreground
    doc->setHtml ( "Some <span style=\"color:#ff0000;\">formatted</span> text." );
    hb = new HTMLBuilder();
    md = new MarkupDirector ( hb );
    md->constructContent ( doc );
    result = hb->getResult();

    regex = QRegExp( "^<p>Some <span style=\"color:#ff0000;\">formatted</span> text.</p>\\n$" );
    QVERIFY ( regex.exactMatch ( result ) );


    // Test Background
    doc->setHtml ( "Some <span style=\"background-color:#ff0000;\">formatted</span> text." );
    hb = new HTMLBuilder();
    md = new MarkupDirector ( hb );
    md->constructContent ( doc );
    result = hb->getResult();

    regex = QRegExp( "^<p>Some <span style=\"background-color:#ff0000;\">formatted</span> text.</p>\\n$" );
    QVERIFY ( regex.exactMatch ( result ) );

    // Test Font Family
    doc->setHtml ( "Some <span style=\"font-family:courier;\">formatted</span> text." );
    hb = new HTMLBuilder();
    md = new MarkupDirector ( hb );
    md->constructContent ( doc );
    result = hb->getResult();

    regex = QRegExp( "^<p>Some <span style=\"font-family:courier;\">formatted</span> text.</p>\\n$" );
    QVERIFY ( regex.exactMatch ( result ) );

    // Test Font Size
    doc->setHtml ( "Some <span style=\"font-size:20pt;\">formatted</span> text." );
    hb = new HTMLBuilder();
    md = new MarkupDirector ( hb );
    md->constructContent ( doc );
    result = hb->getResult();

    regex = QRegExp( "^<p>Some <span style=\"font-size:20pt;\">formatted</span> text.</p>\\n$" );
    QVERIFY ( regex.exactMatch ( result ) );

}

void TestHtmlOutput::testHorizontalRule()
{
    QTextDocument *doc = new QTextDocument();
    doc->setHtml ( "<p style=\"margin-top:0;margin-bottom:0;\">Foo</p><hr /><p style=\"margin-top:0;margin-bottom:0;\">Bar</p>" );

    HTMLBuilder *hb = new HTMLBuilder();
    MarkupDirector *md = new MarkupDirector ( hb );
    md->constructContent ( doc );
    QString result = hb->getResult();

    QRegExp regex = QRegExp( "^<p>Foo</p>\\n<hr />\\n<p>Bar</p>\\n$" );

    QVERIFY ( regex.exactMatch ( result ) );
}

void TestHtmlOutput::testNewlines()
{
    QTextDocument *doc = new QTextDocument();
    doc->setHtml ( "<p style=\"margin-top:0;margin-bottom:0;\">Foo</p>\n<br /><br />\n<p style=\"margin-top:0;margin-bottom:0;\">Bar</p>" );

    HTMLBuilder *hb = new HTMLBuilder();
    MarkupDirector *md = new MarkupDirector ( hb );
    md->constructContent ( doc );
    QString result = hb->getResult();

    QRegExp regex = QRegExp( "^<p>Foo</p>\\n<br />\\n<br />\\n<p>Bar</p>\\n$" );

    QVERIFY ( regex.exactMatch ( result ) );
}

void TestHtmlOutput::testEmptyParagraphs()
{
    QTextDocument *doc = new QTextDocument();
    doc->setHtml ( "<p style=\"margin-top:0;margin-bottom:0;\">Foo</p>\n<p><br /></p>\n<p><br /></p>\n<p style=\"margin-top:0;margin-bottom:0;\">Bar</p>" );

    HTMLBuilder *hb = new HTMLBuilder();
    MarkupDirector *md = new MarkupDirector ( hb );
    md->constructContent ( doc );
    QString result = hb->getResult();

    QRegExp regex = QRegExp( "^<p>Foo</p>\\n<br />\\n<br />\\n<p>Bar</p>\\n$" );

    QVERIFY ( regex.exactMatch ( result ) );
}

void TestHtmlOutput::testNewlinesThroughQTextEdit()
{

    QTextEdit *te = new QTextEdit();
    QTextCursor cursor = te->textCursor();
    cursor.movePosition(QTextCursor::Start);
    cursor.insertText("Foo");
    cursor.insertText("\n");
    cursor.insertText("\n");
    cursor.insertText("\n");
    cursor.insertText("Bar");

    HTMLBuilder *hb = new HTMLBuilder();
    MarkupDirector *md = new MarkupDirector ( hb );
    md->constructContent ( te->document() );
    QString result = hb->getResult();

    QRegExp regex = QRegExp( "^<p>Foo</p>\\n<br />\\n<br />\\n<p>Bar</p>\\n$" );

    QVERIFY ( regex.exactMatch ( result ) );
}

QTEST_KDEMAIN ( TestHtmlOutput, GUI )
#include "htmlbuildertest.moc"
