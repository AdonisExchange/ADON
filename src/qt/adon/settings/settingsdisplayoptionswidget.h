// Copyright (c) 2019-2022 The ADON developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef SETTINGSDISPLAYOPTIONSWIDGET_H
#define SETTINGSDISPLAYOPTIONSWIDGET_H

#include <QWidget>
#include <QDataWidgetMapper>
#include "qt/adon/pwidget.h"

namespace Ui {
class SettingsDisplayOptionsWidget;
}

class SettingsDisplayOptionsWidget : public PWidget
{
    Q_OBJECT

public:
    explicit SettingsDisplayOptionsWidget(ADONGUI* _window = nullptr, QWidget *parent = nullptr);
    ~SettingsDisplayOptionsWidget();

    void setMapper(QDataWidgetMapper *mapper);
    void initLanguages();
    void loadClientModel() override;

public Q_SLOTS:
    void onResetClicked();

Q_SIGNALS:
    void saveSettings();
    void discardSettings();

private:
    Ui::SettingsDisplayOptionsWidget *ui;
};

#endif // SETTINGSDISPLAYOPTIONSWIDGET_H
