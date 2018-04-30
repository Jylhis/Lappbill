/*
 * SPDX-License-Identifier: BSD-3-Clause
 * Copyright (c) 2017 Markus Jylhänkangas
 */

#include "database.hpp"

QDataStream &operator>>(QDataStream &in, ProductStruct &product) {
  in >> product.NameProd;
  in >> product.PriceProd;
  in >> product.KplProd;
  return in;
}

QDataStream &operator<<(QDataStream &out, const ProductStruct &product) {
  out << product.NameProd;
  out << product.PriceProd;
  out << product.KplProd;
  return out;
}

QDataStream &operator>>(QDataStream &in, CustomerStruct &customer) {
  in >> customer.Nimi1;
  in >> customer.Nimi2;
  in >> customer.Puh;
  in >> customer.Katu1;
  in >> customer.Katu2;
  in >> customer.Kaupunki;
  in >> customer.Postinum;
  in >> customer.Potilas;
  return in;
}

QDataStream &operator<<(QDataStream &out, const CustomerStruct &customer) {
  out << customer.Nimi1;
  out << customer.Nimi2;
  out << customer.Puh;
  out << customer.Katu1;
  out << customer.Katu2;
  out << customer.Kaupunki;
  out << customer.Postinum;
  out << customer.Potilas;
  return out;
}

QDataStream &operator>>(QDataStream &in, CompanyStruct &company) {
  in >> company.Address;
  in >> company.City;
  in >> company.Iban;
  in >> company.Name;
  in >> company.Phonenumber;
  in >> company.Postal;
  in >> company.Y_tunnus;
  in >> company.SaveFolderPath;
  in >> company.Maksuaika;
  in >> company.Field1;
  in >> company.Field2;
  in >> company.AlvText;
  return in;
}

QDataStream &operator<<(QDataStream &out, const CompanyStruct &company) {
  out << company.Address;
  out << company.City;
  out << company.Iban;
  out << company.Name;
  out << company.Phonenumber;
  out << company.Postal;
  out << company.Y_tunnus;
  out << company.SaveFolderPath;
  out << company.Maksuaika;
  out << company.Field1;
  out << company.Field2;
  out << company.AlvText;
  return out;
}

QDataStream &operator>>(QDataStream &in, DatabaseStruct &data) {
  in >> data.ProductList;
  in >> data.CustomerList;
  in >> data.CompanyInfo;
  in >> data.InvoiceNumber;
  return in;
}

QDataStream &operator<<(QDataStream &out, const DatabaseStruct &data) {
  out << data.ProductList;
  out << data.CustomerList;
  out << data.CompanyInfo;
  out << data.InvoiceNumber;
  return out;
}
