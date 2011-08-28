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

#ifndef COMMANDBARWIDGET_20110828_H_
#define COMMANDBARWIDGET_20110828_H_

#include <QWidget>

class QString;

namespace Ui { class CommandBar; }

class CommandBarWidget : public QWidget {
	Q_OBJECT
public:
	CommandBarWidget(QWidget *parent = 0, Qt::WindowFlags f = 0);

public Q_SLOTS:
	void textChanged(const QString& text);
	void returnPressed();

private:
	Ui::CommandBar *const ui;
	bool mouse_pressed_;
};

#endif
