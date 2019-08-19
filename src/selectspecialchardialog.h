/*
  Copyright (c) 2012-2019 Montel Laurent <montel@kde.org>

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

#ifndef KPIMTEXTEDIT_SELECTSPECIAL_CHARDIALOG_H
#define KPIMTEXTEDIT_SELECTSPECIAL_CHARDIALOG_H

#include "kpimtextedit_export.h"

#include <QDialog>

namespace KPIMTextEdit {
class SelectSpecialCharDialogPrivate;
/**
 * @brief The SelectSpecialCharDialog class
 * @author Laurent Montel <montel@kde.org>
 */
class KPIMTEXTEDIT_EXPORT SelectSpecialCharDialog : public QDialog
{
    Q_OBJECT
public:
    explicit SelectSpecialCharDialog(QWidget *parent);
    ~SelectSpecialCharDialog();

    void setCurrentChar(QChar c);
    Q_REQUIRED_RESULT QChar currentChar() const;

    void setOkButtonText(const QString &text);

    void showSelectButton(bool show);

    /*
     * When we double click we call accept
     */
    void autoInsertChar();

Q_SIGNALS:
    void charSelected(QChar);

private:
    SelectSpecialCharDialogPrivate *const d;
    friend class SelectSpecialCharDialogPrivate;
    void readConfig();
    void writeConfig();
};
}

#endif // KPIMTEXTEDIT_SELECTSPECIAL_CHAR_H
