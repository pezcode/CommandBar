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

#include "Command.h"

#include "Debugger.h"
#include "MemoryRegions.h"
#include "Configuration.h"
#include "AnalyzerInterface.h"
#include "DebuggerCoreInterface.h"

#include <QDebug>

QMap<QString, Command::fCommand> Command::functions;
QMap<QString, QString> Command::descriptions;
QMap<QString, QStringList> Command::help;

const Command Command::dummy = Command(QString(), QStringList());

Command::Command(const QString& command, const QStringList& arguments) : valid_(false)
{
	initFunctions();
	initDescriptions();
	initHelp();

	Q_ASSERT(functions.size() == names.size());

	command_ = command.toUpper();

	if(functions.contains(command_))
	{
		// check argument length?
		arguments_ = arguments;
		valid_ = true;
	}
}

bool Command::execute()
{
	if(!valid_)
		return false;

	fCommand function = functions[command_];
	Q_ASSERT(function != NULL);

	error_ = "";
	return (this->*function)();
}

void Command::initFunctions()
{
	if(!functions.size())
	{
		functions["AN"] = &Command::cmd_AN;

		functions["BP"] = &Command::cmd_BP;

		functions["GOTO"] = &Command::cmd_GOTO;
		functions["PAUSE"] = &Command::cmd_PAUSE;
		functions["RUN"] = &Command::cmd_RUN;
		functions["STEP"] = &Command::cmd_STEP;
		functions["STEPO"] = &Command::cmd_STEPO;

		functions["REG"] = &Command::cmd_REG;
		functions["PUSH"] = &Command::cmd_PUSH;
		functions["POP"] = &Command::cmd_POP;

		functions["PROT"] = &Command::cmd_PROT;
		functions["FIND"] = &Command::cmd_FIND;
		functions["COPY"] = &Command::cmd_COPY;
		functions["FILL"] = &Command::cmd_FILL;
		functions["DUMP"] = &Command::cmd_DUMP;
		functions["LOAD"] = &Command::cmd_LOAD;

		functions["KILL"] = &Command::cmd_KILL;
		functions["DETACH"] = &Command::cmd_DETACH;
		functions["ATTACH"] = &Command::cmd_ATTACH;
		functions["OPEN"] = &Command::cmd_OPEN;
		functions["RESTART"] = &Command::cmd_RESTART;

		functions["QUIT"] = &Command::cmd_QUIT;
	}
}

void Command::initDescriptions()
{
	if(!descriptions.size())
	{
		descriptions["AN"] = "Analyze region";

		descriptions["BP"] = "Set breakpoint";

		descriptions["GOTO"]  = "Go to address in CPU view";
		descriptions["PAUSE"] = "Pause the process";
		descriptions["RUN"]   = "Run (until)";
		descriptions["STEP"]  = "Step instruction";
		descriptions["STEPO"] = "Step over instruction";

		descriptions["REG"]  = "Get/set register";
		descriptions["PUSH"] = "Push value to stack";
		descriptions["POP"]  = "Pop value from stack";

		descriptions["PROT"] = "Get/set memory protection";
		descriptions["FIND"] = "Find pattern in memory";
		descriptions["COPY"] = "Copy memory";
		descriptions["FILL"] = "Fill memory";
		descriptions["DUMP"] = "Write memory to disk";
		descriptions["LOAD"] = "Write to memory from file";

		descriptions["KILL"]    = "Kill debuggee";
		descriptions["DETACH"]  = "Detach from debuggee";
		descriptions["ATTACH"]  = "Attach to process";
		descriptions["OPEN"]    = "Start process";
		descriptions["RESTART"] = "Restart debuggee";

		descriptions["QUIT"] = "Quit EDB";
	}
}

void Command::initHelp()
{
	if(!help.size())
	{
		help["AN"] = QStringList() << "[address (def: CPU)]";

		help["BP"] = QStringList() << "address" << "[condition]";

		help["PROT"] = QStringList() << "address" << "[new protection: {r/-}{w/-}{x/-}]";
		help["FIND"] = QStringList() << "address" << "pattern" << "[size (def: until end of block)]";
		help["COPY"] = QStringList() << "destination" << "source" << "size";
		help["FILL"] = QStringList() << "address" << "value" << "times";
		help["DUMP"] = QStringList() << "address" << "size" << "file";
		help["LOAD"] = QStringList() << "address" << "file" << "[max size (def: file size)]";

		//help["KILL"] = QStringList();
		//help["DETACH"] = QStringList();
		help["ATTACH"] = QStringList() << "process-id";
		help["OPEN"] = QStringList() << "path";
		//help["RESTART"] = QStringList();

		//help["QUIT"] = QStringList();
	}
}

bool Command::cmd_AN()
{
	if(arguments_.size() < 0 || arguments_.size() > 1) {
		error_ = "Invalid number of arguments";
		return false;
	}

	AnalyzerInterface* analyzer = edb::v1::analyzer();
	if(!analyzer)
	{
		error_ = "No analyzer found";
		return false;
	}

	MemRegion region;
	if(arguments_.size() > 0)
	{
		const QString saddr = arguments_.first();
		edb::address_t address = 0;
		if(!edb::v1::eval_expression(saddr, address))
		{
			error_ = "Couldn't evaluate address: " + saddr;
			return false;
		}
		const MemoryRegions& regions = edb::v1::memory_regions();
		if(!regions.find_region(address, region))
		{
			error_ = "Couldn't find address: " + edb::v1::format_pointer(address);
			return false;
		}
	}
	else
	{
		region = edb::v1::current_cpu_view_region();
	}

	analyzer->analyze(region);
	return true;
}

