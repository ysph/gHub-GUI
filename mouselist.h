// Copyright 2020 Mikhail ysph Subbotin

#ifndef MOUSE_LIST_H
#define MOUSE_LIST_H

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "miscellaneous.h"

#define WIRED_OR_CABLE 			1
#define WIRELESS_RECEIVER 		2
#define CABLE_BUT_SECOND		2
#define ID_PRODUCT_UNIDENTIFIED 0xc539
#define ID_VENDOR 				0x046d

typedef struct Item {
	uint16_t idProduct;
	const char* name;
	uint8_t interface;
	uint8_t mByte3;

	struct Item* next;
} Item;

static const int size_of_Item = sizeof(Item);
int printAllItems(Item* node);
int deleteLinkedList(Item** head_ref);
int pushItem(Item** head_ref, const int new_idProduct, const char* new_name, const uint8_t new_interface, const uint8_t new_byte3);
bool searchItem(Item* head, const int search_idProduct);
const int getInterface(Item* head, const int search_idProduct);
const char* getName(Item* head, const int search_idProduct);
const uint8_t getByte3(Item* head, const int search_idProduct);
const int getSize(Item* head);
const int getNthId(Item* head, int nth);

#endif  // MOUSE_LIST_H
