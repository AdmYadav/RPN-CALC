/* Fill in your Name and GNumber in the following two comment fields
 * Name: AJIT YADAV
 * GNumber: 01210173
 */
#include <stdio.h>
#include <stdlib.h>

#include "node.h"
#include "stack.h"

/* (IMPLEMENT THIS FUNCTION)
 * Create a new Stack_head struct on the Heap and return a pointer to it.
 * Follow the Directions in the Project Documentation for this Function
 * On any malloc errors, return NULL
 */
Stack_head *stack_initialize()
{ //initializing and seeting to null
  Stack_head *head = malloc(sizeof(Stack_head));
  if (head == NULL)
  {
    return NULL;
  }
  head->count = 0;
  head->top = NULL;
  return head;
}

/* (IMPLEMENT THIS FUNCTION)
 * Destroy a stack.
 * Follow the Directions in the Project Documentation for this Function
 * If head is NULL, you can just return.
 */
void stack_destroy(Stack_head *head)
{
  
  if (head == NULL)
  {
    return;
  }
  else if (head->top == NULL)
  {
    free(head);
  }
  else
  {
    Node *walker = head->top;
    while (walker != NULL)
    {					//traversing every node and freeing
      Node *temp = walker;
      free(walker);
      walker = temp->next;
    }
    free(head);                         //releasing head
  }
}

/* (IMPLEMENT THIS FUNCTION)
 * Push a new Token on to the Stack.
 * Follow the Directions in the Project Documentation for this Function
 * On any malloc errors, return -1.
 * If there are no errors, return 0.
 */
int stack_push(Stack_head *stack, Token *tok)
{
  if (stack == NULL)
  {
    return -1;
  }
  if (stack->top == NULL)
  {
    Node *first = malloc(sizeof(Node));   // if the element is first element in the stack
    if (first == NULL)
    {
      return -1;
    }
    first->tok = tok;
    first->next = NULL;
    stack->top = first;
    stack->count++;
    return 0;
  }
  else
  {
    Node *walker = stack->top;
    Node *A = malloc(sizeof(Node));       //pushing the element into stack at the top
    if (A == NULL)
    {
      return -1;
    }
    A->next = walker;
    A->tok = tok;
    stack->top = A;
    stack->count++;
    return 0;
  }
}

/* (IMPLEMENT THIS FUNCTION)
 * Pop a Token off of the Stack.
 * Follow the Directions in the Project Documentation for this Function
 * If the stack was empty, return NULL.
 */
Token *stack_pop(Stack_head *stack)
{
  /* Implement this function */
  if (stack == NULL)
  {
    return NULL;
  }
  else                                // returning the first element from stack
  {
    (stack->count)--;
    Node *poptok = stack->top;
    stack->top = poptok->next;
    Token *tok = poptok->tok;
    return (tok);
  }
}

/* (IMPLEMENT THIS FUNCTION)
 * Return the token in the stack node on the top of the stack
 * Follow the Directions in the Project Documentation for this Function
 * If the stack is NULL, return NULL.
 * If the stack is empty, return NULL.
 */
Token *stack_peek(Stack_head *stack)
{
  //returning the top  element from the stack
  if (stack == NULL)
  {
    return NULL;
  }
  else
  {
    return ((stack->top)->tok);
  }
}

/* (IMPLEMENT THIS FUNCTION)
 * Return the number of nodes in the stack.
 * Follow the Directions in the Project Documentation for this Function
 * If stack is NULL, return -1.
 * Return 1 if the stack is empty or 0 otherwise.
 */
int stack_is_empty(Stack_head *stack)
{
 //returning the number of nodes in the stack
  if (stack == NULL)
  {
    return -1;
  }
  else
  {
    return stack->count;
  }
}

/* These two functions are written for you.
 * It recurses the stack and prints out the tokens in reverse order
 * eg. top->2->4->1->8 will print at Stack: 8 1 4 2
 * eg. stack_push(5) will then print Stack: 8 1 4 2 5
 */

/* This is implemented for you.
 * Recursive print. (Local function)
 * Base Case: node == NULL, return
 * Recursive Case: call print_node(node->next, print_data), then print node.
 */
static void print_node(Node *node)
{
  if (node == NULL)
  {
    return;
  }
  token_print(node->tok);
  print_node(node->next);
  return;
}

/* This is implemented for you.
 * Setup function for the recursive calls.  Starts printing with stack->top
 */
void stack_print(Stack_head *stack)
{
  if (stack == NULL)
  {
    return;
  }
  printf("|-----Program Stack\n");
  printf("| ");
  print_node(stack->top);
  printf("\n");
  return;
}
