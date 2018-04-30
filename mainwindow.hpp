/*
 * SPDX-License-Identifier: BSD-3-Clause
 * Copyright (c) 2017 Markus Jylhänkangas
 */

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "database.hpp"
#include "invoicetemplate.hpp"
#include "settingsdialog.hpp"
#include <QFile>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMessageBox>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow {
  Q_OBJECT

public:
  explicit MainWindow(QWidget *parent = 0);
  ~MainWindow();

private slots:

  void on_Remove_Product_clicked();

  void on_Add_Product_clicked();

  void on_SaveButton_clicked();

  void on_CustomerList_activated(const QString &arg1);

  void on_ProductList_activated(const QString &arg1);

  void on_SaveCustomerButton_clicked();

  void on_RemoveCustomerButton_clicked();

  void on_Save_Product_Button_clicked();

  void on_Remove_Product_from_List_clicked();

  void on_Settings_Button_clicked();

private:
  Ui::MainWindow *ui;
  DatabaseStruct databaseInstance;

  CustomerStruct get_Current_Customer();

  void lauchSettings(bool first);
  void UpdateLists();
  void UpdateInvoiceNumber();

protected:
  void closeEvent(QCloseEvent *event);
};

#endif // MAINWINDOW_H
