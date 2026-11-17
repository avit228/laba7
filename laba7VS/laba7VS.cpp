/******************************
* Структуры и файлы           *
* Вариант 1                   *
* Витвинова Альвина           *
******************************/
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <iomanip>
#include <clocale>

using namespace std;

struct Product {
    string name, country, supplier;
    int deliveryTime = 0;
    double volume = 0.0;
};

vector<string> splitFields(const string& line) {
    vector<string> fields;
    size_t start = 0;
    while (true) {
        size_t sep = line.find(';', start);
        if (sep == string::npos) {
            fields.push_back(line.substr(start));
            break;
        }
        fields.push_back(line.substr(start, sep - start));
        start = sep + 1;
    }
    return fields;
}

bool readInt(const string& text, int& value) {
    stringstream parser(text);
    char extra;
    if (!(parser >> value)) return false;
    return !(parser >> extra);
}

bool readDouble(const string& text, double& value) {
    stringstream parser(text);
    char extra;
    if (!(parser >> value)) return false;
    return !(parser >> extra);
}

bool readFile(const string& filename, vector<Product>& products) {
    ifstream input(filename);
    if (!input.is_open()) {
        cout << "ERROR: Cannot open input file\n";
        return false;
    }
    string line;
    int lineNumber = 0;

    while (getline(input, line)) {
        lineNumber++;
        if (!line.empty() && line.back() == '\r') line.pop_back();
        if (line.empty()) continue;

        vector<string> fields = splitFields(line);
        if (fields.size() != 5) {
            cout << "ERROR line " << lineNumber << "\n";
            return false;
        }

        Product p;
        p.name = fields[0];
        p.country = fields[1];
        p.supplier = fields[2];

        if (!readInt(fields[3], p.deliveryTime) || !readDouble(fields[4], p.volume)) {
            cout << "ERROR line " << lineNumber << "\n";
            return false;
        }
        products.push_back(p);
    }
    return true;
}

void processData(const vector<Product>& products, const string& searchName, ostringstream& report) {
    int count = 0, totalTime = 0;
    double totalVolume = 0.0;

    report << "--- Результаты поиска для: " << searchName << " ---\n";
    for (size_t i = 0; i < products.size(); ++i) {
        if (products[i].name == searchName) {
            report << "Страна: " << products[i].country << ", Фирма: " << products[i].supplier << "\n";
            totalTime += products[i].deliveryTime;
            totalVolume += products[i].volume;
            count++;
        }
    }

    if (count == 0) {
        report << "Совпадений не найдено.\n";
    }
    else {
        report << "Средний срок доставки: " << fixed << setprecision(1) << (double)totalTime / count << "\n";
        report << "Общий объем поставок: " << totalVolume << "\n";
    }

    double maxVolume = -1;
    string bestSupplier = "Нет данных";
    for (size_t i = 0; i < products.size(); ++i) {
        if (products[i].volume > maxVolume) {
            maxVolume = products[i].volume;
            bestSupplier = products[i].supplier;
        }
    }

    report << "\n--- Общая статистика ---\n";
    report << "Фирма с наибольшим объемом партии: " << bestSupplier << " (Объем: " << maxVolume << ")\n";
}

int main() {
    setlocale(LC_ALL, "Russian");

    // --- ХИТРОСТЬ: Автоматически создаем файл с данными, если его нет ---
    // Это избавит вас от ошибки "Cannot open input file"
    ifstream testFile("input.txt");
    if (!testFile.is_open()) {
        ofstream createFile("input.txt");
        createFile << "TV;Japan;Sony;10;500\n";
        createFile << "Laptop;China;Lenovo;5;300\n";
        createFile << "TV;Korea;Samsung;7;600\n";
        createFile << "TV;China;Xiaomi;12;450\n";
        createFile.close();
    }
    testFile.close();
    // ---------------------------------------------------------------------

    vector<Product> products;
    if (!readFile("input.txt", products)) return 1;

    ostringstream report;
    processData(products, "TV", report);

    cout << report.str();

    ofstream output("output.txt");
    if (!output.is_open()) {
        cout << "ERROR: Cannot open output file\n";
        return 1;
    }
    output << report.str();
    output.close();

    return 0;
}