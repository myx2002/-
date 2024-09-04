#include<string>
#include<iostream>
using namespace std;

//5.1a修改
//将宏定义转变为全局变量，便于修改
// #define SIZE 109
int SIZE = 109;
//定义当前bucket个数,到达SIZE的两倍时，SIZE*2
int bucket_now_num = 0;

//program5 未定义
void *checked_malloc(int len);

struct bucket
{
    string key;
    void *binding;
    struct bucket *next;
};

//5.1a修改
struct bucket** table = (struct bucket**)checked_malloc(SIZE * sizeof(struct bucket*));

unsigned int hash_str(char* s0)
{
    unsigned int h = 0;
    char *s;
    for (s = s0; *s; s++)
        h = h * 65599 + *s;
    return h;
}

//5.1a添加，扩大并复制table
struct bucket** copy_table()
{
    int target_size = SIZE * 2;
    struct bucket **new_table = (struct bucket**)checked_malloc(target_size * sizeof(struct bucket*));
    for(int i = 0; i < SIZE; i++)
    {
        struct bucket *b = table[i];
        while(b)
        {
            int index = hash_str((char*)b->key.c_str()) % target_size;
            struct bucket* temp = new_table[index];
            if(temp == NULL)
                new_table[index] = Bucket(b->key, b->binding, NULL);
            else
            {
                while(temp->next != NULL)
                    temp = temp->next;
                temp->next = Bucket(b->key, b->binding, NULL);
            }
            b = b->next;
        }
    }
    return new_table;
}

//建立一个新的bucket并返回
struct bucket *Bucket(string key, void *binding, struct bucket *next)
{
    struct bucket *b = (struct bucket*)checked_malloc(sizeof(*b));
    b->key = key;
    b->binding = binding;
    b->next = next;
    return b;
}

//5.1b修改
//插入新key及映射
void insert(struct bucket** in_table, string key, void *binding)
{
    //5.1a修改
    bucket_now_num++;
    if(bucket_now_num >= SIZE * 2 && in_table == table)
    {
        //修改table大小并拷贝
        table = copy_table();
        //修改SIZE大小
        SIZE *= 2;
    }
    //原始略有错误，略有修改
    int index = hash_str((char*)key.c_str()) % SIZE;
    //新建的bucket放头部
    in_table[index] = Bucket(key, binding, in_table[index]);
}

//5.1b修改
//搜索对映key的binding
void *lookup(struct bucket** look_table, string key)
{
    //原始略有错误，略有修改
    int index = hash_str((char*)key.c_str()) % SIZE;
    struct bucket *b;
    for (b = look_table[index]; b; b = b->next)
        //略有修改
        if (0 == strcmp(b->key.c_str(), key.c_str()))
            return b->binding;
    return NULL;
}

//移除一个key
void pop(string key)
{
    int index = hash_str((char*)key.c_str()) % SIZE;
    table[index] = table[index]->next;
    bucket_now_num--;
}