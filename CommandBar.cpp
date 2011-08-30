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

#include "CommandBar.h"

#include "CommandBarWidget.h"
#include <QMenu>
#include <QMainWindow>
#include <QToolBar>

CommandBar::CommandBar() : menu_(0), commandbar_widget_(0) {
}

CommandBar::~CommandBar() {
}

QMenu *CommandBar::menu(QWidget *parent) {

	if(menu_ == 0) {
		menu_ = new QMenu(tr("CommandBar"), parent);
	}

	// if we are dealing with a main window (and we are...)
	// add the dock object
	if(QMainWindow *const main_window = qobject_cast<QMainWindow *>(parent)) {
		commandbar_widget_ = new CommandBarWidget;

		// make the toolbar widget and _name_ it, it is important to name it so
		// that its state is saved in the GUI info
		QToolBar *const tool_bar = new QToolBar(tr("Command Bar"), parent);
		tool_bar->setAllowedAreas(Qt::TopToolBarArea | Qt::BottomToolBarArea);
		tool_bar->setObjectName(QString::fromUtf8("Command Bar"));
		tool_bar->addWidget(commandbar_widget_);

		// add it to the bottom by default
		main_window->addToolBar(Qt::BottomToolBarArea, tool_bar);

		// make the menu and add the show/hide toggle for the widget
		menu_->addAction(tool_bar->toggleViewAction());
	}

	return menu_;
}

Q_EXPORT_PLUGIN2(CommandBar, CommandBar)
