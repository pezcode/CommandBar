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

#include "CommandBar.h"
#include "Debugger.h"
#include "CommandBarOptionsPage.h"
#include "CommandBarWidget.h"
#include <QMenu>
#include <QMainWindow>
#include <QDockWidget>
#include <QSettings>

//------------------------------------------------------------------------------
// Name: CommandBar()
// Desc:
//------------------------------------------------------------------------------
CommandBar::CommandBar() : menu_(0), commandbar_widget_(0) {

	QSettings settings;
	if(settings.value("CommandBar/check_on_start.enabled", true).toBool()) {
		do_check();
	}
}

//------------------------------------------------------------------------------
// Name: ~CommandBar()
// Desc:
//------------------------------------------------------------------------------
CommandBar::~CommandBar() {
}

//------------------------------------------------------------------------------
// Name: options_page()
// Desc:
//------------------------------------------------------------------------------
QWidget *CommandBar::options_page() {
	return new CommandBarOptionsPage;
}

//------------------------------------------------------------------------------
// Name: menu(QWidget *parent)
// Desc:
//------------------------------------------------------------------------------
QMenu *CommandBar::menu(QWidget *parent) {

	if(menu_ == 0) {
		menu_ = new QMenu(tr("CommandBar"), parent);
		menu_->addAction(tr("&Check For Latest Version"), this, SLOT(show_menu()));
	}

	// if we are dealing with a main window (and we are...)
	// add the dock object
	if(QMainWindow *const main_window = qobject_cast<QMainWindow *>(parent)) {
		commandbar_widget_ = new CommandBarWidget;

		// make the dock widget and _name_ it, it is important to name it so
		// that it's state is saved in the GUI info
		QDockWidget *const dock_widget = new QDockWidget(tr("Command Bar"), parent);
		dock_widget->setAllowedAreas(Qt::AllDockWidgetAreas);
		dock_widget->setFeatures(QDockWidget::DockWidgetMovable | QDockWidget::DockWidgetFloatable | QDockWidget::DockWidgetClosable);
		dock_widget->setObjectName(QString::fromUtf8("Command Bar"));
		dock_widget->setWidget(commandbar_widget_);

		// add it to the dock
		main_window->addDockWidget(Qt::BottomDockWidgetArea, dock_widget);

		// make the menu and add the show/hide toggle for the widget
		menu_->addAction(dock_widget->toggleViewAction());
	}

	return menu_;
}

//------------------------------------------------------------------------------
// Name: do_check()
// Desc:
//------------------------------------------------------------------------------
void CommandBar::do_check() {


}

//------------------------------------------------------------------------------
// Name: show_menu()
// Desc:
//------------------------------------------------------------------------------
void CommandBar::show_menu() {
	do_check();
}

Q_EXPORT_PLUGIN2(CommandBar, CommandBar)
