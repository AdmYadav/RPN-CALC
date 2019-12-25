/* Fill in your Name and GNumber in the following two comment fields
 * Name: AJIT YADAV
 * GNumber: 01210173
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "node.h"
#include "hash.h"

/* (IMPLEMENT THIS FUNCTION)
 * In this function, you will create a new Symtab struct.
 * Follow the Directions in the Project Documentation for this Function
 * Return the pointer to the new symtab.
 * On any memory errors, return NULL
 */
Symtab *hash_initialize()
{
	//creating and new hash table and initializing to null
	int i = 1;
	Symtab *sym = malloc(sizeof(Symtab));
	if (sym == NULL)
	{
		return NULL;
	}
	else
	{
		sym->size = 0;
		sym->capacity = HASH_TABLE_INITIAL;
		sym->table = malloc(sizeof(Symbol *) * (sym->capacity)); // creation of table
		if (sym->table == NULL)
		{
			return NULL;
		}
		else
		{
			for (i = 0; i < sym->capacity; i++)
			{
				sym->table[i] = NULL;
			}
		}
		return sym;
	}
}

/* (IMPLEMENT THIS FUNCTION)
 * Destroy your Symbol Table.
 * Follow the Directions in the Project Documentation for this Function
 * Return on any memory errors.
 */
void hash_destroy(Symtab *symtab)
{
	//traversing every symbol in hash and freeing
	int i = 0;
	if (symtab == NULL)
	{
		return;
	}
	for (i = 0; i < symtab->capacity; i++)
	{
		if (symtab->table[i] != NULL)
		{
			Symbol *walker = symtab->table[i];
			Symbol *reaper = symtab->table[i];
			while (walker != NULL)
			{
				walker = walker->next;
				symbol_free(reaper);
				reaper = walker;
			}
		}
	}
	free(symtab);
	return;
}

/* (IMPLEMENT THIS FUNCTION)
 * Return the capacity of the table inside of symtab.
 * If symtab is NULL, return -1;
 */
int hash_get_capacity(Symtab *symtab)
{
	if (symtab == NULL)
	{
		return -1;
	}
	else
	{
		return (symtab->capacity);    //returns the total capacity of hash table
	}
}

/* (IMPLEMENT THIS FUNCTION)
 * Return the number of used indexes in the table (size) inside of symtab.
 * If symtab is NULL, return -1;
 */
int hash_get_size(Symtab *symtab)
{

	int i = 0;
	int index = 0;
	if (symtab == NULL)
	{
		return -1;
	}
	else
	{
		return(symtab->size);  //returning the size of hash table
	}
}

/* (IMPLEMENT THIS FUNCTION)
 * Adds a new Symbol to the symtab via Hashing.
 * Follow the Directions in the Project Documentation for this Function
 * If symtab is NULL, there are any malloc errors, or if any rehash fails, return -1;
 * Otherwise, return 0;
 */
int hash_put(Symtab *symtab, char *var, int val)
{
	if (symtab == NULL)
	{
		return -1;
	}
	else
	{
		double load = ((symtab->size) / (symtab->capacity));
		long a;
		int index;
		a = hash_code(var);
		Symbol *symb = malloc(sizeof(Symbol));
		Symbol *temp = NULL;
		if (symb == NULL)
		{
			return -1;
		}
		else
		{

			if (load >= 2.0)
			{
				hash_rehash(symtab, (symtab->capacity * 2));  //as the load is greater than 2 a new hash table is required
			}
			strcpy(symb->variable, var);
			symb->val = val;
			symb->next = NULL;
			index = a % (symtab->capacity);
			if ((symtab->table[index]) == NULL)
			{
				symtab->table[index] = symb;
				symtab->size++;
			}
			else
			{
				Symbol *walker = symtab->table[index];
				while (walker->next != NULL)
				{
					if (strcmp(var, walker->variable) == 0)
					{
						walker->val = val;
						return 0;
					}

					walker = walker->next;
				}
				if (walker->next == NULL)
				{
					walker->next = symb;
					symtab->size++;
				}
			}

			return 0;
		}
	}
}
/* (IMPLEMENT THIS FUNCTION)
 * Gets the Symbol for a variable in the Hash Table.
 * Follow the Directions in the Project Documentation for this Function
 * On any NULL symtab or memory errors, return NULL
 */
