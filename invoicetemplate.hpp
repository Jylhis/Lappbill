/*
 * SPDX-License-Identifier: BSD-3-Clause
 * Copyright (c) 2017 Markus Jylhänkangas
 */

#ifndef INVOICETEMPLATE_H
#define INVOICETEMPLATE_H

#include "database.hpp"
#include <QPainter>
#include <QPdfWriter>
#include <QStaticText>
#include <deque>

class invoiceTemplate {
public:
  QString Date;
  QString Delivery_Date;
  QString Invoice_Number;
  QString Reference_Number;
  QString Due_Date;

  CompanyStruct Company;

  CustomerStruct Customer;

  QList<ProductStruct> Products;

  invoiceTemplate(QString location) : m_location(location) {}

  void GeneratePDF() {
    QPdfWriter writer(m_location);
    QPainter painter(&writer);
    writer.setPageSize(QPagedPaintDevice::A4);

    // One pixel in mm
    // 1 inch = 25.4 mm
    // px = 25.4 mm / 96 dpi
    m_onepixel = 25.4 / writer.resolution();

    QPen pen;
    pen.setColor(Qt::black);
    pen.setWidthF(0.5 / m_onepixel);
    pen.setStyle(Qt::PenStyle::SolidLine);
    painter.setPen(pen);

    topPart(&painter);
    itemList(&painter);
    alvAndYtunnus(&painter);
    bottomPart(&painter);
    fillBottom(&painter);

    QStringList prices = QString::number(totalPrice).split(".");
        QString barcodeEuro = prices.first().rightJustified(6, '0');
        QString barcodeCent = prices.last().rightJustified(2, '0');
        QString barcodeIBAN = Company.Iban.trimmed().remove(0, 2).replace(" ", "");
        QString barcodeREF = Reference_Number.rightJustified(20, '0');
        QString barcodeDueDate = Due_Date;//.toString("yyMMdd");

        // FIXME: nollat tulostuu 16
         printBarcode('4' + barcodeIBAN + barcodeEuro + barcodeCent + '0' + '0' +
                         '0' + barcodeREF + barcodeDueDate,
                    &painter);
  }

  QString GenerateReferenceNumber(quint64 invoiceNumber) {
    // TODO: Siisti tämä
    quint64 orig = invoiceNumber;
    std::deque<int> digits;
    while (invoiceNumber > 0) {
      digits.push_front(invoiceNumber % 10);
      invoiceNumber = invoiceNumber / 10;
    }

    quint64 output = 0;
    int kerroin = 0;
    const int kertoimet[3] = {7, 3, 1};
    while (digits.size() > 0) {
      output += digits.back() * kertoimet[kerroin];
      digits.pop_back();
      if (kerroin < 2) {
        kerroin++;
      } else {
        kerroin = 0;
      }
    }

    int checkNumber = roundUp(output, 10) - output;
    if (checkNumber == 10) {
      checkNumber = 0;
    }

    return QString::number(orig) + QString::number(checkNumber);
  }

private:
  const QString m_location;

  float m_onepixel;

  static const int bottomPadding = 900;
  static constexpr int BOTTOM_EDGE = 13860 - bottomPadding;
  static constexpr int RIGHT_EDGE = 9740 - 100;
  static constexpr int MIDDLE = RIGHT_EDGE / 2;
  const int lineHeight = 400;

  double totalPrice = 0;

  int roundUp(int numToRound, int multiple) {
    if (multiple == 0)
      return numToRound;

    int remainder = numToRound % multiple;
    if (remainder == 0)
      return numToRound;

    return numToRound + multiple - remainder;
  }

  inline void topPart(QPainter *painter) {
    painter->setFont(QFont("Helvetica", 13, QFont::Bold));

    painter->drawStaticText(1000, 300, QStaticText(Company.Name));
    painter->drawStaticText(RIGHT_EDGE - 1800, 300, QStaticText("Lasku"));

    painter->setFont(QFont("Helvetica", 9));

    painter->drawStaticText(1000, 300 + lineHeight * 2,
                            QStaticText(Company.Address));
    painter->drawStaticText(1000, 300 + lineHeight * 2.5,
                            QStaticText(Company.Postal + " " + Company.City));
    painter->drawStaticText(1000, 300 + lineHeight * 3,
                            QStaticText("puh. " + Company.Phonenumber));

    painter->drawLine(600, 2000, RIGHT_EDGE - 600, 2000);

    painter->drawStaticText(MIDDLE + 500, 300 + lineHeight * 4.5,
                            QStaticText("Päiväys: " + Date));

    painter->drawStaticText(
        MIDDLE + 500, 300 + lineHeight * 5,
        QStaticText("Maksuehto: " + QString::number(Company.Maksuaika) +
                    " pv netto"));
    painter->drawStaticText(MIDDLE + 500, 300 + lineHeight * 5.5,
                            QStaticText("Viiv.korko: 11%"));

    if(!Customer.Potilas.trimmed().isEmpty()){
        painter->drawStaticText(MIDDLE + 500, 300 + lineHeight * 6.5,
                            QStaticText(Company.Field2+": " + Customer.Potilas));
    }

    if(!Customer.Nimi2.trimmed().isEmpty()){
    painter->drawStaticText(MIDDLE + 500, 300 + lineHeight * 6,
                        QStaticText(Company.Field1+": " + Customer.Nimi2));
    }

    painter->drawLine(600, 300 + lineHeight * 7.5, RIGHT_EDGE - 600,
                      300 + lineHeight * 7.5);
  }

