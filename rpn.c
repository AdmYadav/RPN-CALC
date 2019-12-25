/* Fill in your Name and GNumber in the following two comment fields
 * Name:
 * GNumber:
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "stack.h"
#include "token.h"
#include "hash.h"

/* Local Function Declarations (that you need to finish implementing) */
static int read_file(char *filename, char *line);
static int parse_token(Symtab *symtab, Stack_head *stack, Token *tok);

/* Pre-Written Local Function Declarations */
static void print_header(char *filename, int step);
static void print_step_header(int step);
static void print_step_footer(Symtab *symtab, Stack_head *stack);
static void print_step_output(int val);

/* Defines the largest line that can be read from a file */
#define MAX_LINE_LEN 255

/* This has been written for you.
 * Main function to run your program (written for you).
 * 1) Opens the file using the passed in filename. (your function)
 * -- If the file is not found (ie. fopen returns NULL), then exit(-1);
 * 2) Reads one line from the file.
 * -- The programs will all only be one line in size.
 * -- That line may be up to MAX_LINE_LEN long.
 * 3) Closes the file.
 * 4) Calls token_read_line(line, strlen(line))
 * -- This parses the line and prepares the tokens to be ready to get.
 * 5) While there are tokens remaining to parse: token_has_next() != 0
 * -- a) Get the next token: token_get_next()
 * 6) Parse the token (your function)
 * 7) Print out all of the relevant information
 */
int rpn(Stack_head *stack, Symtab *symtab, char *filename)
{
	int step = 0; /* Used to track the program steps */
	int ret = 0;
	char line[MAX_LINE_LEN];
	Token *tok = NULL;

	/* Complete the read_file function that is defined later in this file. */
	ret = read_file(filename, line);
	if (ret != 0)
	{
		printf("Error: Cannot Read File %s.  Exiting\n", filename);
		exit(-1);
	}

	/* Pass the line into the tokenizer to initialize that system */
	token_read_line(line, strlen(line));

	/* Prints out the nice program output header */
	print_header(filename, step);

	/* Iterate through all tokens */
	while (token_has_next())
	{
		/* Begin the next step of execution and print out the step header */
		step++; /* Begin the next step of execution */
		print_step_header(step);

		/* Get the next token */
		tok = token_get_next();
		/* Complete the implementation of this function later in this file. */
		ret = parse_token(symtab, stack, tok);
		if (ret != 0)
		{
			printf("Critical Error in Parsing.  Exiting Program!\n");
			exit(-1);
		}

		/* Prints out the end of step information */
		print_step_footer(symtab, stack);
	}

	return 0;
}

/* (IMPLEMENT THIS FUNCTION)
 * Local function to open a file or exit.
 * Follow the Directions in the Project Documentation for this Function
 * Open filename, read its contents (up to MAX_LINE_LEN) into line, then
 *   close the file and return 0.
 * On any file error, return -1.
 */
static int read_file(char *filename, char *line)
{
	/* Implement This Function */
	FILE *fp = fopen(filename, "r");
	if (fp == NULL)
	{
		return -1;
	}
	else
	{
		fgets(line, MAX_LINE_LEN, fp);
		fclose(fp);
		return 0;
	}
}

/* (IMPLEMENT THIS FUNCTION)
 * Parses the Token to implement the rpn calculator features
 * Follow the Directions in the Project Documentation for this Function
 * You may implement this how you like, but many small functions would be good!
 * If the token you are passed in is NULL, return -1.
 * If there are any memory errors, return -1.
 */
