/*
Copyright (C) 2011 RVRS Industriis <http://rvrs.in>

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

CommandBarWidget::CommandBarWidget(QWidget* parent, Qt::WindowFlags f) : QWidget(parent, f), ui(new Ui::CommandBar) {
	ui->setupUi(this);

	connect(ui->comboCommand->lineEdit(), SIGNAL(textChanged(const QString&)), this, SLOT(textChanged(const QString&)));
	connect(ui->comboCommand->lineEdit(), SIGNAL(returnPressed()), this, SLOT(returnPressed()));
}

void CommandBarWidget::textChanged(const QString& text) {
	ui->labelInfo->clear();
	if(!text.isEmpty())
	{
		QString cmd_str;
		QStringList arg_list;
		if(split_input(text, cmd_str, arg_list))
		{
			Command cmd(cmd_str, arg_list);
			if(cmd.isValid())
			{
				// Show description of command and possible arguments
				QString info = QString("<b>") + cmd.name() + "</b>: " + cmd.description();

				QStringList arghelp = cmd.arghelp();
				if(!arghelp.empty())
				{
					QString argstr;
					int i = 0;
					Q_FOREACH(const QString& arg, arghelp)
					{
						if(i+1 == arg_list.size())
							argstr += "<b>" + arg + "</b>";
						else
							argstr += arg;
						i++;
						if(i != arghelp.size())
							argstr += ", ";
					}

					info += "<hr /> Arguments: " + argstr;
				}

				ui->labelInfo->setText(info);
			}
			else if(arg_list.empty())
			{
				// Show list of commands beginning with the entered characters
				QList<Command> similar = Command::similar_commands(cmd_str);
				if(!similar.empty())
				{
					QString info;
					int i = 0;
					Q_FOREACH(const Command& cmd_sim, similar)
					{
						info += "<b>" + cmd_sim.name() + "</b> " + cmd_sim.description();
						i++;
						if(i != similar.size())
							info += ", ";
					}
					ui->labelInfo->setText(info);
				}
			}
		}
	}
}

void CommandBarWidget::returnPressed() {
	const QString text = ui->comboCommand->lineEdit()->text();
	if(!text.isEmpty())
	{
		QString cmd_str;
		QStringList arg_list;
		if(split_input(text, cmd_str, arg_list))
		{
			Command cmd(cmd_str, arg_list);
			if(cmd.isValid())
			{
				if(cmd.execute())
				{
					ui->labelInfo->clear();
				}
				else
				{
					ui->labelInfo->setText("<font color=\"Crimson\">" + cmd.error() + "</font>");
				}
			}
			else
			{
				ui->labelInfo->setText("<font color=\"Crimson\">Unknown command: " + cmd_str + "</font>");
			}
		}
	}
}

bool CommandBarWidget::split_input(const QString& input, QString& command, QStringList& arguments)
{
	arguments = input.split(QRegExp("\\s+"), QString::SkipEmptyParts);
	Q_ASSERT(!arguments.empty());

	command = arguments.first();
	arguments.removeFirst();
	return true;
}
