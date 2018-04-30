/*
 * SPDX-License-Identifier: BSD-3-Clause
 * Copyright (c) 2017 Markus Jylhänkangas
 */

#include "mainwindow.hpp"
#include "ui_mainwindow.h"

/*********************
 * Main stuff
 *********************/

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
  ui->setupUi(this);
  this->setWindowTitle("Lappbill");
  QFileInfo check_file("data.db");
  if (check_file.exists() && check_file.isFile()) {

    QFile databasefile("data.db");
    databasefile.open(QIODevice::ReadOnly);
    QDataStream in(&databasefile);
    in >> databaseInstance;
    databasefile.close();
  } else {
    databaseInstance = DatabaseStruct();
  }

  UpdateLists();
  UpdateInvoiceNumber();

  if (databaseInstance.CompanyInfo.Y_tunnus == "0" &&
      databaseInstance.CompanyInfo.Name == "0") {
    lauchSettings(true);
    if (databaseInstance.CompanyInfo.Y_tunnus == "0" &&
        databaseInstance.CompanyInfo.Name == "0") {
      exit(EXIT_SUCCESS);
    }
  }

  ui->DueDate->setDate(
      QDate::currentDate().addDays(databaseInstance.CompanyInfo.Maksuaika));
}

void MainWindow::lauchSettings(bool first) {
  this->setEnabled(false);
  SettingsDialog dia(first, &databaseInstance, this);

  dia.exec();

  this->setEnabled(true);
}

void MainWindow::closeEvent(QCloseEvent *event) {
  QFile databasefile("data.db");
  databasefile.open(QIODevice::WriteOnly);
  QDataStream out(&databasefile);
  out << databaseInstance;
  databasefile.close();
}

MainWindow::~MainWindow() { delete ui; }

void MainWindow::UpdateLists() {
  ui->CustomerList->clear();
  ui->ProductList->clear();

  if (databaseInstance.CustomerList.count() > 0) {
    ui->CustomerList->addItem(tr("Valitse"), QVariant("Check"));
    foreach (CustomerStruct cust, databaseInstance.CustomerList) {
      ui->CustomerList->addItem(cust.Nimi1, QVariant("Check"));
    }
  } else {
    ui->CustomerList->addItem(tr("Ei yhtään tallenttua"), QVariant("Check"));
  }

  if (databaseInstance.ProductList.count() > 0) {
    ui->ProductList->addItem(tr("Valitse"), QVariant("Check"));
    foreach (ProductStruct prod, databaseInstance.ProductList) {
      ui->ProductList->addItem(prod.NameProd, QVariant("Check"));
    }
  } else {
    ui->ProductList->addItem(tr("Ei yhtään tallennettua"), QVariant("Check"));
  }
}

void MainWindow::UpdateInvoiceNumber() {
  if (databaseInstance.InvoiceNumber >= ui->Invoice_Number->maximum()) {
    databaseInstance.InvoiceNumber = 100;
  }
  ui->Invoice_Number->setValue(databaseInstance.InvoiceNumber);
}

void MainWindow::on_SaveButton_clicked() {

  QList<ProductStruct> prods;
  for (int row = 0; row < ui->OutputProducts->rowCount(); ++row) {

    ProductStruct tempProd = {
        ui->OutputProducts->item(row, 0)->text(),
        ui->OutputProducts->item(row, 2)->text().toFloat(),
        ui->OutputProducts->item(row, 1)->text().toInt()};

    prods.append(tempProd);
  }

  CustomerStruct tmpCus = get_Current_Customer();

  if (!QDir(databaseInstance.CompanyInfo.SaveFolderPath + "/" +
            tmpCus.Nimi1.replace(" ", "_"))
           .exists()) {
    QDir().mkdir(databaseInstance.CompanyInfo.SaveFolderPath + "/" +
                 tmpCus.Nimi1.replace(" ", "_"));
  }

  QString invoiceName = databaseInstance.CompanyInfo.SaveFolderPath + "/" +
                        tmpCus.Nimi1.replace(" ", "_") + "/" +
                        ui->Invoice_Number->text() + "_" +
                        QDate::currentDate().toString("yyyy-M-d");

  if (!tmpCus.Potilas.trimmed().isEmpty()) {
    invoiceName += "_" + tmpCus.Potilas;
  }
  invoiceName += ".pdf";

  // Generate pdf
  invoiceTemplate pdf = invoiceTemplate(invoiceName);

  pdf.Company = databaseInstance.CompanyInfo;
  pdf.Customer = tmpCus;
  pdf.Products = prods;
  pdf.Due_Date = ui->DueDate->text();
  pdf.Date = QDate::currentDate().toString("d.M.yyyy");
  pdf.Invoice_Number = ui->Invoice_Number->text();
  pdf.Reference_Number =
      pdf.GenerateReferenceNumber(ui->Invoice_Number->value());
  pdf.GeneratePDF();

  databaseInstance.InvoiceNumber = ui->Invoice_Number->value() + 1;
  UpdateInvoiceNumber();

  QMessageBox::information(this, tr("Lappbill"),
                           tr("PDF tallennettu sijaintiin:") + '\n' +
                               invoiceName);
}

/******************
 * Product
 ******************/
void MainWindow::on_Remove_Product_clicked() {
  const QString nimi = ui->Product_Name->text();
  const float hinta = (float)ui->Product_Price->value();
  const int kpl = ui->Product_kpl->text().toInt();

  ProductStruct p = {nimi, hinta, kpl};

  int index = 0;
  foreach (ProductStruct prod, databaseInstance.ProductList) {

    if (QString::compare(prod.NameProd, p.NameProd) == 0 &&
        prod.PriceProd == p.PriceProd) {
      databaseInstance.ProductList.removeAt(index);
    }

    ++index;
  }
  UpdateLists();
}

