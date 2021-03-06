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

#ifndef COMMANDBAR_20110828_H_
#define COMMANDBAR_20110828_H_

#include "DebuggerPluginInterface.h"

class QMenu;
class CommandBarWidget;

class CommandBar : public QObject, public DebuggerPluginInterface {
	Q_OBJECT
	Q_INTERFACES(DebuggerPluginInterface)
	Q_CLASSINFO("author", "RVRS")
	Q_CLASSINFO("url", "http://rvrs.in")

public:
	CommandBar();
	virtual ~CommandBar();

public:
	virtual QMenu *menu(QWidget *parent = 0);

private:
	QMenu* menu_;
	CommandBarWidget* commandbar_widget_;
};

#endif
