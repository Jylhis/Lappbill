/*
 * SPDX-License-Identifier: BSD-3-Clause
 * Copyright (c) 2017 Markus Jylhänkangas
 */

#ifndef DATABASE_HPP
#define DATABASE_HPP

#include <QDataStream>
#include <QString>

// TODO: Lisää tietorakenteille vertailu overload
struct ProductStruct {
  QString NameProd;
  float PriceProd;
  int KplProd;
};

QDataStream &operator>>(QDataStream &in, ProductStruct &product);
QDataStream &operator<<(QDataStream &out, const ProductStruct &product);

struct CustomerStruct {
  QString Nimi1;
  QString Nimi2;
  QString Puh;
  QString Katu1;
  QString Katu2;
  QString Kaupunki;
  QString Postinum;
  QString Potilas;
};
QDataStream &operator>>(QDataStream &in, CustomerStruct &customer);
QDataStream &operator<<(QDataStream &out, const CustomerStruct &customer);

struct CompanyStruct {
  QString Y_tunnus = "0";
  QString Name = "0";
  QString Address;
  QString Postal;
  QString City;
  QString Phonenumber;
  QString Iban;
  QString SaveFolderPath;
  QString Field2;
  QString Field1;
  QString AlvText;
  int Maksuaika;
};
QDataStream &operator>>(QDataStream &in, CompanyStruct &company);
QDataStream &operator<<(QDataStream &out, const CompanyStruct &company);

struct DatabaseStruct {
  QList<ProductStruct> ProductList;
  QList<CustomerStruct> CustomerList;
  CompanyStruct CompanyInfo;
  quint64 InvoiceNumber = 100;
};
QDataStream &operator>>(QDataStream &in, DatabaseStruct &data);
QDataStream &operator<<(QDataStream &out, const DatabaseStruct &data);

#endif // DATABASE_HPP
