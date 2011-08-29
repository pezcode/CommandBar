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

#include "CommandBarOptionsPage.h"
#include <QSettings>

#include "ui_commandbar_options_page.h"

CommandBarOptionsPage::CommandBarOptionsPage(QWidget *parent) : QWidget(parent), ui(new Ui::CommandBarOptionsPage) {
	ui->setupUi(this);
}

CommandBarOptionsPage::~CommandBarOptionsPage() {
	delete ui;
}

void CommandBarOptionsPage::showEvent(QShowEvent *event) {
	Q_UNUSED(event);

	QSettings settings;
	ui->checkBox->setChecked(settings.value("CommandBar/check_on_start.enabled", true).toBool());
}

void CommandBarOptionsPage::on_checkBox_toggled(bool checked) {
	Q_UNUSED(checked);

	QSettings settings;
	settings.setValue("CommandBar/check_on_start.enabled", ui->checkBox->isChecked());
}
