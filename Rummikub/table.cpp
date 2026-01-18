/**
*
* Solution to course project #07
* Introduction to programming course
* Faculty of Mathematics and Informatics of Sofia University
* Winter semester 2025/2026
*
* @author Stefan Kozhuharov
* @idnumber 9MIO600529
* @compiler VC
*
* <containts functions related to actions with the table and the structure for the table and table combinations>
*
*/
#include "table.h"
#include "ui.h"
#include <iostream>

using namespace std;

void initTable(Table& table) {

	table.count = 0;

}

void printTable(const Table& table) {

	cout << endl << "=== TABLE ===" << endl;

	if (table.count == 0) {

		cout << "(empty)" << endl;
		return;

	}

	for (int c = 0; c < table.count; c++) {

		cout << "[" << c + 1 << "] ";

		for (int t = 0; t < table.combos[c].count; t++) {

			printTile(table.combos[c].tiles[t]);
			cout << " ";

		}

		cout << endl;

	}

}