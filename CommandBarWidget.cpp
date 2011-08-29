/*
Copyright (C) 2011 Christopher Daun

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "CommandBarWidget.h"
#include "ui_commandbar.h"
#include "Command.h"
#include <QComboBox>
#include <QLineEdit>
#include <QLabel>
#include <QString>
#include <QStringList>
#include <QRegExp>

CommandBarWidget::CommandBarWidget(QWidget *parent, Qt::WindowFlags f) : QWidget(parent, f), ui(new Ui::CommandBar), mouse_pressed_(false) {
	ui->setupUi(this);

	connect(ui->comboCommand->lineEdit(), SIGNAL(textChanged(const QString&)), this, SLOT(textChanged(const QString&)));
	connect(ui->comboCommand->lineEdit(), SIGNAL(returnPressed()), this, SLOT(returnPressed()));
}

void CommandBarWidget::textChanged(const QString& text) {
	ui->labelInfo->clear();
	if(text.length() > 0)
	{
		QStringList parts = text.split(QRegExp("\\s+"), QString::SkipEmptyParts);
		Q_ASSERT(parts.length() > 0);

		const QString cmd_str = parts.first().toUpper();
		Command cmd(cmd_str, QStringList());
		if(cmd.isValid())
		{
			QStringList arguments = cmd.arghelp();
			QString argstr;
			int i = 0;
			Q_FOREACH(const QString& arg, arguments)
			{
				if(i+1 == parts.size()-1)
					argstr += "<b>" + arg + "</b>";
				else
					argstr += arg;
				i++;
				if(i != arguments.size())
					argstr += ", ";
			}
			ui->labelInfo->setText(QString("<b>") + cmd_str + "</b>: " + cmd.name() + "<hr />" + "Arguments: " + argstr);
		}
	}
}

void CommandBarWidget::returnPressed() {
	const QString text = ui->comboCommand->lineEdit()->text();
	if(text.length() > 0)
	{
		QStringList parts = text.split(QRegExp("\\s+"), QString::SkipEmptyParts);
		Q_ASSERT(parts.length() > 0);

		const QString cmd_str = parts.first();
		parts.removeFirst();
		Command cmd(cmd_str, parts);
		if(cmd.isValid())
		{
			if(!cmd.execute())
			{

			}
		}
	}
}
