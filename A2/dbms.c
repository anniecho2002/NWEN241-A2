#include "dbms.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>



int db_show_row(const struct db_table *db, unsigned int row){
	if(row > db -> rows_used){ return 0; }
	
	struct album *table = db -> table; // pull out the table value inside the db structure (points to an album value)
	struct album record = *(table + row);
	if(table + row != NULL){
		printf("%6lu:", record.id);
		
		if(strlen(record.title) > 20){ printf("%.20s:", record.title); }
		else{ printf("%20s:", record.title); }
		
		if(strlen(record.artist) > 20){ printf("%.20s:", record.artist); }
		else{ printf("%20s:", record.artist); }
		
		printf("%4hu \n", record.year);
		return 1;
	}
	return 0;
}



int db_add_row(struct db_table *db, struct album *a){
	if(db == NULL){ return 0; }
	if(db -> rows_used < db -> rows_total){
		db -> table[db -> rows_used] = *a;
		db -> rows_used = db -> rows_used + 1;
	}
	else if(db -> rows_used >= db -> rows_total){
		struct album *mem = (struct album *)realloc(db -> table, (db -> rows_total + 5)*sizeof(struct album));
		if(mem == NULL){ return 0; }
		db -> table = mem;
		
		db -> table[db -> rows_used] = *a;
		db -> rows_used = db -> rows_used + 1; // update number of used rows
		db -> rows_total = db -> rows_total + 5; // add 5 rows to total
		
	}
	return 1;
}


int db_remove_row(struct db_table *db, unsigned long id){
	int value = 0;
	
	// going through and finding the record that needs to be removed
	for(int r = 0; r < db -> rows_used; r++){
		unsigned int albumID = db -> table[r].id;
		if(albumID == id){
			// then the record needs to be removed and all of the ones underneath are moved upwards
			for(int pos = r; pos < db -> rows_used - 1; pos++){
				// going through all the ones that come after it
				db -> table[pos] = db -> table[pos + 1];
			}
			value = 1;
			db -> rows_used = db -> rows_used - 1;
		}
	}
	
	if(db -> rows_total - db -> rows_used >= 5){
		struct album *freeRows = (struct album *)realloc(db -> table, (db -> rows_total - 5)*sizeof(struct album));
		db -> table = freeRows;
		db -> rows_total = db -> rows_total - 5;
	}
	return value;
	

}