  inline void bottomPart(QPainter *painter) {
    // VAAKA VIIVAT

    // allekirjoitus

    QPen pen;
    pen.setWidthF(0.13 / m_onepixel);
    pen.setStyle(Qt::PenStyle::SolidLine);
    painter->setPen(pen);
    painter->drawLine(850, BOTTOM_EDGE - lineHeight * 1.5, MIDDLE,
                      BOTTOM_EDGE - lineHeight * 1.5);

    // Ylin viiva
    pen.setColor(Qt::gray);
    pen.setWidthF(0.13 / m_onepixel);
    pen.setStyle(Qt::PenStyle::DashLine);
    painter->setPen(pen);

    painter->drawLine(0, BOTTOM_EDGE - lineHeight * 9.5, RIGHT_EDGE,
                      BOTTOM_EDGE - lineHeight * 9.5);

    pen.setColor(Qt::black);
    pen.setWidthF(0.5 / m_onepixel);
    pen.setStyle(Qt::PenStyle::SolidLine);
    painter->setPen(pen);

    painter->drawLine(0, BOTTOM_EDGE - lineHeight * 7, MIDDLE,
                      BOTTOM_EDGE - lineHeight * 7);

    painter->drawLine(0, BOTTOM_EDGE - lineHeight * 5, MIDDLE,
                      BOTTOM_EDGE - lineHeight * 5);

    painter->drawLine(MIDDLE, BOTTOM_EDGE - lineHeight * 2, RIGHT_EDGE,
                      BOTTOM_EDGE - lineHeight * 2);

    painter->drawLine(0, BOTTOM_EDGE - lineHeight, RIGHT_EDGE,
                      BOTTOM_EDGE - lineHeight);

    painter->drawLine(0, BOTTOM_EDGE, RIGHT_EDGE, BOTTOM_EDGE);


    // PYSTY VIIVAT

    painter->drawLine(20 / m_onepixel, BOTTOM_EDGE - lineHeight * 9.5,
                      20 / m_onepixel, BOTTOM_EDGE - lineHeight * 5);

    painter->drawLine(20 / m_onepixel, BOTTOM_EDGE, 20 / m_onepixel,
                      BOTTOM_EDGE - lineHeight);

    painter->drawLine(MIDDLE, BOTTOM_EDGE - lineHeight * 9.5, MIDDLE,
                      BOTTOM_EDGE);

    painter->drawLine(MIDDLE + 800, BOTTOM_EDGE, MIDDLE + 800,
                      BOTTOM_EDGE - lineHeight * 2);

    painter->drawLine(MIDDLE + MIDDLE / 2, BOTTOM_EDGE, MIDDLE + MIDDLE / 2,
                      BOTTOM_EDGE - lineHeight);

    // POHJA TEKSTIT
    painter->setFont(QFont("Helvetica", 6));

    painter->drawStaticText(
        MIDDLE + 1000, BOTTOM_EDGE + 100,
        QStaticText("Maksu välitetään saajalle maksujenvälityksen ehtojen"));
    painter->drawStaticText(
        MIDDLE + 1000, BOTTOM_EDGE + 200,
        QStaticText("mukaisesti ja vain maksajan ilmoittaman tilinumeron"));
    painter->drawStaticText(MIDDLE + 1000, BOTTOM_EDGE + 300,
                            QStaticText("perusteella."));
    painter->drawStaticText(
        MIDDLE + 1000, BOTTOM_EDGE + 400,
        QStaticText("Betalningen förmedlas till mottagaren enligt villkoren "));
    painter->drawStaticText(
        MIDDLE + 1000, BOTTOM_EDGE + 500,
        QStaticText("för betalningsförmedling och endast till det "));
    painter->drawStaticText(MIDDLE + 1000, BOTTOM_EDGE + 600,
                            QStaticText("kontonummer som betalaren angivit."));

    painter->setFont(QFont("Helvetica", 7));

    painter->drawStaticText(250, BOTTOM_EDGE - lineHeight / 1.1,
                            QStaticText("Tililtä nro"));
    painter->drawStaticText(40, BOTTOM_EDGE - lineHeight / 2.1,
                            QStaticText("Från konto nr"));

    painter->drawStaticText(MIDDLE + 50, BOTTOM_EDGE - lineHeight,
                            QStaticText("Eräpäivä"));
    painter->drawStaticText(MIDDLE + 50, BOTTOM_EDGE - lineHeight * 0.5,
                            QStaticText("Förfallodag"));

    painter->drawStaticText(MIDDLE + 50 + MIDDLE / 2, BOTTOM_EDGE - lineHeight,
                            QStaticText("Euro"));

    painter->drawStaticText(200, BOTTOM_EDGE - 150 - lineHeight * 1.5,
                            QStaticText("Allekirjoitus"));
    painter->drawStaticText(250, BOTTOM_EDGE + 50 - lineHeight * 1.5,
                            QStaticText("Underskrift"));

    painter->drawStaticText(MIDDLE + 50, BOTTOM_EDGE - lineHeight * 2,
                            QStaticText("Viitenumero"));
    painter->drawStaticText(MIDDLE + 50, BOTTOM_EDGE - lineHeight * 1.5,
                            QStaticText("Ref. nr"));

    // TODO: Käännä sivuttain
    // painter->drawStaticText(0,100, QStaticText("TILISIIRTO. GIRERING"));

    painter->drawStaticText(250, BOTTOM_EDGE - lineHeight * 5,
                            QStaticText("Maksajan"));
    painter->drawStaticText(300, BOTTOM_EDGE + 100 - lineHeight * 5,
                            QStaticText("nimi ja"));
    painter->drawStaticText(350, BOTTOM_EDGE + 200 - lineHeight * 5,
                            QStaticText("osoite"));
    painter->drawStaticText(200, BOTTOM_EDGE + 300 - lineHeight * 5,
                            QStaticText("Betalarens"));
    painter->drawStaticText(250, BOTTOM_EDGE + 400 - lineHeight * 5,
                            QStaticText("nams och"));
    painter->drawStaticText(300, BOTTOM_EDGE + 500 - lineHeight * 5,
                            QStaticText("adress"));

    painter->drawStaticText(400, BOTTOM_EDGE - lineHeight * 6.5,
                            QStaticText("Saaja"));
    painter->drawStaticText(150, BOTTOM_EDGE - lineHeight * 6,
                            QStaticText("Mottagare"));

    painter->drawStaticText(400, BOTTOM_EDGE - lineHeight * 9,
                            QStaticText("Saajan"));
    painter->drawStaticText(250, BOTTOM_EDGE - lineHeight * 8.5,
                            QStaticText("Tilinumero"));
    painter->drawStaticText(150, BOTTOM_EDGE - lineHeight * 8,
                            QStaticText("Mottagarens"));
    painter->drawStaticText(80, BOTTOM_EDGE - lineHeight * 7.5,
                            QStaticText("kontonummer"));
    painter->drawStaticText(1000, BOTTOM_EDGE - lineHeight * 9.5,
                            QStaticText("IBAN"));
  }

