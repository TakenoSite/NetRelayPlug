#ifndef __TAG_ITEM_H_
#define __TAG_ITEM_H_

/* XML analysis 
 * Search from xml body based on ID and copy Item to resolve
 */

int xml_item_tag(char* r_xml_item, char* id, char* resolve, int buf_size);
int xml_item_search(char* r_xml_item, char* id);  


#endif 