bool Command::cmd_BP()
{
	if(arguments_.size() < 1 || arguments_.size() > 2) {
		error_ = "Invalid number of arguments";
		return false;
	}

	QStringList::const_iterator arg_it = arguments_.begin();

	const QString saddr = *arg_it++;
	edb::address_t address = 0;
	if(!edb::v1::eval_expression(saddr, address))
	{
		error_ = "Couldn't evaluate address: " + saddr;
		return false;
	}

	edb::v1::create_breakpoint(address);

	if(arguments_.size() > 1)
	{
		const QString condition = *arg_it;
		edb::v1::set_breakpoint_condition(address, condition);
	}

	// todo: check if BP was actually created

	return true;
}

bool Command::cmd_GOTO()
{
	error_ = "Not yet implemented";
	return false;
}

bool Command::cmd_PAUSE()
{
	error_ = "Not yet implemented";
	return false;
}

bool Command::cmd_RUN()
{
	error_ = "Not yet implemented";
	return false;
}

bool Command::cmd_STEP()
{
	error_ = "Not yet implemented";
	return false;
}

bool Command::cmd_STEPO()
{
	error_ = "Not yet implemented";
	return false;
}

bool Command::cmd_REG()
{
	error_ = "Not yet implemented";
	return false;
}

bool Command::cmd_PUSH()
{
	error_ = "Not yet implemented";
	return false;
}

bool Command::cmd_POP()
{
	error_ = "Not yet implemented";
	return false;
}

bool Command::cmd_PROT()
{
	error_ = "Not yet implemented";
	return false;
}

bool Command::cmd_FIND()
{
	error_ = "Not yet implemented";
	return false;
}

bool Command::cmd_COPY()
{
	error_ = "Not yet implemented";
	return false;
}

bool Command::cmd_FILL()
{
	error_ = "Not yet implemented";
	return false;
}

bool Command::cmd_DUMP()
{
	error_ = "Not yet implemented";
	return false;
}

bool Command::cmd_LOAD()
{
	error_ = "Not yet implemented";
	return false;
}

bool Command::cmd_KILL()
{
	if(arguments_.size() != 0) {
		error_ = "Invalid number of arguments";
		return false;
	}

	DebuggerCoreInterface* debugger = edb::v1::debugger_core;
	if(!debugger)
	{
		error_ = "No debugger core found";
		return false;
	}

	if(!debugger->pid())
	{
		error_ = "No process running";
		return false;
	}

	debugger->kill();
	return true;
}

bool Command::cmd_DETACH()
{
	if(arguments_.size() != 0) {
		error_ = "Invalid number of arguments";
		return false;
	}

	DebuggerCoreInterface* debugger = edb::v1::debugger_core;
	if(!debugger)
	{
		error_ = "No debugger core found";
		return false;
	}

	if(!debugger->pid())
	{
		error_ = "No process running";
		return false;
	}

	debugger->detach();
	return true;
}

bool Command::cmd_ATTACH()
{
	if(arguments_.size() != 1) {
		error_ = "Invalid number of arguments";
		return false;
	}

	DebuggerCoreInterface* debugger = edb::v1::debugger_core;
	if(!debugger)
	{
		error_ = "No debugger core found";
		return false;
	}

	const QString spid = arguments_.first();
	edb::address_t value;
	if(!edb::v1::eval_expression(spid, value))
	{
		error_ = "Couldn't evaluate input: " + spid;
		return false;
	}

	if(!debugger->attach(value))
	{
		error_ = "Attaching failed";
		return false;
	}
	return true;
}

bool Command::cmd_OPEN()
{
	error_ = "Not yet implemented";
	return false;

	if(arguments_.size() != 1) {
		error_ = "Invalid number of arguments";
		return false;
	}

	DebuggerCoreInterface* debugger = edb::v1::debugger_core;
	if(!debugger)
	{
		error_ = "No debugger core found";
		return false;
	}

	if(!debugger->pid())
	{
		error_ = "No process running";
		return false;
	}

	debugger->detach();
	return true;
}

bool Command::cmd_RESTART()
{
	if(arguments_.size() != 0) {
		error_ = "Invalid number of arguments";
		return false;
	}

	DebuggerCoreInterface* debugger = edb::v1::debugger_core;
	if(!debugger)
	{
		error_ = "No debugger core found";
		return false;
	}

	if(!debugger->pid())
	{
		error_ = "No process running";
		return false;
	}

	const QString path = edb::v1::get_process_exe();
	const QStringList args = edb::v1::get_process_args();
	const QString cwd  = edb::v1::get_process_cwd();
	QString tty;
	if(edb::v1::config().tty_enabled)
		tty = edb::v1::config().tty_command;

	debugger->kill();
	if(!debugger->open(path, cwd, args, tty))
	{
		error_ = "Process creating failed";
		return false;
	}
	return true;
}

bool Command::cmd_QUIT()
{
	error_ = "Not yet implemented";
	return false;
}
