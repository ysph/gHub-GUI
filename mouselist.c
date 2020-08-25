// Copyright 2020 Mikhail ysph Subbotin

#include "mouselist.h"

int printAllItems(Item* head) {
	if (head == NULL) {
		printf("Error: There is no item you look for!\n");
		return EXIT_FAILURE;
	}

	int length = 0;
	while (head->next != NULL) {
		length++;
		printf("[%d] Device: name=%s, id=0x%02x, interface=%d\n", length, head->name, head->idProduct, head->interface);
		head = head->next;
	}

	return EXIT_SUCCESS;
}

int deleteLinkedList(Item** head_ref) {
	if (head_ref == NULL) {
		printf("Error: There are no items in the list!\n");
		return EXIT_FAILURE;
	}

	Item* current = *head_ref;
	Item* next;

	while (current != NULL) {
		next = current->next;
		free(current);
		current = next;
	}

	*head_ref = NULL;

	return EXIT_SUCCESS;
}

int pushItem(Item** head_ref, const int new_idProduct, const char* new_name, const uint8_t new_interface, const uint8_t new_byte3) {
	Item* new_node = (Item*)malloc(size_of_Item);

	new_node->idProduct = (uint16_t)new_idProduct;
	new_node->name = new_name;
	new_node->interface = new_interface;
	new_node->mByte3 = new_byte3;

	new_node->next = (*head_ref);
	(*head_ref)    = new_node;

	return EXIT_SUCCESS;
}
// FIXME: use generics
bool searchItem(Item* head, const int search_idProduct) {
	Item* current = head;
	while (current != NULL) {
		if (current->idProduct == (uint16_t)search_idProduct)
			return current->interface;
		current = current->next;
	}
	return FALSE;
}

// FIXME: use generics
const int getInterface(Item* head, const int search_idProduct) {
	Item* current = head;
	while (current != NULL) {
		if (current->idProduct == (uint16_t)search_idProduct)
			return current->interface;
		current = current->next;
	}
	return FALSE;
}
// FIXME: use generics
const char* getName(Item* head, const int search_idProduct) {
	Item* current = head;
	while (current != NULL) {
		if (current->idProduct == (uint16_t)search_idProduct)
			return current->name;
		current = current->next;
	}
	return "UNDEFINED";
}
// FIXME: use generics
const uint8_t getByte3(Item* head, const int search_idProduct) {
	Item* current = head;
	while (current != NULL) {
		if (current->idProduct == (uint16_t)search_idProduct)
			return current->mByte3;
		current = current->next;
	}
	return FALSE;
}
const int getSize(Item* head) {
	int length = 0;
	if (head == NULL) {
		return length;
	}
	while (head->next != NULL) {
		length++;
		head = head->next;
	}
	return length;
}
const int getNthId(Item* head, int needed) {
	int nth = 1;
	if (head == NULL) {
		printf("Error: the list is empty!\n");
		return EXIT_FAILURE;
	}
	if (needed <= 0) {
		printf("Error: the number of the item cannot be negative or zero!\n");
		return EXIT_FAILURE;
	}
	Item* current = head;
	while (current->next != NULL) {
		if (nth == needed) {
			return current->idProduct;
		}
		nth++;
		current = current->next;
	}

	return EXIT_FAILURE;
}
