#include"tree.hh"

node* create_node(string type, Kind kind, int value, string name, node* left, node* right)
{
    node* new_node = new node;
    if(new_node == NULL)
    {
        cout << "Create node failed" << endl;
        exit(0);
    }
    new_node->type = type;
    new_node->kind = kind;
    new_node->value = value;
    new_node->name = name;
    new_node->left = left;
    new_node->right = right;

    return new_node;
}
/*root即为start节点*/
void tree_print(node* t_root, int level)
{
    if(t_root == NULL)
        return;
    //kind变量待会再说
    for(int i = 0; i < level; i++)
        cout << "  ";
    cout << t_root->type << " "<< t_root->lineno;

    if(t_root->kind == NOE_k)
        cout <<  endl;
    else if(t_root->kind == STR_k)
        cout << " STR_k " << t_root->name << endl;
    else if(t_root->kind == NUM_k)
        cout << " NUM_k " << t_root->value << endl;
    
    tree_print(t_root->left, level + 1);
    tree_print(t_root->right, level);
}

void def_right(node* origin, node* value)
{
    node* temp = origin;
    while(temp->right != NULL)
        temp = temp->right;
    temp->right = value;
}

int base_to_dec(int oct, int base)
{
    int dec = 0;
    int temp_base = 1;
    while(oct)
    {
        dec += (oct % 10) * temp_base;
        oct /= 10;
        temp_base *= base;
    }
    return dec;
} 