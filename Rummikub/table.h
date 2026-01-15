#pragma once
#include "deck.h"

const int MAX_TABLE_COMBINATIONS = 200;

struct TableCombination {

	Tile tiles[DECK_SIZE];
	int count;

};

struct Table {

	TableCombination combos[MAX_TABLE_COMBINATIONS];
	int count;

};

void initTable(Table& table);
void printTable(const Table& table);