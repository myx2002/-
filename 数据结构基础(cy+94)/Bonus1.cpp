/*Author: ¬Ì”Ó–•  ID:3210104530   NO.02*/
#include<stdio.h>
#include<stdlib.h>
#define max 1000000000000

int cmp_longlongint(const void* index1, const void* index2)
{
    if (*(long long int*)index2 - *(long long int*)index1 > 0)
        return -1;
    else if (*(long long int*)index2 - *(long long int*)index1 < 0)
        return 1;
}

int cmp_longint(const void* index1, const void* index2)
{
    return  *(long int*)index2 - *(long int*)index1;
}

typedef struct input {
    char ch;
    long long int card;
    int score;
    int site;
    int date;
    int test;
    struct input* next;
}node;
int main()
{
    int total, request;
    int index, in, i, j;
    long long int record;
    int flag = 0;
    char deal;
    node* p;
    node* q;
    node* head;
    p = (node*)malloc(sizeof(struct input));
    p->next = NULL;
    head = p;
    scanf("%d %d", &total, &request);
    int array[request][2];//request
    int find1[total][2];//total
    long int find[total];//total
    long long int findplus[total];//total
    for (index = 0;index < total;index++)
    {
        q = (node*)malloc(sizeof(struct input));
        getchar();
        scanf("%c", &(q->ch));
        scanf("%lld", &(q->card));
        scanf("%d", &(q->score));
        q->site = q->card / 1000000000;
        q->date = (q->card / 1000) % 1000000;
        q->test = q->card % 1000;
        q->next = NULL;
        p->next = q;
        p = q;
    }
    for (index = 0;index < request;index++)
    {
        in = 0;
        scanf("%d", &array[index][0]);
        if (array[index][0] == 1)
        {
            getchar();
            scanf("%c", &deal);
            getchar();
            printf("Case %d: %d %c\n", index + 1, array[index][0], deal);
            p = head;
            p = p->next;
            findplus[0] = 0;
            while (p != NULL)
            {
                if (p->ch == deal)
                {
                    findplus[in] = (100 - p->score) * max;
                    findplus[in] += p->card;
                    in++;
                }
                p = p->next;
            }
            qsort(findplus, in, sizeof(findplus[0]), cmp_longlongint);

            if (findplus[0] == 0)
                printf("NA\n");
            else
                for (i = 0;i < in;i++)
                    printf("%c%lld %d\n", deal, findplus[i] % max, 100 - findplus[i] / max);
        }
        else if (array[index][0] == 2)
        {
            scanf("%d", &array[index][1]);
            printf("Case %d: %d %03d\n", index + 1, array[index][0], array[index][1]);
            record = array[index][1];
            p = head;
            p = p->next;
            find1[0][0] = 0;
            find1[0][1] = 0;
            while (p != NULL)
            {
                if (p->site == record)
                {
                    find1[0][0] += 1;
                    find1[0][1] += p->score;
                }
                p = p->next;
            }
            if (find1[0][0] == 0)
                printf("NA\n");
            else
                printf("%d %d\n", find1[0][0], find1[0][1]);
        }
        else
        {
            scanf("%d", &array[index][1]);
            printf("Case %d: %d %06d\n", index + 1, array[index][0], array[index][1]);
            record = array[index][1];
            p = head;
            p = p->next;
            find[0] = 0;
            while (p != NULL)
            {
                if (p->date == record)
                {
                    flag = 0;
                    for (i = 0;i < in;i++)
                    {
                        if (1000 - p->site == find[i] % 1000)
                        {
                            find[i] = find[i] + 10000;
                            flag = 1;
                            break;
                        }
                    }
                    if (flag == 0)
                    {
                        find[in] = 1;
                        find[in] = find[in] * 10000 + 1000 - p->site;
                        in++;
                    }
                }
                p = p->next;
            }
            qsort(find, in, sizeof(find[0]), cmp_longint);
            if (find[0] == 0)
                printf("NA\n");
            else
                for (i = 0;i < in;i++)
                    printf("%03d %d\n", 1000 - find[i] % 1000, find[i] / 10000);
        }
    }
    return 0;
}