  inline void fillBottom(QPainter *painter) {

    painter->setFont(QFont("Helvetica", 9));

    painter->drawStaticText(1000, BOTTOM_EDGE - lineHeight * 9,
                            QStaticText(Company.Iban));

    painter->drawStaticText(1000, BOTTOM_EDGE - lineHeight * 7,
                            QStaticText(Company.Name));
    painter->drawStaticText(1000, BOTTOM_EDGE - lineHeight * 6.5,
                            QStaticText(Company.Address));
    painter->drawStaticText(1000, BOTTOM_EDGE - lineHeight * 6,
                            QStaticText(Company.Postal + " " + Company.City));

    painter->drawStaticText(1000, BOTTOM_EDGE - lineHeight * 5,
                            QStaticText(Customer.Nimi1));
                            
    painter->drawStaticText(
        1000, BOTTOM_EDGE - lineHeight * 4.5,
        QStaticText(Customer.Katu1 + ", " + Customer.Katu2));
    painter->drawStaticText(
        1000, BOTTOM_EDGE - lineHeight * 4,
        QStaticText(Customer.Postinum + " " + Customer.Kaupunki));

    painter->drawStaticText(MIDDLE + 300, BOTTOM_EDGE - lineHeight * 7,
                            QStaticText("LASKUNNUMERO: " + Invoice_Number));

    painter->drawStaticText(MIDDLE + 1000, BOTTOM_EDGE - lineHeight * 1.8,
                            QStaticText(Reference_Number));

    painter->drawStaticText(MIDDLE + 1000, BOTTOM_EDGE - (lineHeight * 0.7),
                            QStaticText(Due_Date));
    painter->drawStaticText(MIDDLE + 3000, BOTTOM_EDGE - (lineHeight * 0.7),
                            QStaticText(QString::number(totalPrice) + " €"));
  }

