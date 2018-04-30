/*
 * SPDX-License-Identifier: BSD-3-Clause
 * Copyright (c) 2017 Markus Jylhänkangas
 */

#include "settingsdialog.hpp"
#include "ui_settingsdialog.h"

SettingsDialog::SettingsDialog(bool first, DatabaseStruct *db, QWidget *parent)
    : QDialog(parent), databaseInstance(db), ui(new Ui::SettingsDialog) {
  ui->setupUi(this);
  this->setWindowTitle(tr("Asetukset"));
  if (!first) {
    ui->Address->setText(databaseInstance->CompanyInfo.Address);
    ui->City->setText(databaseInstance->CompanyInfo.City);
    ui->iban->setText(databaseInstance->CompanyInfo.Iban);
    ui->Name->setText(databaseInstance->CompanyInfo.Name);
    ui->Phone->setText(databaseInstance->CompanyInfo.Phonenumber);
    ui->Postalnum->setText(databaseInstance->CompanyInfo.Postal);
    ui->ytunnus->setText(databaseInstance->CompanyInfo.Y_tunnus);
    ui->Maksuaika->setValue(databaseInstance->CompanyInfo.Maksuaika);
    ui->field1->setText(databaseInstance->CompanyInfo.Field1);
    ui->field2->setText(databaseInstance->CompanyInfo.Field2);
    ui->alvText->setText(databaseInstance->CompanyInfo.AlvText);


    if (!databaseInstance->CompanyInfo.SaveFolderPath.trimmed().isEmpty()) {
      selectedDir = databaseInstance->CompanyInfo.SaveFolderPath;
    }
  }
}

SettingsDialog::~SettingsDialog() { delete ui; }

void SettingsDialog::on_chooseFolder_button_clicked() {
  selectedDir = QFileDialog::getExistingDirectory(
      this, tr("Valitse kansio"), selectedDir,
      QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);

  if (!selectedDir.trimmed().isEmpty()) {
    ui->chooseFolder_button->setStyleSheet(
        "QPushButton { background-color: rgba(54, 200, 88, 0.39);  }");
  }
}

void SettingsDialog::on_buttonBox_clicked(QAbstractButton *button) {

  if (ui->buttonBox->buttonRole(button) ==
          QDialogButtonBox::ButtonRole::AcceptRole &&
      !selectedDir.trimmed().isEmpty()) {

    databaseInstance->CompanyInfo.Address = ui->Address->text();
    databaseInstance->CompanyInfo.City = ui->City->text();
    databaseInstance->CompanyInfo.Iban = ui->iban->text();
    databaseInstance->CompanyInfo.Name = ui->Name->text();
    databaseInstance->CompanyInfo.Phonenumber = ui->Phone->text();
    databaseInstance->CompanyInfo.Postal = ui->Postalnum->text();
    databaseInstance->CompanyInfo.Y_tunnus = ui->ytunnus->text();
    databaseInstance->CompanyInfo.Maksuaika = ui->Maksuaika->value();
    databaseInstance->CompanyInfo.Field1 = ui->field1->text();
    databaseInstance->CompanyInfo.Field2 = ui->field2->text();
    databaseInstance->CompanyInfo.AlvText = ui->alvText->text();

    databaseInstance->CompanyInfo.SaveFolderPath = selectedDir;
    accept();
  } else {
    ui->chooseFolder_button->setStyleSheet(
        "QPushButton { background-color: rgba(216, 99, 101, 0.39);  }");
  }
}

void SettingsDialog::on_buttonBox_rejected() { this->close(); }

void SettingsDialog::checkLineEdit(QLineEdit *line) {

  if (line->text().size() > 0) {
    line->setStyleSheet("QLineEdit { background: rgba(54, 200, 88, 0.39);  }");
  } else {
    line->setStyleSheet("QLineEdit { background: rgba(216, 99, 101, 0.39);  }");
  }
}

void SettingsDialog::on_Name_editingFinished() { checkLineEdit(ui->Name); }

void SettingsDialog::on_Address_editingFinished() {
  checkLineEdit(ui->Address);
}

void SettingsDialog::on_Postalnum_editingFinished() {
  checkLineEdit(ui->Postalnum);
}

void SettingsDialog::on_City_editingFinished() { checkLineEdit(ui->City); }

void SettingsDialog::on_Phone_editingFinished() { checkLineEdit(ui->Phone); }

void SettingsDialog::on_iban_editingFinished() { checkLineEdit(ui->iban); }

void SettingsDialog::on_ytunnus_editingFinished() {
  checkLineEdit(ui->ytunnus);
}
