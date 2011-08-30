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

#include "AnalyzerInterface.h"
#include "MemoryRegions.h"
#include "Debugger.h"

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

	error_ = "";
	return (this->*function)();
}

void Command::initFunctions()
{
	if(!functions.size())
	{
		functions["AN"] = &Command::cmd_AN;
		functions["BP"] = &Command::cmd_BP;
	}
}

void Command::initDescriptions()
{
	if(!descriptions.size())
	{
		descriptions["AN"] = "Analyze region";
		descriptions["BP"] = "Set breakpoint";
	}
}

void Command::initHelp()
{
	if(!help.size())
	{
		help["AN"] = QStringList() << "[address (def: CPU)]";
		help["BP"] = QStringList() << "address" << "[condition]";
	}
}

bool Command::cmd_AN()
{
	if(arguments_.size() < 0 || arguments_.size() > 1) {
		error_ = "Invalid number of arguments";
		return false;
	}

	AnalyzerInterface* analyzer = edb::v1::analyzer();
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
			error_ = "Couldn't find address: " + saddr;
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
