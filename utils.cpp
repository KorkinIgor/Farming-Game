#define _CRT_SECURE_NO_WARNINGS
#include "utils.h"
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <cstdio>

using namespace std;

void cell_cleaning() {
	printf("¬ведите корректное значение \n");
	while (getchar() != '\n');
}

void parser() {
	ifstream file("input.txt");
	if (!file.is_open()) {
		printf("не удалось открыть файл \n");
	}
	else {
		string line;
		int parsed_count = 0, skipped_count = 0;
		while (std::getline(file, line)) {
			if (line.empty() || line[0] == '#') { skipped_count++; continue; }
			size_t separator = line.find('=');
			if (separator == std::string::npos) { skipped_count++; continue; }

			std::string key = line.substr(0, separator);
			std::string value = line.substr(separator + 1);
			if (key.empty() || value.empty()) { skipped_count++; continue; }

			std::cout << key << "=" << value << "\n";
			std::ofstream out_file("output.txt", std::ios::app);
			out_file << key << "=" << value << "\n";
			out_file.close();
			parsed_count++;
		}
		std::cout << "допущенно:" << parsed_count << " —кип:" << skipped_count << "\n";
		file.close();
	}
}