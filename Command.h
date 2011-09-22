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

#include "Debugger.h"

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

	bool argCheck(int min, int max);
	DebuggerCoreInterface* getDebuggerCoreChecked(bool checkRunning = true);

	/*
	 * TODO:
	 * 
	 * BP+ (BPX?) instead of BP
	 * BP- (BPR?)(* for all)
	 * BPD disable
	 * BP(toggle)
	 * HWBP ...
	 * 
	 * RUN [address]
	 * STEP(STEPI) [times]
	 * STEPO [times]
	 * BREAK
	 * 
	 * KILL
	 * DETACH
	 * ATTACH pid
	 * OPEN path
	 * RESTART (target)
	 * 
	 * QUIT (edb)
	 * 
	 * G (go to)
	 * 
	 * REG register [, value]
	 * PUSH val
	 * POP
	 * 
	 * PROT address [, protection : rwx/---]
	 * 
	 * DUMP address, file
	 * LOAD address, file
	 * COPY dest, src, size
	 * FILL dest, data, size
	 * FIND mem, data [, size]
	 * //ALLOC
	 **/


	bool cmd_AN();

	bool cmd_BP();

	bool cmd_GOTO();
	bool cmd_PAUSE();
	bool cmd_RUN();
	bool cmd_STEP();
	bool cmd_STEPO();

	bool cmd_REG();
	bool cmd_PUSH();
	bool cmd_POP();

	bool cmd_PROT();
	bool cmd_FIND();
	bool cmd_COPY();
	bool cmd_FILL();
	bool cmd_DUMP();
	bool cmd_LOAD();

	bool cmd_KILL();
	bool cmd_DETACH();
	bool cmd_ATTACH();
	bool cmd_OPEN();
	bool cmd_RESTART();

	bool cmd_QUIT();

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
