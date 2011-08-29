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

#include "Command.h"

#include "AnalyzerInterface.h"
#include "MemoryRegions.h"
#include "Debugger.h"
//#include "DebuggerCoreInterface.h"
//#include "State.h"

#include <QDebug>

QMap<QString, Command::fCommand> Command::functions;
QMap<QString, QString> Command::names;
QMap<QString, QStringList> Command::help;

Command::Command(const QString& command, const QStringList& arguments) : valid_(false)
{
	if(!functions.size())
		initFunctions();
	if(!names.size())
		initNames();
	if(!help.size())
		initArguments();

	//Q_ASSERT(functions.size() == names.size() && names.size() == helps.size());

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

	return function(arguments_);
}

void Command::initFunctions()
{
	if(!functions.size())
	{
		functions["AN"] = &cmd_AN;
	}
}

void Command::initNames()
{
	if(!names.size())
	{
		names["AN"] = "Analyze region";
	}
}

void Command::initArguments()
{
	if(!help.size())
	{
		help["AN"] = QStringList() << "[address (def: CPU)]";
	}
}

bool Command::cmd_AN(const QStringList& arguments)
{
	if(arguments.size() < 0 || arguments.size() > 1) {
		return false;
	}

	AnalyzerInterface* analyzer = edb::v1::analyzer();
	MemRegion region;
	if(arguments.size() > 0)
	{
		const QString saddr = arguments.first();
		edb::address_t address = 0;
		if(!edb::v1::eval_expression(saddr, address))
		{
			//error message
			return false;
		}
		const MemoryRegions& regions = edb::v1::memory_regions();
		if(!regions.find_region(address, region))
		{
			//error message
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