  inline void alvAndYtunnus(QPainter *painter) {
    painter->setFont(QFont("Helvetica", 9));
    painter->drawStaticText(1000, BOTTOM_EDGE - lineHeight * 10.7,
                            QStaticText(Company.AlvText));
    painter->drawStaticText(1000, BOTTOM_EDGE - lineHeight * 10.2,
                            QStaticText("Ly Tunnus: " + Company.Y_tunnus));
  }

#define CODE128_C_START 105
#define CODE128_STOP 106

  int codeToChar(int code) { return code + 105; }

  int charToCode(int ch) { return ch - 32; }

  int calculateCheckCharacter(QString code) {
    QByteArray encapBarcode(code.toUtf8()); // Convert code to utf8

    // Calculate check character
    long long sum =
        CODE128_C_START; // The sum starts with the B Code start character value
    int weight = 1;      // Initial weight is 1

    foreach (char ch, encapBarcode) {
      int code_char = charToCode((int)ch); // Calculate character code
      sum += code_char * weight;           // add weighted code to sum
      weight++;                            // increment weight
    }

    int remain = sum % 103; // The check character is the modulo 103 of the sum

    // Calculate the font integer from the code integer
    if (remain >= 95)
      remain += 105;
    else
      remain += 32;

    return remain;
  }

  QString encodeBarcode(QString code) {
    QString encoded;

    encoded.prepend(
        QChar(codeToChar(CODE128_C_START))); // Start set with B Code 104
    encoded.append(code);
    encoded.append(QChar(calculateCheckCharacter(code)));
    encoded.append(
        QChar(codeToChar(CODE128_STOP))); // End set with Stop Code 106

    return encoded;
  }

  inline void printBarcode(QString text, QPainter *painter) {
    double MmToDot = 8;

    QRect barcodeRect =
        QRect(5 * MmToDot, 10 * MmToDot, 67.5 * MmToDot, 10 * MmToDot);
    QRect barcodeTextRect =
        QRect(5 * MmToDot, 20.5 * MmToDot, 67.5 * MmToDot, 5 * MmToDot);

    QFont barcodefont = QFont("Code 128", 25, QFont::Normal);
    barcodefont.setLetterSpacing(QFont::AbsoluteSpacing, 0.0);
    painter->setFont(barcodefont);

    QString arr = encodeBarcode(text);
    // painter->drawText(barcodeRect, Qt::AlignCenter, arr);
    painter->drawStaticText(0, BOTTOM_EDGE + 100, QStaticText(arr));


    // painter->setFont(QFont("PT Sans", 10));
    // painter->drawText(barcodeTextRect, Qt::AlignCenter, text);
  }

  inline void itemList(QPainter *painter) {

    QPen pen;
    pen.setColor(Qt::black);
    pen.setWidthF(2);
    pen.setStyle(Qt::PenStyle::SolidLine);
    painter->setPen(pen);

    // Header
    painter->drawStaticText(1200, 300 + lineHeight * 7.6,
                            QStaticText("Nimike"));
    painter->drawStaticText(RIGHT_EDGE - 3700, 300 + lineHeight * 7.6,
                            QStaticText("Kpl"));
    painter->drawStaticText(RIGHT_EDGE - 2700, 300 + lineHeight * 7.6,
                            QStaticText("á-hinta"));
    painter->drawStaticText(RIGHT_EDGE - 1500, 300 + lineHeight * 7.6,
                            QStaticText("Yhteensä"));

    painter->drawLine(600, 300 + lineHeight * 8.2, RIGHT_EDGE - 600,
                      300 + lineHeight * 8.2);

    for (int i = 0; i < Products.size(); ++i) {

      // Item
      painter->drawStaticText(1200, 300 + lineHeight * (8.4 + i),
                              QStaticText(Products[i].NameProd));

      // starthight + (0.5 * i)
      painter->drawStaticText(
          RIGHT_EDGE - 3650, 300 + lineHeight * (8.4 + i),
          QStaticText(QString("%1").arg(Products[i].KplProd)));
      painter->drawStaticText(
          RIGHT_EDGE - 2650, 300 + lineHeight * (8.4 + i),
          QStaticText(QString("%1").arg(Products[i].PriceProd)));
      painter->drawStaticText(
          RIGHT_EDGE - 1400, 300 + lineHeight * (8.4 + i),
          QStaticText(
              QString("%1").arg(Products[i].KplProd * Products[i].PriceProd)));

      totalPrice += Products[i].KplProd * Products[i].PriceProd;
      painter->drawLine(600, 300 + lineHeight * (9 + i), RIGHT_EDGE - 600,
                        300 + lineHeight * (9 + i));
    }
  }
};

#endif // INVOICETEMPLATE_HPP
