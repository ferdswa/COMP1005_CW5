#include "sort.h"
#include <stdlib.h>
#define MIN(i,j)(((i)<(j))?(i):(j))
/* sort linked list *list using merge insertion sort. */
/* upon success, the elements in *list will be sorted. */
/* return silently if *list is uninitialised or empty. */
/* the compare argument is a pointer to a function which returns */
/* less than 0, 0, or greater than 0 if first argument is */
/* less than, equal to, or greater than second argument respectively. */
struct storageBin{
    char *string;
};
void merger();
void insertion_sort(LinkedList *list, int (*compare)(void *, void *))
{
    struct storageBin *st;
    Node *ptr;
    char* temp;
    int i=0,j=0;
    int x=0,y=0;
    ptr=list->head;
    while(ptr)
    {
        i++;
        j++;
        ptr=ptr->next;
    }
    st=calloc(i,sizeof(LinkedList));
    ptr=list->head;
    i=0;
    while(ptr)
    {
        st[i].string=ptr->data;
        ptr=ptr->next;
        i++;
    }
    /*Tried the list way for hours - this works so this it is*/
    while(x<j)
    {
        y=x;
        while(y>0&&(compare(st[y-1].string,st[y].string)>0))
        {
            temp=st[y].string;
            st[y].string=st[y-1].string;
            st[y-1].string=temp;
            y--;
        }
        x++;
    }
    for(i=0;i<j;i++)
    {
        remove_tail_linked_list(list);
    }
    for(i=0;i<j;i++)
    {
        append_linked_list(list,st[i].string);
    }
    free(st);
}
/* sort linked list *list using merge sort algorithm. */
/* upon success, the elements in *list will be sorted. */
/* return silently if *list is uninitialised or empty. */
/* the compare argument is a pointer to a function which returns */
/* less than 0, 0, or greater than 0 if first argument is */
/* less than, equal to, or greater than second argument respectively. */
void merge_sort(LinkedList *list, int (*compare)(void *, void *))
{
    struct storageBin *st;
    struct storageBin *workArray;
    Node *ptr;
    int i=0,arlen=0,rWidth;
    int x,y;
    char* plc="test";
    ptr=list->head;
    while(ptr)
    {
        i++;
        arlen++;
        ptr=ptr->next;
    }
    st=calloc(arlen,sizeof(LinkedList));
    workArray=calloc(arlen,sizeof(LinkedList));
    ptr=list->head;
    i=0;
    while(ptr)
    {
        st[i].string=ptr->data;
        ptr=ptr->next;
        i++;
    }
    st[i].string=plc;
    arlen++;
    /*List is now split into runs of size 1*/
    for(rWidth=1;rWidth<arlen;rWidth=2*rWidth)
    {
        for(x=0;x<arlen;x=(x+2*rWidth))
        {
            
            int iL,iR,ctr,res;/*temp ints*/
            int indexLeft,indexRight,indexEnd;
            indexLeft=x;
            indexRight=MIN(x+rWidth,arlen);
            indexEnd=MIN(x+2*rWidth,arlen);
            
            if(indexRight>=arlen)
            {
                indexRight-=1;
            }
            if(indexEnd>=arlen)
            {
                indexEnd-=1;
            }
            iL=indexLeft;
            iR=indexRight;

            for(ctr=indexLeft;ctr<=indexEnd;ctr++)
            {
                res=compare(st[iL].string,st[iR].string);
                if(iL<indexRight&&((res<=0)||(iR>=indexEnd)))
                {
                    workArray[ctr].string=st[iL].string;
                    iL++;
                }
                else
                {
                    workArray[ctr].string=st[iR].string;
                    iR++;
                }
            }
        }
        for(i=0;i<arlen;i++)
        {
            st[i].string=workArray[i].string;
        }
    }
    arlen--;
    for(y=0;y<arlen;y++)
    {
        remove_tail_linked_list(list);
    }
    for(i=0;i<arlen;i++)
    {
        append_linked_list(list,st[i].string);
    }
    free(st);
    free(workArray);
}