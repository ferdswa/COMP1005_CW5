#include "rpn.h"

#include "stack.h"
#include "queue.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <unistd.h>
/* convert an infix expression to postfix (rpn) using the */
/* shunting yard algorithm. */
/* return a queue containing the postfix expression. */
/* if an error occurs during evaluation, return silently with NULL. */
int bisNumber(char);
int isOperator(char);
int isLeftAssociative(char);
int getPrecedence(char);
int comparePrecedence(char, char);
int isNumber(char, char, char);
int shouldIPop(char,char);
int isOperatorString(char*);
double calc(char*,double,double);
Queue *infix_to_postfix(char *expr)
{
	Queue *outputQueue;
	Stack *operatorStack;
	char buffer[8];
	char popped[8];
	char bufOp[8];
	char curOp[2];
	char current,*popper,*peepO,prevOperator,*ptr,*toBeSplit=expr;
	char *currentAsString;
	char spaceless[64];
	int i,j=0,k=0,iop,shouldPop;
	if(strchr(expr,32)!=NULL)/*Lets get rid of those spaces*/
	{
		memset(spaceless,0,sizeof(spaceless));
		ptr=strtok(toBeSplit," ");
		while(ptr!=NULL)
		{
			strcat(spaceless,ptr);
			ptr=strtok(NULL," ");
		}
		expr=spaceless;
	}
	j=0;
	memset(buffer,0,sizeof(buffer));
	memset(popped,0,sizeof(popped));
	memset(bufOp,0,sizeof(bufOp));
	currentAsString=calloc(2,sizeof(char));
	outputQueue=initialise_queue();
	operatorStack=initialise_stack();
	for(i=0;i<strlen(expr);i++)
	{
		int isN;
		current = expr[i];
		isN=isNumber(expr[i-1],current,expr[i+1]);
		if(isN==1)/*Builds the number until it hits a non-number*/
		{
			buffer[j]=current;
			j++;
		}
		else if(isN==0)/*Non-number (operator) encountered*/
		{
			curOp[0]=current;
			curOp[1]='\0';
			memcpy(currentAsString,curOp,2);
			if(strlen(buffer)>0){
				push_queue(outputQueue,buffer,sizeof(buffer));
				memset(buffer,0,sizeof(buffer));
				j=0;
			}
			iop=isOperator(current);
			if(iop==1)/*operators*/
			{
				if(k>0)/*case for all other operators (compare precedence)*/
				{
					peepO=peek_stack(operatorStack);
					prevOperator=peepO[0];
					shouldPop=shouldIPop(current,prevOperator);
					while(k>0&&shouldPop==1)
					{
						popper=pop_stack(operatorStack);
						popped[0]=popper[0];
						push_queue(outputQueue,popped,sizeof(popped));
						if(k-1>0){
							peepO=peek_stack(operatorStack);
							prevOperator=peepO[0];
							shouldPop=shouldIPop(current,prevOperator);
						}
						free(popper);
						k--;
					}
					push_stack(operatorStack,currentAsString,sizeof(currentAsString));
					k++;
				}
				else/*first operator*/
				{
					push_stack(operatorStack,currentAsString,sizeof(currentAsString));
					k++;
				}
			}
			else if(current==40)/*Left bracket*/
			{
				push_stack(operatorStack,currentAsString,sizeof(currentAsString));
				k++;
			}
			else if(current==41)/*Right bracket - WORKING*/
			{
				peepO=peek_stack(operatorStack);
				prevOperator=peepO[0];
				while((prevOperator!=40)&&(k>0))
				{
					popper=pop_stack(operatorStack);
					popped[0]=popper[0];
					free(popper);
					push_queue(outputQueue,popped,sizeof(popped));
					peepO=peek_stack(operatorStack);
					prevOperator=peepO[0];
					k--;
				}
				if(prevOperator!=40)
				{
					free_stack(operatorStack);
					free(currentAsString);
					free_queue(outputQueue);
					return NULL;
				}
				else if(prevOperator==40)
				{
					popper=pop_stack(operatorStack);
					free(popper);
					k--;
				}
			}
			else{/*We dont want any non-op symbols or alphabetical characters*/
				free_stack(operatorStack);
				free(currentAsString);
				free_queue(outputQueue);
				return NULL;
			}
		}
		if((i==strlen(expr)-1)&&isN==1)
		{
			push_queue(outputQueue,buffer,sizeof(buffer));
			memset(buffer,0,sizeof(buffer));
			j=0;
		}
	}
	while(k>0)/*Popping remaining ops, ending if brackets mismatch*/
	{
		popper = pop_stack(operatorStack);
		bufOp[0]=popper[0];
		if(popper[0]==40)
		{
			free(popper);
			free_stack(operatorStack);
			free(currentAsString);
			free_queue(outputQueue);
			return NULL;
		}
		free(popper);
		push_queue(outputQueue,bufOp,sizeof(bufOp));
		k--;
	}
	free_stack(operatorStack);
	free(currentAsString);
	return outputQueue;
}
int shouldIPop(char c, char top)
{
	int decider,lefty;
	decider = comparePrecedence(c,top);
	lefty = isLeftAssociative(c);
	if(top==40)
		return 0;
	else if(decider<0)
		return 1;
	else if(decider==0 && lefty == 1)
		return 1;
	else
		return 0;
}
int isOperator(char c)
{
	char arrayOfOps[]={42,43,45,47,94};
	int i;
	for(i=0;i<sizeof(arrayOfOps);i++)
	{
		if(c==arrayOfOps[i])
			return 1;
	}
	return 0;
}
int isLeftAssociative(char c)/*Assuming that c is already an operator*/
{
	if(c==94)
		return 0;
	return 1;
}
int getPrecedence(char c)/*Assuming that c is already an operator*/
{
	if(c==94)
		return 3;
	else if(c==42)
		return 2;
	else if(c==47)
		return 2;
	return 1;
}
int comparePrecedence(char c, char d)/*Assuming that both c and d are already operator*/
{
	int cFirst,dFirst;
	cFirst=getPrecedence(c);
	dFirst=getPrecedence(d);
	if(cFirst>dFirst)
		return 1;
	else if(cFirst<dFirst)
		return -1;
	else
		return 0;
}
int isNumber(char prev, char cur, char next)
{
	int preTrue,nextTrue;
	if((48<=cur)&&(cur<=57))
		return 1;
	preTrue=bisNumber(prev);
	nextTrue=bisNumber(next);
	if((cur==45)||(cur==43))
	{
		if(preTrue==0)/*if a number isnt the previous character, then this will be the start of a signed number*/
			return 1;
		return 0;
	}
	else if(cur==46)
	{
		if((preTrue==1)&&(nextTrue==1))
			return 1;
		return 0;
	}
	return 0;
}
int bisNumber(char c)
{
	if((48<=c)&&(c<=57))
		return 1;
	else if(c==41)
		return 1;
	return 0;
}
/* evaluate the rpn expression given in *queue. */
/* return the value of the evaluated expression. */
/* if an error occurs during evaluation, return silently with HUGE_VAL. */
/* assume a precision of eight decimal places when performing arithmetic. */
double evaluate_rpn(Queue *queue)
{
	/*Queue queue is the result of task3*/
	/*It is FIFO so left to right*/
	Stack *aStack;
	Node *ptr;
	double result;
	double operand1,operand2;
	char *current,*popper,*tof,*tof2,*convRes;
	char pusher[32];
	int isOp;
	aStack=initialise_stack();
	ptr=queue->head;
	while(ptr)
	{
		current=ptr->data;
		isOp=isOperatorString(current);
		if(isOp==1)
		{
			popper=pop_stack(aStack);
			operand2=strtod(popper,&tof);
			free(popper);
			popper=pop_stack(aStack);
			operand1=strtod(popper,&tof2);
			free(popper);
			result = calc(current,operand1,operand2);
			sprintf(pusher,"%.8f",result);
			push_stack(aStack,pusher,sizeof(pusher));
			memset(pusher,0,sizeof(pusher));
		}
		else
		{
			push_stack(aStack,current,sizeof(current));
		}
		ptr=ptr->next;
	}
	popper=pop_stack(aStack);
	result=strtod(popper,&convRes);
	free(popper);
	free(aStack);
	return result;
}
double calc(char *operator, double f1, double f2)/*Already asserted that operator is an operator so no need for sanity*/
{
	char c = operator[0];
	double res;
	if(c==42)
		res = f1*f2;
	else if(c==43)
		res = f1+f2;
	else if(c==45)
		res = f1-f2;
	else if(c==47)
		res = f1/f2;
	else
		res = pow(f1,f2);
	return res;
}
int isOperatorString(char *c)
{
	char arrayOfOps[]={42,43,45,47,94};
	char d = c[0];
	int i;
	if(strlen(c)>1)
		return 0;
	for(i=0;i<sizeof(arrayOfOps);i++)
	{
		if(d==arrayOfOps[i])
			return 1;
	}
	return 0;
}