static int parse_token(Symtab *symtab, Stack_head *stack, Token *tok)
{
	/* Implement This Function */
	if (tok == NULL)
	{
		return -1;
	}

	if (tok->type == TYPE_ASSIGNMENT)
	{ //stack pop 2 token put in symbol table;
		Token *a = stack_pop(stack);
		Token *b = stack_pop(stack);
		if (a->type == TYPE_VARIABLE && b->type == TYPE_VALUE)
		{
			hash_put(symtab, (a->variable), b->value);    //calling hash put
		}
		else
		{
			hash_put(symtab, (b->variable), a->value);              //calling hash put
		}
		token_free(a);
		token_free(b);
		return 0;
	}
	else if (tok->type == TYPE_OPERATOR)
	{ //operator Pop two tokens off the stack, perform the operation on them, push the result.
		int val = 0;
		Token *a = stack_pop(stack);
		Token *b = stack_pop(stack);
		Token *result = NULL;
		if (a->type == TYPE_VARIABLE && b->type == TYPE_VARIABLE)
		{ // if both poped element are variable
			Symbol *sym1 = hash_get(symtab, a->variable);
			Symbol *sym2 = hash_get(symtab, b->variable);
			if (tok->oper == OPERATOR_PLUS)
			{
				val = (sym2->val + sym1->val);
				result = token_create_value(val);
				stack_push(stack, result);
			}
			else if (tok->oper == OPERATOR_MINUS)
			{
				val = (sym2->val - sym1->val);
				result = token_create_value(val);
				stack_push(stack, result);
			}
			else if (tok->oper == OPERATOR_MULT)
			{
				val = (sym2->val * sym1->val);
				result = token_create_value(val);
				stack_push(stack, result);
			}
			else if (tok->oper == OPERATOR_DIV)
			{
				val = (sym2->val / sym1->val);
				result = token_create_value(val);
				stack_push(stack, result);
			}
			token_free(a);
			token_free(b);
			return 0;
		}
		else if (a->type == TYPE_VARIABLE && b->type == TYPE_VALUE)
		{//if one poped is variable and other is value
			Symbol *sym1 = hash_get(symtab, a->variable);
			Token *result = NULL;
			if (tok->oper == OPERATOR_PLUS)
			{
				val = (b->value + sym1->val);
				result = token_create_value(val);
				stack_push(stack, result);
			}
			else if (tok->oper == OPERATOR_MINUS)
			{
				val = (b->value - sym1->val);
				result = token_create_value(val);
				stack_push(stack, result);
			}
			else if (tok->oper == OPERATOR_MULT)
			{
				val = (b->value * sym1->val);
				result = token_create_value(val);
				stack_push(stack, result);
			}
			else if (tok->oper == OPERATOR_DIV)
			{
				val = (b->value / sym1->val);
				result = token_create_value(val);
				stack_push(stack, result);
			}
			token_free(a);
			token_free(b);
			return 0;
		}
		else if (b->type == TYPE_VARIABLE && a->type == TYPE_VALUE)
		{//if one element is variable and other is value
			Symbol *sym2 = hash_get(symtab, b->variable);
			Token *result = NULL;
			if (tok->oper == OPERATOR_PLUS)
			{
				val = (sym2->val + a->value);
				result = token_create_value(val);
				stack_push(stack, result);
			}
			else if (tok->oper == OPERATOR_MINUS)
			{
				val = (sym2->val - a->value);
				result = token_create_value(val);
				stack_push(stack, result);
			}
			else if (tok->oper == OPERATOR_MULT)
			{
				val = (sym2->val * a->value);
				result = token_create_value(val);
				stack_push(stack, result);
			}
			else if (tok->oper == OPERATOR_DIV)
			{
				val = (sym2->val / a->value);
				result = token_create_value(val);
				stack_push(stack, result);
			}
			token_free(a);
			token_free(b);
			return 0;
		}
		else
		{//if n variable is poped
			Token *result = NULL;
			if (tok->oper == OPERATOR_PLUS)
			{
				val = b->value + a->value;
				result = token_create_value(val);
				stack_push(stack, result);
			}
			else if (tok->oper == OPERATOR_MINUS)
			{
				val = b->value - a->value;
				result = token_create_value(val);
				stack_push(stack, result);
			}
			else if (tok->oper == OPERATOR_MULT)
			{
				val = b->value * a->value;
				result = token_create_value(val);
				stack_push(stack, result);
			}
			else if (tok->oper == OPERATOR_DIV)
			{
				val = b->value / a->value;
				result = token_create_value(val);
				stack_push(stack, result);
			}
			token_free(a);
			token_free(b);
			return 0;
		}
	}
	else if (tok->type == TYPE_VARIABLE)
	{
		//variablePush the token on to the stack.
		stack_push(stack, tok);
		return 0;
	}
	else if (tok->type == TYPE_VALUE)
	{
		//value Push the token on to the stack.
		stack_push(stack, tok);
		return 0;
	}
	else if (tok->type == TYPE_PRINT)
	{
		//printPop one token off the stack and print it to the screen as output.
		Token *p = stack_pop(stack);
		if (p->type == TYPE_VARIABLE)
		{
			Symbol *sym1 = hash_get(symtab, p->variable);
			print_step_output(sym1->val);
		}
		else
		{
			print_step_output(p->value);
		}
		return 0;
	}
}

/* This has been written for you.
 * Prints out the main output header
 */
static void print_header(char *filename, int step)
{
	printf("######### Beginning Program (%s) ###########\n", filename);
	printf("\n.-------------------\n");
	printf("| Program Step = %2d\n", step);
	token_print_remaining();
	printf("o-------------------\n");
}

/* This has been written for you.
 * Prints out the information at the top of each step
 */
static void print_step_header(int step)
{
	printf("\n.-------------------\n");
	printf("| Program Step = %2d\n", step++);
}

/* This has been written for you.
 * Prints out the output value (print token) nicely
 */
static void print_step_output(int val)
{
	printf("|-----Program Output\n");
	printf("| %d\n", val);
}

/* This has been written for you.
 * Prints out the information at the bottom of each step
 */
static void print_step_footer(Symtab *symtab, Stack_head *stack)
{
	hash_print_symtab(symtab);
	stack_print(stack);
	token_print_remaining();
	printf("o-------------------\n");
}
