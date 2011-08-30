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

#ifndef COMMAND_20110828_H_
#define COMMAND_20110828_H_

#include <QString>
#include <QStringList>
#include <QList>
#include <QMap>

class Command
{
public:
	Command(const QString& command, const QStringList& arguments);

	bool isValid() const { return valid_; }
	bool execute();

	QString error() const { return error_; }

	QString name() const { return command_; }
	QString description() const { return descriptions[command_]; }
	QStringList arghelp() const { return help[command_]; }

	static QStringList commands() { return functions.keys(); }

private:
	void initFunctions();
	void initDescriptions();
	void initHelp();

	/*
	 * TODO:
	 * 
	 * BP+ (BPX?) instead of BP
	 * BP- (BPR?)(* for all)
	 * BPD disable
	 * BP(toggle)
	 * HWBP ...
	 * RUN, STEP(STEPI), STEPO, BREAK
	 * KILL
	 * DETACH
	 * ATTACH pid
	 * OPEN path
	 * RESTART (target)
	 * QUIT (edb)
	 **/
	bool cmd_AN();
	bool cmd_BP();

private:
	typedef bool (Command::*fCommand)();

	// make sure static maps are initialized
	static const Command dummy;

	static QMap<QString, fCommand> functions;
	static QMap<QString, QString> descriptions;
	static QMap<QString, QStringList> help;

	QString command_;
	QStringList arguments_;
	bool valid_;
	QString error_;
};

#endif
