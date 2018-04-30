/*
 * SPDX-License-Identifier: BSD-3-Clause
 * Copyright (c) 2017 Markus Jylhänkangas
 */

#include "mainwindow.hpp"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[]) {

  QApplication a(argc, argv);
  MainWindow w;
  w.show();

  return a.exec();
}