void MainWindow::on_Add_Product_clicked() {
  const QString nimi = ui->Product_Name->text();
  const QString hinta = QString::number(ui->Product_Price->value());
  const QString kpl = QString::number(ui->Product_kpl->text().toInt());

  ui->OutputProducts->insertRow(0);
  ui->OutputProducts->setItem(0, 0, new QTableWidgetItem(nimi));
  ui->OutputProducts->setItem(0, 1, new QTableWidgetItem(kpl));
  ui->OutputProducts->setItem(0, 2, new QTableWidgetItem(hinta));
}

void MainWindow::on_ProductList_activated(const QString &arg1) {
  ProductStruct tmp;
  if (QString::compare(arg1, tr("Valitse")) == 0) {
    tmp = ProductStruct{"", 0.0, 0};
  } else {
    foreach (ProductStruct prod, databaseInstance.ProductList) {
      if (QString::compare(prod.NameProd, arg1) == 0) {
        tmp = prod;
        break;
      }
    }
  }
  ui->Product_kpl->setValue(tmp.KplProd);
  ui->Product_Name->setText(tmp.NameProd);
  ui->Product_Price->setValue(tmp.PriceProd);
}

void MainWindow::on_Save_Product_Button_clicked() {
  const QString nimi = ui->Product_Name->text();
  const float hinta = (float)ui->Product_Price->value();

  ProductStruct p = {nimi, hinta, 1};

  // Tee tästä parempi
  bool found = false;
  foreach (ProductStruct prod, databaseInstance.ProductList) {
    if (QString::compare(prod.NameProd, p.NameProd) == 0 &&
        prod.PriceProd == p.PriceProd) {
      found = true;
    }
  }

  if (!found) {
    databaseInstance.ProductList.append(p);
  }

  UpdateLists();
}

void MainWindow::on_Remove_Product_from_List_clicked() {

  auto reply = QMessageBox::question(this, tr("Poista tuote"),
                                     tr("Haluatko todella poistaa tuotteen?"),
                                     QMessageBox::Yes | QMessageBox::No);
  if (reply == QMessageBox::Yes) {
    QSet<int> selectedRows;
    QList<QTableWidgetItem *> itemList = ui->OutputProducts->selectedItems();
    QTableWidgetItem *item;

    foreach (item, itemList) { selectedRows.insert(item->row()); }
    QList<int> rows = selectedRows.toList();
    qSort(rows.begin(), rows.end());
    foreach (int row, rows) { ui->OutputProducts->removeRow(row); }
  }
  UpdateLists();
}

/****************
 * Customers
 ****************/

void MainWindow::on_CustomerList_activated(const QString &arg1) {
  CustomerStruct tmp;
  if (QString::compare(arg1, tr("Valitse")) == 0) {
    tmp = CustomerStruct{"", "", "", "", "", "", "", ""};
  } else {
    foreach (CustomerStruct cust, databaseInstance.CustomerList) {
      if (QString::compare(cust.Nimi1, arg1) == 0) {
        tmp = cust;
        break;
      }
    }
  }
  ui->Customer_Name1->setText(tmp.Nimi1);
  ui->Customer_Name2->setText(tmp.Nimi2);
  ui->Customer_Puh->setText(tmp.Puh);
  ui->Customer_street1->setText(tmp.Katu1);
  ui->Customer_street2->setText(tmp.Katu2);
  ui->Customer_City->setText(tmp.Kaupunki);
  ui->Customer_postal->setText(tmp.Postinum);
  ui->Potilas->setText(tmp.Potilas);
}

CustomerStruct MainWindow::get_Current_Customer() {
  const QString nimi1 = ui->Customer_Name1->text();
  const QString nimi2 = ui->Customer_Name2->text();
  const QString puh = ui->Customer_Puh->text();
  const QString katu1 = ui->Customer_street1->text();
  const QString katu2 = ui->Customer_street2->text();
  const QString kaupunki = ui->Customer_City->text();
  const QString postinum = ui->Customer_postal->text();
  const QString potilas = ui->Potilas->text();

  CustomerStruct c = {nimi1, nimi2,    puh,      katu1,
                      katu2, kaupunki, postinum, potilas};

  return c;
}

void MainWindow::on_SaveCustomerButton_clicked() {
  databaseInstance.CustomerList.append(get_Current_Customer());

  UpdateLists();
}

void MainWindow::on_RemoveCustomerButton_clicked() {
  CustomerStruct current = get_Current_Customer();

  int index = 0;
  foreach (CustomerStruct cus, databaseInstance.CustomerList) {
    if (QString::compare(cus.Nimi1, current.Nimi1) == 0 &&
        QString::compare(cus.Nimi2, current.Nimi2) == 0 &&
        QString::compare(cus.Puh, current.Puh) == 0 &&
        QString::compare(cus.Postinum, current.Postinum) == 0 &&
        QString::compare(cus.Katu1, current.Katu1) == 0) {

      databaseInstance.CustomerList.removeAt(index);
      break;
    }
    ++index;
  }
  UpdateLists();
}

void MainWindow::on_Settings_Button_clicked() {
  lauchSettings(false);
  ui->DueDate->setDate(
      QDate::currentDate().addDays(databaseInstance.CompanyInfo.Maksuaika));
}
