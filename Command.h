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

#ifndef COMMAND_20110828_H_
#define COMMAND_20110828_H_

#include <QString>
#include <QStringList>
#include <QMap>

class Command
{
public:
	Command(const QString& command, const QStringList& arguments);

	bool isValid() const { return valid_; }
	bool execute();

	QString name() const { return names[command_]; }
	QStringList arghelp() const { return help[command_]; }

private:
	void initFunctions();
	void initNames();
	void initArguments();

	static bool cmd_AN(const QStringList&);

private:
	typedef bool (*fCommand)(const QStringList&);

	static QMap<QString, fCommand> functions;
	static QMap<QString, QString> names;
	static QMap<QString, QStringList> help;

	QString command_;
	QStringList arguments_;
	bool valid_;
};

#endif
