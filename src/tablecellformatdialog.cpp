/*
  SPDX-FileCopyrightText: 2012-2025 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: LGPL-2.0-or-later

*/

#include "tablecellformatdialog.h"

#include <KColorButton>
#include <KLocalizedString>
#include <KSeparator>
#include <QComboBox>

#include <QCheckBox>
#include <QDialogButtonBox>
#include <QLabel>
#include <QVBoxLayout>

using namespace KPIMTextEdit;

class TableCellFormatDialog::TableCellFormatDialogPrivate
{
public:
    explicit TableCellFormatDialogPrivate(TableCellFormatDialog *qq)
        : q(qq)
    {
        q->setWindowTitle(i18nc("@title:window", "Cell Format"));
        auto mainLayout = new QVBoxLayout;
        q->setLayout(mainLayout);

        auto hbox = new QHBoxLayout;
        auto lab = new QLabel(i18nc("@label:textbox", "Vertical Alignment:"));
        hbox->addWidget(lab);
        verticalAlignment = new QComboBox;
        verticalAlignment->addItem(i18n("Top"), QTextCharFormat::AlignTop);
        verticalAlignment->addItem(i18n("Middle"), QTextCharFormat::AlignMiddle);
        verticalAlignment->addItem(i18n("Bottom"), QTextCharFormat::AlignBottom);

        hbox->addWidget(verticalAlignment);
        mainLayout->addLayout(hbox);

        auto sep = new KSeparator;
        mainLayout->addWidget(sep);

        hbox = new QHBoxLayout;
        useBackgroundColor = new QCheckBox(i18nc("@option:check", "Background Color:"));
        hbox->addWidget(useBackgroundColor);
        backgroundColor = new KColorButton;
        backgroundColor->setDefaultColor(Qt::white);
        hbox->addWidget(backgroundColor);
        mainLayout->addLayout(hbox);

        sep = new KSeparator;
        mainLayout->addWidget(sep);
        backgroundColor->setEnabled(false);
        q->connect(useBackgroundColor, &QCheckBox::toggled, backgroundColor, &KColorButton::setEnabled);

        auto buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, q);
        buttonBox->button(QDialogButtonBox::Ok)->setShortcut(Qt::CTRL | Qt::Key_Return);
        q->connect(buttonBox, &QDialogButtonBox::accepted, q, &QDialog::accept);
        q->connect(buttonBox, &QDialogButtonBox::rejected, q, &QDialog::reject);

        mainLayout->addWidget(buttonBox);
    }

    QCheckBox *useBackgroundColor = nullptr;

    KColorButton *backgroundColor = nullptr;
    QComboBox *verticalAlignment = nullptr;
    TableCellFormatDialog *const q;
};

TableCellFormatDialog::TableCellFormatDialog(QWidget *parent)
    : QDialog(parent)
    , d(new TableCellFormatDialogPrivate(this))
{
}

TableCellFormatDialog::~TableCellFormatDialog() = default;

QColor TableCellFormatDialog::tableCellBackgroundColor() const
{
    return d->backgroundColor->color();
}

void TableCellFormatDialog::setTableCellBackgroundColor(const QColor &col)
{
    d->backgroundColor->setColor(col);
    d->useBackgroundColor->setChecked(true);
}

QTextCharFormat::VerticalAlignment TableCellFormatDialog::verticalAlignment() const
{
    return static_cast<QTextCharFormat::VerticalAlignment>(d->verticalAlignment->itemData(d->verticalAlignment->currentIndex()).toInt());
}

void TableCellFormatDialog::setVerticalAlignment(QTextCharFormat::VerticalAlignment vertical)
{
    d->verticalAlignment->setCurrentIndex(d->verticalAlignment->findData(QVariant(vertical)));
}

bool TableCellFormatDialog::useBackgroundColor() const
{
    return d->useBackgroundColor->isChecked();
}

#include "moc_tablecellformatdialog.cpp"
