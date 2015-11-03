#include<stdio.h>
#include<string.h>
#include<malloc.h>

#define MAXSIZE 1000000
#define NUM_CHAR 256

struct node {
    int data;
    int weight;
    struct node *lc, *rc;
};
typedef struct node node;

char *codes[NUM_CHAR] = {NULL};
unsigned char output[MAXSIZE];
unsigned char decode[MAXSIZE];

node *buildTree(int [][NUM_CHAR]);
void assignCodes(node *, char *);
void encode(char []);
void decoder(node * ,char []);
void inorder(node *);
void sort_parray(node *[], int , int );
int get_input(char [], int [][NUM_CHAR]);

int main()
{
    unsigned char input[MAXSIZE];
    int freqs[NUM_CHAR][NUM_CHAR] = {{0}},i;
    node *head;
    char pat[NUM_CHAR];
    pat[0] = '\0';
    get_input(input,freqs);
    head = buildTree(freqs);
    //inorder(head);
    assignCodes(head,pat);
    encode(input);
    //printf("%s\n",output);
    decoder(head,output);
    printf("Original text length %ld\n",strlen(input));
    printf("Requires %ld bits. (%ld bytes)\n",strlen(output),strlen(output)/8);
    if(strcmp(input,decode) == 0)
        printf("Restored matches original True\n");
    else
        printf("Restored matches original False\n");
    for(i = 0; i<NUM_CHAR; i++) {
        if(codes[i] != NULL)
            printf("Code for %c %s\n",i,codes[i]);
    }
    return 0;
}
node *buildTree(int freqs[][NUM_CHAR])
{
    int i,k=0, lastindex = 0;
    node *parray[NUM_CHAR], *ptr;

    for(i = 0; i < NUM_CHAR; i++) {

        if(freqs[i][1] != 0) {
            ptr = (node *)malloc(sizeof(node));
            ptr->weight = freqs[i][1];
            ptr->data = freqs[i][0];
            ptr->lc = NULL;
            ptr->rc = NULL;
            parray[k++] = ptr;
            lastindex++;
        }
    }
    i = 0;
    sort_parray(parray,i,lastindex);
    while(i < lastindex-1) {
        sort_parray(parray,i,lastindex);
        ptr = (node *)malloc(sizeof(node));
        ptr->data = '\0';
        ptr->lc = parray[i];
        ptr->rc = parray[i+1];
        ptr->weight = (ptr->lc)->weight + (ptr->rc)->weight;
        parray[i+1] = ptr;
        i++;
    }
    return parray[lastindex-1];
}

void sort_parray(node *parray[], int start, int end)
{
    int i,j;
    node *temp;
    for(i = start; i < end; i++) {
        for(j = i; j < end; j++) {
            if(parray[j]->weight < parray[i]->weight) {
                temp = parray[j];
                parray[j] = parray[i];
                parray[i] = temp;
            }
            else if(parray[j]->weight == parray[i]->weight) {

                if(parray[j]->data < parray[i]->data) {
                    temp = parray[j];
                    parray[j] = parray[i];
                    parray[i] = temp;

                }
            }
        }
    }
}
void assignCodes(node *nod, char *pat)
{
    char *s;
    char temp[NUM_CHAR];
    temp[0] = '\0';
    if((nod->data)!= '\0') {
        s = (char *)malloc(strlen(pat)*sizeof(char));
        strcpy(s,pat);
        codes[nod->data] = s;

    }
    else {
        strcpy(temp,pat);
        assignCodes(nod->lc, strcat(pat,"0"));
        assignCodes(nod->rc, strcat(temp,"1"));
    }
}

void encode(char input[])
{
    int i = 0;
    output[0] = '\0';
    while(input[i] != '\0')
    {
        strcat(output,codes[input[i]]);
        i++;
    }
}

void decoder(node *head,char input[])
{
    int i,k =0;
    node *p = head;
    decode[0] = '\0';
    for(i =0; input[i] != '\0'; i++)
    {
        if(input[i] == '0')
            p = p->lc;
        else
            p = p->rc;
        if(p->data != '\0') {
            decode[k++] = p->data;
            p = head;
        }
    }
}

void inorder(node *ptr)
{
    if(ptr != NULL) {
        inorder(ptr->lc);
        printf("\'%c\' %d\n",ptr->data,ptr->weight);
        inorder(ptr->rc);
    }
}

int get_input(char input[MAXSIZE], int freqs[][NUM_CHAR])
{
    char c;
    int i = 0;
    while((c = getchar()) != EOF) {
        input[i++] = c;
        freqs[c][1] = freqs[c][1] + 1;
        freqs[c][0] = c;

    }
    input[i] = '\0';
    return i;
}

