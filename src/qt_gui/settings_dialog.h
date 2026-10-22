// SPDX-FileCopyrightText: Copyright 2024 shadPS4 Emulator Project
// SPDX-License-Identifier: GPL-2.0-or-later

#pragma once

#include <array>
#include <memory>
#include <span>
#include <QComboBox>
#include <QDialog>
#include <QGroupBox>
#include <QPushButton>

#include "common/path_util.h"
#include "core/emulator_settings.h"
#include "gui_settings.h"
#include "ipc/ipc_client.h"
#include "qt_gui/compatibility_info.h"

namespace Ui {
class SettingsDialog;
}

class SettingsDialog : public QDialog {
    Q_OBJECT
public:
    explicit SettingsDialog(std::shared_ptr<gui_settings> gui_settings,
                            std::shared_ptr<CompatibilityInfoClass> m_compat_info,
                            std::shared_ptr<IpcClient> ipc_client, QWidget* parent = nullptr,
                            bool is_game_running = false, bool is_game_specific = false,
                            std::string gsc_serial = "");
    ~SettingsDialog();

    bool eventFilter(QObject* obj, QEvent* event) override;
    void updateNoteTextEdit(const QString& groupName);

    int exec() override;

signals:
    void LanguageChanged(const QString& locale);
    void CompatibilityChanged();
    void BackgroundOpacityChanged(int opacity);

private:
    void LoadValuesFromConfig();
    void UpdateSettings(bool is_specific = false);
    void SyncRealTimeWidgetstoConfig();
    void InitializeEmulatorLanguages();
    void OnLanguageChanged(int index);
    void OnCursorStateChanged(s16 index);
    void SetDefaultValues();
    void VolumeSliderChange(int value);
    void PollSDLevents();
    void GetPhysicalDevices();
    void SaveSettings();
    void RefreshAudioDevices();
    void CreatePlayerWidgets();
    void UpdateInstrumentDevices(size_t player);
    void SelectItem(QComboBox* box, const QVariant& data);

    void closeEvent(QCloseEvent* event) override;

    struct PlayerWidgets {
        QComboBox* controller;
        QComboBox* instrument;
        QComboBox* instrument_device;
    };
    std::array<PlayerWidgets, 4> player_widgets{};
    std::array<QComboBox*, 3> mic_boxes{};
    QList<QPair<QString, QString>> guitar_devices;
    QList<QPair<QString, QString>> drum_devices;

    std::map<std::string, int> languages;
    QString defaultTextEdit;
    int initialHeight;
    std::string gs_serial;
    QFuture<void> Polling;

    bool is_game_running = false;
    bool is_game_specific = false;
    bool is_game_saving = false;

    std::unique_ptr<Ui::SettingsDialog> ui;
    std::shared_ptr<gui_settings> m_gui_settings;
    std::shared_ptr<IpcClient> m_ipc_client;
};
