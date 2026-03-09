#include "fileHandler.h"
#include <iostream>
#include <fstream>
#include <sstream>

using namespace std;

vector<Data> FileHandler::readCSV(string filename)
{

      vector<Data> dataset;
      ifstream file(filename);

      if (!file.is_open())
      {
            cout << "File tidak bisa dibuka\n";
            return dataset;
      }

      string line;

      getline(file, line); // skip header

      while (getline(file, line))
      {

            stringstream ss(line);
            string temp;

            Data d;

                  getline(ss, temp, ';');
                  try {
                        d.id = stoi(temp); // Invoice
                  } catch (const std::exception &e) {
                        // jika id tidak dapat di-convert, lewati baris ini
                        continue;
                  }

            getline(ss, d.content, ';'); // StockCode

            getline(ss, d.name, ';'); // Description

                  getline(ss, temp, ';');
                  try {
                        d.size = stoi(temp); // Quantity
                  } catch (const std::exception &e) {
                        d.size = 0;
                  }

            getline(ss, d.upload_date, ';'); // InvoiceDate

            getline(ss, temp, ';'); // Price (ignore)

            getline(ss, temp, ';'); // CustomerID (ignore)

            getline(ss, d.source, ';'); // Country

            dataset.push_back(d);
      }

      return dataset;
}