Symbol *hash_get(Symtab *symtab, char *var)
{
	long hash_c = hash_code(var);
	int index = 0;
	int flag = 0;
	index = hash_c % (symtab->capacity);   //finding the index of symbol
	if (symtab == NULL)
	{
		return NULL;
	}
	else
	{
		Symbol *sym_temp = NULL;
		sym_temp = symtab->table[index];
		if (sym_temp->next == NULL)
		{
			Symbol *sym = symbol_copy(sym_temp);
			return sym;
		}
		else
		{
			while (sym_temp != NULL)
			{
				if (strcmp(var, sym_temp->variable) == 0)
				{
					Symbol *sym = symbol_copy(sym_temp);
					flag = 1;
					return sym;
				}
				else
				{
					flag = 0;
				}
				sym_temp = sym_temp->next;
			}
		}
		if (flag == 0)
		{
			return NULL;
		}
	}
}
/* (IMPLEMENT THIS FUNCTION)
 * Doubles the size of the Array in symtab and rehashes.
 * Follow the Directions in the Project Documentation for this Function
 * If there were any memory errors, set symtab->array to NULL
 * If symtab is NULL, return immediately.
 */
void hash_rehash(Symtab *symtab, int new_capacity)
{
	int i = 0;
	if (symtab == NULL)
	{
		return;
	}
	else
	{
		Symbol **table_new = malloc(sizeof(Symbol *) * (new_capacity));  //creating the new table
		if (table_new == NULL)
		{
			return NULL;
		}
		else
		{
			for (i = 0; i < new_capacity; i++)
			{
				table_new[i] = NULL;                             //initializing the table to NULL
 			}
		}
		for (i = 0; i < (symtab->capacity); i++)
		{
			long a = 0;
			int index = 0;
			Symbol *sym = symtab->table[i];
			if (sym->next == NULL)                                  //if the old table has only one symbol at index
			{
				Symbol *sym1 = malloc(sizeof(Symbol));
				strcpy(sym1->variable, sym->variable);
				sym1->val = sym->val;
				sym1->next = NULL;

				a = hash_code(sym1->variable);
				index = a % (new_capacity);
				if (table_new[index] == NULL)
				{
					table_new[index] = sym1;
				}
				else
				{
					Symbol *walker = table_new[index];
					while (walker->next != NULL)
					{
						walker = walker->next;
					}
					walker->next = sym1;
				}
			}
			else
			{
				while (sym != NULL)                                   //travering in seperate chain for putting in new table
				{
					Symbol *sym1 = malloc(sizeof(Symbol));
					strcpy(sym1->variable, sym->variable);
					sym1->val = sym->val;
					sym1->next = NULL;

					a = hash_code(sym->variable);
					index = a % (new_capacity);
					if (table_new[index] == NULL)
					{
						table_new[index] = sym1;
					}
					else
					{
						Symbol *walker1 = table_new[index];
						if (walker1->next == NULL)
						{
							walker1->next = sym1;
						}
						else
						{
							while (walker1->next != NULL)
							{
								walker1 = walker1->next;
							}
							sym1->next = NULL;
							walker1->next = sym1;
						}
					}

					sym = sym->next;
				}
			}
		}
		for (i = 0; i < symtab->capacity; i++)                        //freeing old table
		{
			if (symtab->table[i]->next == NULL)
			{
				free(symtab->table[i]);
			}
			else
			{
				Symbol *walker = symtab->table[i];
				Symbol *reaper = walker;
				while (walker != NULL)
				{
					reaper = walker->next;
					free(walker);
					walker = reaper;
				}
			}
		}
		symtab->table = table_new;
		symtab->capacity = (symtab->capacity) * 2;
		return;
	}
	return;
}

/* Implemented for you.
 * Provided function to print the symbol table */
void hash_print_symtab(Symtab *symtab)
{
	if (symtab == NULL)
	{
		return;
	}
	printf("|-----Symbol Table [%d size/%d cap]\n", symtab->size, symtab->capacity);

	int i = 0;
	Symbol *walker = NULL;

	/* Iterate every index, looking for symbols to print */
	for (i = 0; i < symtab->capacity; i++)
	{
		walker = symtab->table[i];
		/* For each found linked list, print every symbol therein */
		while (walker != NULL)
		{
			printf("| %10s: %d \n", walker->variable, walker->val);
			walker = walker->next;
		}
	}
	return;
}

/* This function is written for you.
 * This computes the hash function for a String
 */
long hash_code(char *var)
{
	long code = 0;
	int i;
	int size = strlen(var);

	for (i = 0; i < size; i++)
	{
		code = (code + var[i]);
		if (size == 1 || i < (size - 1))
		{
			code *= 128;
		}
	}

	return code;
}
