/*
 * SPDX-License-Identifier: BSD-3-Clause
 * Copyright (c) 2017 Markus Jylhänkangas
 */

#ifndef SETTINGSDIALOG_H
#define SETTINGSDIALOG_H

#include "database.hpp"
#include <QtWidgets/QAbstractButton>
#include <QtWidgets/QFileDialog>
#include <QtWidgets/QLineEdit>

namespace Ui {
class SettingsDialog;
}

class SettingsDialog : public QDialog {
  Q_OBJECT

public:
  explicit SettingsDialog(bool first, DatabaseStruct *db, QWidget *parent = 0);
  ~SettingsDialog();

  DatabaseStruct *databaseInstance;

private slots:
  void on_chooseFolder_button_clicked();

  void on_buttonBox_clicked(QAbstractButton *button);

  void on_buttonBox_rejected();

  void on_Name_editingFinished();

  void on_Address_editingFinished();

  void on_Postalnum_editingFinished();

  void on_City_editingFinished();

  void on_Phone_editingFinished();

  void on_iban_editingFinished();

  void on_ytunnus_editingFinished();

private:
  Ui::SettingsDialog *ui;

  QString selectedDir = "";

  void checkLineEdit(QLineEdit *line);
};

#endif // SETTINGSDIALOG_H
