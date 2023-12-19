/*
 * CSE 351 Lab 1b (Manipulating Bits in C)
 *
 * Name(s): Jaehyeon Ahn
 * NetID(s): 2377324
 *
 * This is a file for managing a store of various aisles, represented by an
 * array of 64-bit integers. See aisle_manager.c for details on the aisle
 * layout and descriptions of the aisle functions that you may call here.
 *
 * Written by Porter Jones (pbjones@cs.washington.edu)
 */

#include <stddef.h>  // To be able to use NULL
#include "aisle_manager.h"
#include "store_client.h"
#include "store_util.h"

// Number of aisles in the store
#define NUM_AISLES 10

// Number of sections per aisle
#define SECTIONS_PER_AISLE 4

// Number of items in the stockroom (2^6 different id combinations)
#define NUM_ITEMS 64

// Global array of aisles in this store. Each unsigned long in the array
// represents one aisle.
unsigned long aisles[NUM_AISLES];

// Array used to stock items that can be used for later. The index of the array
// corresponds to the item id and the value at an index indicates how many of
// that particular item are in the stockroom.
int stockroom[NUM_ITEMS];


/* Starting from the first aisle, refill as many sections as possible using
 * items from the stockroom. A section can only be filled with items that match
 * the section's item id. Prioritizes and fills sections with lower addresses
 * first. Sections with lower addresses should be fully filled (if possible)
 * before moving onto the next section.
 */
void refill_from_stockroom() {
  for (int aisle_index = 0; aisle_index < NUM_AISLES; aisle_index++) {
    unsigned long* aisle = &aisles[aisle_index];
    for (int section_index = 0; section_index < SECTIONS_PER_AISLE; section_index++) {
      unsigned short section_id = get_id(aisle, section_index);
      int items_needed = 10 - num_items(aisle, section_index);

      if (items_needed > 0 && section_id < NUM_ITEMS) {
        int items_available = stockroom[section_id];

        if (items_available >= items_needed) {
          add_items(aisle, section_index, items_needed);
          stockroom[section_id] -= items_needed;
        } else {
          add_items(aisle, section_index, items_available);
          stockroom[section_id] = 0;
        }
      }
    }
  }
}

/* Remove at most num items from sections with the given item id, starting with
 * sections with lower addresses, and return the total number of items removed.
 * Multiple sections can store items of the same item id. If there are not
 * enough items with the given item id in the aisles, first remove all the
 * items from the aisles possible and then use items in the stockroom of the
 * given item id to finish fulfilling an order. If the stockroom runs out of
 * items, you should remove as many items as possible.
 */
int fulfill_order(unsigned short id, int num) {
  int removed_item = 0;
  for (int aisle_index = 0; aisle_index < NUM_AISLES; aisle_index++) {
    unsigned long* aisle = &aisles[aisle_index];
    for (int section_index = 0; section_index < SECTIONS_PER_AISLE; section_index++) {
      unsigned short section_id = get_id(aisle, section_index);

      if (section_id == id) {
        int items_in_section = num_items(aisle, section_index);
        int items_to_remove = (num - removed_item < items_in_section) ? num - removed_item : items_in_section;
        remove_items(aisle, section_index, items_to_remove);
        removed_item += items_to_remove;

        if (removed_item >= num) {
          return removed_item;
        }
      }
    }
  }

  if (removed_item < num) {
    int items_from_stockroom = (stockroom[id] < num - removed_item) ? stockroom[id] : num - removed_item;
    stockroom[id] -= items_from_stockroom;
    removed_item += items_from_stockroom;
  }

  return removed_item;
}

/* Return a pointer to the first section in the aisles with the given item id
 * that has no items in it or NULL if no such section exists. Only consider
 * items stored in sections in the aisles (i.e., ignore anything in the
 * stockroom). Break ties by returning the section with the lowest address.
 */
unsigned short* empty_section_with_id(unsigned short id) {
  unsigned short* result = NULL;
  for (int aisle_index = 0; aisle_index < NUM_AISLES; aisle_index++) {
    unsigned long* aisle = &aisles[aisle_index];

    for (int section_index = 0; section_index < SECTIONS_PER_AISLE; section_index++) {
      unsigned short spaces = get_spaces(aisle, section_index);
      unsigned short section_id = get_id(aisle, section_index);

      if (id == section_id && spaces == 0) {
        result = (unsigned short*) aisle + section_index;
        return result;
      } 
    }
  }
  return result;
}



/* Return a pointer to the section with the most items in the store. Only
 * consider items stored in sections in the aisles (i.e., ignore anything in
 * the stockroom). Break ties by returning the section with the lowest address.
 */
unsigned short* section_with_most_items() {
  int max_item = -1;
  unsigned short* result = NULL;
  for (int aisle_index = 0; aisle_index < NUM_AISLES; aisle_index++) {
    unsigned long* aisle = &aisles[aisle_index];

    for (int section_index = 0; section_index < SECTIONS_PER_AISLE; section_index++) {
      unsigned short section_item = num_items(aisle, section_index);
      
      if (section_item > max_item) {
        max_item = section_item;
        result = (unsigned short*) aisle + section_index;
      }
    }
  }
  return result;
}