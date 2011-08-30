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
#include <QRegExp>
#include <QEvent>
#include <QPalette>

const QString CommandBarWidget::tip_text("Enter a command...");

CommandBarWidget::CommandBarWidget(QWidget* parent, Qt::WindowFlags f) : QWidget(parent, f), ui(new Ui::CommandBar), has_tip_(true) {
	ui->setupUi(this);

	connect(ui->comboCommand->lineEdit(), SIGNAL(textChanged(const QString&)), this, SLOT(textChanged(const QString&)));
	connect(ui->comboCommand->lineEdit(), SIGNAL(returnPressed()), this, SLOT(returnPressed()));

	ui->comboCommand->installEventFilter(this);

	pal_new_ = pal_old_ = ui->comboCommand->lineEdit()->palette();
	pal_new_.setColor(QPalette::Text, Qt::lightGray);

	ui->comboCommand->lineEdit()->setPalette(pal_new_);
	ui->comboCommand->lineEdit()->setText(tip_text);
}

bool CommandBarWidget::eventFilter(QObject* object, QEvent* event)
{
	if(object == ui->comboCommand)
	{
		switch(event->type())
		{
		case QEvent::FocusIn:
			if(has_tip_)
			{
				ui->comboCommand->lineEdit()->clear();
				ui->comboCommand->lineEdit()->setPalette(pal_old_);
			}
			has_tip_ = false;
			break;
		case QEvent::FocusOut:
			if(ui->comboCommand->lineEdit()->text().isEmpty())
			{
				ui->comboCommand->lineEdit()->setPalette(pal_new_);
				ui->comboCommand->lineEdit()->setText(tip_text);
				has_tip_ = true;
			}
			break;
		}
	}
    return false;
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
				QString info = "<b>" + cmd.description() + "</b> -- " + cmd.name();

				QStringList arghelp = cmd.arghelp();
				if(!arghelp.empty())
				{
					QString argstr;
					int i = 0;
					int arg_pos = arg_list.size();
					if(text[text.length()-1].isSpace())
						arg_pos++;
					Q_FOREACH(const QString& arg, arghelp)
					{
						if(i+1 == arg_pos)
							argstr += "<u>" + arg + "</u>";
						else
							argstr += arg;
						i++;
						if(i != arghelp.size())
							argstr += ", ";
					}

					info += " " + argstr;
				}

				ui->labelInfo->setText(info);
			}
			else if(arg_list.empty() && !text[text.length()-1].isSpace())
			{
				// Show list of commands beginning with the entered characters
				QStringList similar = similar_commands(cmd_str);
				if(!similar.empty())
				{
					QString info;
					int i = 0;
					Q_FOREACH(const QString& name, similar)
					{
						Command curcmd (name, QStringList());
						Q_ASSERT(curcmd.isValid());
						info += "<b>" + curcmd.name() + "</b> " + curcmd.description();
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
	if(arguments.empty())
		return false;

	command = arguments.first();
	arguments.removeFirst();
	return true;
}

QStringList CommandBarWidget::similar_commands(const QString& command)
{
	QStringList similar;

	QStringList commands = Command::commands();
	Q_FOREACH(const QString& name, commands)
	{
		if(name.startsWith(command, Qt::CaseInsensitive))
		{
			similar << name;
		}
	}

	return similar;
}
