/*
  Copyright (c) 2012 Montel Laurent <montel@kde.org>
  
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

#include "tableactionmenu.h"
#include "textedit.h"
#include "inserttabledialog.h"
#include "tableformatdialog.h"
#include <KActionCollection>
#include <KLocale>
#include <QTextTable>

namespace KPIMTextEdit {

class TableActionMenuPrivate
{
public:
  TableActionMenuPrivate(KActionCollection *ac, TextEdit *edit,TableActionMenu *qq)
    : actionCollection( ac ),
      textEdit(edit),
      q(qq)
  {

  }

  void _k_slotInsertRowBelow();
  void _k_slotInsertRowAbove();
  void _k_slotInsertColumnBefore();
  void _k_slotInsertColumnAfter();

  void _k_slotInsertTable();

  void _k_slotRemoveRowBelow();
  void _k_slotRemoveRowAbove();
  void _k_slotRemoveColumnBefore();
  void _k_slotRemoveColumnAfter();
  void _k_slotMergeCell();
  void _k_slotTableFormat();
  void _k_slotSplitCell();

  KAction *actionInsertTable;

  KAction *actionInsertRowBelow;
  KAction *actionInsertRowAbove;

  KAction *actionInsertColumnBefore;
  KAction *actionInsertColumnAfter;


  KAction *actionRemoveRowBelow;
  KAction *actionRemoveRowAbove;

  KAction *actionRemoveColumnBefore;
  KAction *actionRemoveColumnAfter;

  KAction *actionMergeCell;
  KAction *actionSplitCell;

  KAction *actionTableFormat;

  KActionCollection *actionCollection;
  TextEdit *textEdit;
  TableActionMenu *q;
};

void TableActionMenuPrivate::_k_slotRemoveRowBelow()
{
  if(textEdit->textMode() == KRichTextEdit::Rich ) {
    QTextTable *table = textEdit->textCursor().currentTable();
    if(table) {
      QTextTableCell cell = table->cellAt(textEdit->textCursor());
      //TODO:
    }
  }
}

void TableActionMenuPrivate::_k_slotRemoveRowAbove()
{
  if(textEdit->textMode() == KRichTextEdit::Rich ) {
    QTextTable *table = textEdit->textCursor().currentTable();
    if(table) {
      QTextTableCell cell = table->cellAt(textEdit->textCursor());
      //TODO:
    }
  }
}

void TableActionMenuPrivate::_k_slotRemoveColumnBefore()
{
  if(textEdit->textMode() == KRichTextEdit::Rich ) {
    QTextTable *table = textEdit->textCursor().currentTable();
    if(table) {
      QTextTableCell cell = table->cellAt(textEdit->textCursor());
      if(cell.column()>0) {
        table->removeColumns(cell.column()-1,1);
      }
    }
  }
}

void TableActionMenuPrivate::_k_slotRemoveColumnAfter()
{
  if(textEdit->textMode() == KRichTextEdit::Rich ) {
    QTextTable *table = textEdit->textCursor().currentTable();
    if(table) {
      QTextTableCell cell = table->cellAt(textEdit->textCursor());
      if(cell.column()<table->columns() - 1) {
        table->removeColumns(cell.column(),1);
      }
    }
  }
}



void TableActionMenuPrivate::_k_slotInsertRowBelow()
{
  if(textEdit->textMode() == KRichTextEdit::Rich ) {
    QTextTable *table = textEdit->textCursor().currentTable();
    if(table) {
      QTextTableCell cell = table->cellAt(textEdit->textCursor());
      if(cell.row()<table->rows()) {
        table->insertRows(cell.row()+1,1);
      } else {
        table->appendRows(1);
      }
    }
  }
}

void TableActionMenuPrivate::_k_slotInsertRowAbove()
{
  if(textEdit->textMode() == KRichTextEdit::Rich ) {
    QTextTable *table = textEdit->textCursor().currentTable();
    if(table) {
      QTextTableCell cell = table->cellAt(textEdit->textCursor());
      table->insertRows(cell.row(),1);
    }
  }
}

void TableActionMenuPrivate::_k_slotInsertColumnBefore()
{
  if(textEdit->textMode() == KRichTextEdit::Rich ) {
    QTextTable *table = textEdit->textCursor().currentTable();
    if(table) {
      QTextTableCell cell = table->cellAt(textEdit->textCursor());
      table->insertColumns(cell.column(),1);
    }
  }
}

void TableActionMenuPrivate::_k_slotInsertColumnAfter()
{
  if(textEdit->textMode() == KRichTextEdit::Rich ) {
    QTextTable *table = textEdit->textCursor().currentTable();
    if(table) {
      QTextTableCell cell = table->cellAt(textEdit->textCursor());
      if(cell.column()<table->columns()) {
        table->insertColumns(cell.column()+1,1);
      } else {
        table->appendColumns(1);
      }
    }
  }
}


void TableActionMenuPrivate::_k_slotInsertTable()
{
  if(textEdit->textMode() == KRichTextEdit::Rich ) {
    InsertTableDialog *dialog = new InsertTableDialog(textEdit);
    if(dialog->exec()) {
      QTextCursor cursor = textEdit->textCursor();
      QTextTableFormat tableFormat;
      tableFormat.setBorder(dialog->border());
      cursor.insertTable( dialog->rows(),dialog->columns(), tableFormat );
    }
    delete dialog;
  }
}

void TableActionMenuPrivate::_k_slotMergeCell()
{
  if(textEdit->textMode() == KRichTextEdit::Rich ) {
    QTextTable *table = textEdit->textCursor().currentTable();
    if(table) {
      QTextTableCell cell = table->cellAt(textEdit->textCursor());
      table->mergeCells(cell.row(),cell.column(), 0, 1 );
    }
  }
}

void TableActionMenuPrivate::_k_slotTableFormat()
{
  if(textEdit->textMode() == KRichTextEdit::Rich ) {
    QTextTable *table = textEdit->textCursor().currentTable();
    if(table) {
      TableFormatDialog *dialog = new TableFormatDialog(textEdit);
      if(dialog->exec()) {

      }
      delete dialog;
      //TODO:
    }
  }
}

void TableActionMenuPrivate::_k_slotSplitCell()
{
  if(textEdit->textMode() == KRichTextEdit::Rich ) {
    QTextTable *table = textEdit->textCursor().currentTable();
    if(table) {
      //TODO:
    }
  }
}

TableActionMenu::TableActionMenu(KActionCollection *ac, TextEdit *textEdit)
  : KActionMenu(textEdit), d(new TableActionMenuPrivate(ac, textEdit,this))
{
    KActionMenu *insertMenu = new KActionMenu(i18n("Insert"),this);
    addAction(insertMenu);

    d->actionInsertTable = new KAction( i18n( "Table..." ), this );
    insertMenu->addAction(d->actionInsertTable);
    ac->addAction( QLatin1String( "insert_new_table" ), d->actionInsertTable );
    connect( d->actionInsertTable, SIGNAL(triggered(bool)), SLOT(_k_slotInsertTable()) );

    insertMenu->addSeparator();
    d->actionInsertRowBelow = new KAction( i18n( "Row Below" ), this );
    insertMenu->addAction(d->actionInsertRowBelow);
    ac->addAction( QLatin1String( "insert_row_below" ), d->actionInsertRowBelow );
    connect( d->actionInsertRowBelow, SIGNAL(triggered(bool)), SLOT(_k_slotInsertRowBelow()) );

    d->actionInsertRowAbove = new KAction( i18n( "Row Above" ), this );
    insertMenu->addAction(d->actionInsertRowAbove);
    ac->addAction( QLatin1String( "insert_row_above" ), d->actionInsertRowAbove );
    connect( d->actionInsertRowAbove, SIGNAL(triggered(bool)), SLOT(_k_slotInsertRowAbove()) );

    insertMenu->addSeparator();
    d->actionInsertColumnBefore = new KAction( i18n( "Column Before" ), this );
    insertMenu->addAction(d->actionInsertColumnBefore);
    ac->addAction( QLatin1String( "insert_column_before" ), d->actionInsertColumnBefore );

    connect( d->actionInsertColumnBefore, SIGNAL(triggered(bool)), SLOT(_k_slotInsertColumnBefore()) );

    d->actionInsertColumnAfter = new KAction( i18n( "Column After" ), this );
    insertMenu->addAction(d->actionInsertColumnAfter);
    ac->addAction( QLatin1String( "insert_column_after" ), d->actionInsertColumnAfter );
    connect( d->actionInsertColumnAfter, SIGNAL(triggered(bool)), SLOT(_k_slotInsertColumnAfter()) );

    KActionMenu *removeMenu = new KActionMenu(i18n("Delete"),this);
    addAction(removeMenu);


    d->actionRemoveRowBelow = new KAction( i18n( "Row Below" ), this );
    removeMenu->addAction(d->actionRemoveRowBelow);
    ac->addAction( QLatin1String( "remove_row_below" ), d->actionRemoveRowBelow );
    connect( d->actionRemoveRowBelow, SIGNAL(triggered(bool)), SLOT(_k_slotRemoveRowBelow()) );

    d->actionRemoveRowAbove = new KAction( i18n( "Row Above" ), this );
    removeMenu->addAction(d->actionRemoveRowAbove);
    ac->addAction( QLatin1String( "remove_row_above" ), d->actionRemoveRowAbove );
    connect( d->actionRemoveRowAbove, SIGNAL(triggered(bool)), SLOT(_k_slotRemoveRowAbove()) );

    removeMenu->addSeparator();
    d->actionRemoveColumnBefore = new KAction( i18n( "Column Before" ), this );
    removeMenu->addAction(d->actionRemoveColumnBefore);
    ac->addAction( QLatin1String( "remove_column_before" ), d->actionRemoveColumnBefore );

    connect( d->actionRemoveColumnBefore, SIGNAL(triggered(bool)), SLOT(_k_slotRemoveColumnBefore()) );

    d->actionRemoveColumnAfter = new KAction( i18n( "Column After" ), this );
    removeMenu->addAction(d->actionRemoveColumnAfter);
    ac->addAction( QLatin1String( "remove_column_after" ), d->actionRemoveColumnAfter );
    connect( d->actionRemoveColumnAfter, SIGNAL(triggered(bool)), SLOT(_k_slotRemoveColumnAfter()) );


    d->actionMergeCell = new KAction(i18n("Join With Cell to the Right"),this);
    ac->addAction( QLatin1String( "join_cell_to_the_right" ), d->actionMergeCell );
    connect( d->actionMergeCell, SIGNAL(triggered(bool)), SLOT(_k_slotMergeCell()) );
    addAction(d->actionMergeCell);
    addSeparator();

    d->actionSplitCell = new KAction(i18n("Split cells"),this);
    ac->addAction( QLatin1String( "split_cells" ), d->actionSplitCell );
    connect( d->actionSplitCell, SIGNAL(triggered(bool)), SLOT(_k_slotSplitCell()) );
    addAction(d->actionSplitCell);


    d->actionTableFormat = new KAction(i18n("Table Format..."),this);
    ac->addAction( QLatin1String( "table_format" ), d->actionTableFormat );
    connect( d->actionTableFormat, SIGNAL(triggered(bool)), SLOT(_k_slotTableFormat()) );
    addAction(d->actionTableFormat);

}

TableActionMenu::~TableActionMenu()
{
  delete d;
}
}

#include "tableactionmenu.moc"