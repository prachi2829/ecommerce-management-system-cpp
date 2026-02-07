#include <iostream>
#include <conio.h>
#include <windows.h>
#include <string>
#include <unordered_map>
#include <map>
#include <vector>
#include <queue>
#include <climits>
#define width 100
#define height 50
#define INT_MAX 2147483647

using namespace std;
void gotoxy(int x, int y)
{
    fflush(stdout);
    COORD c;
    c.X = x;
    c.Y = y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), c);
}
       
struct Product
{
    int productNo;
    string name;
    double price;
    Product(int p, string n, double pr) : productNo(p), name(n), price(pr) {}
    Product() : productNo(0), name(""), price(0.0) {}
};

struct Order
{
    static int o_id;
    int cur_order_id;
    vector<Product> products;
    int cust_id;
    string delivery_add;
    Order(vector<Product> products_para, int cust_id_para, string address)
    {
        o_id++;
        cur_order_id = o_id;
        products = products_para;
        cust_id = cust_id_para;
        delivery_add = address;
    }
};

int Order::o_id = 100;
struct Node
{
    Node *left;
    Node *right;
    int h;
    Order obj;
    Node(Order order) : left(nullptr), right(nullptr), h(1), obj(order) {}
};

Node *root;

int get_height(Node *root)
{
    if (root == nullptr)
    {
        return 0;
    }
    return root->h;
}

Node *right_rotate(Node *x)
{
    Node *y = x->left;
    Node *z = y->right;
    x->left = z;
    y->right = x;
    x->h = 1 + max(get_height(x->left), get_height(x->right));
    y->h = 1 + max(get_height(y->left), get_height(y->right));
    return y;
}

Node *left_rotate(Node *y)
{
    Node *x = y->right;
    Node *z = x->left;
    y->right = z;
    x->left = y;
    y->h = 1 + max(get_height(y->left), get_height(y->right));
    x->h = 1 + max(get_height(x->left), get_height(x->right));
    return x;
}

int get_balanced_factor(Node *root)
{
    if (root == nullptr)
    {
        return 0;
    }

    return get_height(root->left) - get_height(root->right);
}

Node *insert_node(Node *root, Order order)
{
    if (root == nullptr)
    {
        Node *newnode = new Node(order);
        return newnode;
    }
    if (order.cur_order_id < root->obj.cur_order_id)
    {
        root->left = insert_node(root->left, order);
    }
    else if (order.cur_order_id > root->obj.cur_order_id)
    {
        root->right = insert_node(root->right, order);
    }

    root->h = max(get_height(root->left), get_height(root->right)) + 1;

    int bf = get_balanced_factor(root);

    if (bf > 1 && order.cur_order_id < root->left->obj.cur_order_id)
    {
        return right_rotate(root);
    }

    else if (bf < -1 && order.cur_order_id > root->right->obj.cur_order_id)
    {
        return left_rotate(root);
    }

    else if (bf > 1 && order.cur_order_id > root->left->obj.cur_order_id)
    {
        root->left = left_rotate(root->left);
        return right_rotate(root);
    }

    else if (bf < -1 && order.cur_order_id < root->right->obj.cur_order_id)
    {
        root->right = right_rotate(root->right);
        return left_rotate(root);
    }

    return root;
}

void print_product_details(vector<Product> product)
{
    int j=22;
    for (int i = 0; i < product.size(); i++)
    {
        gotoxy(50,j);
        cout << product[i].productNo << " " << product[i].name << " " << product[i].price << endl;
        j++;
    }
}

void search(int id, Node *root)
{
    if (root == NULL)
    {
        cout << "Order Id not found" << endl;
    }
    else if (root->obj.cust_id == id)
    {
        print_product_details(root->obj.products);
    }
    else if (id < root->obj.cust_id)
    {
        search(id, root->left);
    }
    else if (id > root->obj.cust_id)
    {
        search(id, root->right);
    }
}

string helper_search(int id, Node *root)
{
    if (root == NULL)
    {
        cout << "Order Id not found" << endl;
        return "";
    }
    else if (root->obj.cur_order_id == id)
    {
        return root->obj.delivery_add;
    }
    else if (id < root->obj.cur_order_id)
    {
        return helper_search(id, root->left);
    }
    else if (id > root->obj.cur_order_id)
    {
        return helper_search(id, root->right);
    }
    return "";
}

void inorder_traversal(Node *root)
{
    if (root)
    {
        inorder_traversal(root->left);
        cout << root->obj.cust_id << endl;
        print_product_details(root->obj.products);
        inorder_traversal(root->right);
    }
    cout << endl;
}

void place_order(map<int, Product> products, int username,string address);

void draw_boundaries()
{
    system("cls");
    for (int i = 0; i <= width - 1; i++)
    {
        gotoxy(i, 0);
        cout << "-";
        gotoxy(i, height);
        cout << "-";
    }
    for (int i = 1; i < height; i++)
    {a
        gotoxy(0, i);
        cout << "|";
        gotoxy(width, i);
        cout << "|";
    }
}

void display_login_screen(int &username, string &address)
{
    draw_boundaries();

   
    string title = "Welcome to E-Commerce Portal";
    gotoxy((width - title.size()) / 2, height / 2 - 5);
    cout << title;

   
    string username_label = "User ID: ";
    string Name="Name:"; 
    string password_label = "Password: ";
    string address_label = "Address:  ";

    gotoxy((width - username_label.size()) / 2 - 5, height / 2);
    cout << username_label;
    gotoxy((width - username_label.size()) / 2 - 5, height / 2+2);
    cout << Name;
    gotoxy((width - password_label.size()) / 2 - 5, height / 2 + 4);
    cout << password_label;
    gotoxy((width - address_label.size()) / 2 - 5, height / 2 + 6);
    cout << address_label;

    // Input fields
    string password;
    string name_id;

    gotoxy((width + username_label.size()) / 2 - 3, height / 2);
    cin >> username;
    gotoxy((width + username_label.size()) / 2 - 3, height / 2+2);
    cin >> name_id;

    gotoxy((width + password_label.size()) / 2 - 3, height / 2 + 4);

    // Capture password input without displaying it
    char ch;
    while ((ch = getch()) != '\r') // '\r' is the Enter key
    {
        if (ch == '\b' && !password.empty()) // Handle backspace
        {
            cout << "\b \b";
            password.pop_back();
        }
        else if (ch != '\b')
        {
            password += ch;
            cout <<"*";
        }
    }

    gotoxy((width + address_label.size()) / 2 - 3, height / 2 + 6);
    cin >> address;

    // Confirm login
    gotoxy((width - 20) / 2, height / 2 + 5);
    cout << "Login Successful!";
}

vector<vector<pair<int, int>>> graph;
void addEdge(int src, int dest, int time)
{
    graph[src].push_back({dest, time});
    graph[dest].push_back({src, time}); // For bidirectional graph
}

void initializeGraph()
{
    int n = 6; 
    graph.resize(n);
    addEdge(0, 1, 4); 
    addEdge(0, 2, 2); 
    addEdge(1, 3, 5); 
    addEdge(2, 3, 8); 
    addEdge(2, 4, 10);
    addEdge(3, 4, 2);  
    addEdge(3, 5, 6);  
}


int dijkstra(int store, int destination, int n)
{
    priority_queue<pair<int,int>,vector<pair<int,int>>,greater<pair<int,int>>>pq;
    vector<int> distances(n, INT_MAX);
    distances[store] = 0;

    // Start from the store node
    pq.push({0, store});

    while (!pq.empty())
    {
        int current_distance = pq.top().first;
        int current_node = pq.top().second;
        pq.pop();

        // Stop if we reach the destination
        if (current_node == destination)
        {
            return current_distance;
        }

        // Check all neighbors of the current node
        for (const auto &neighbor : graph[current_node])
        {
            int neighbor_node = neighbor.first;
            int edge_weight = neighbor.second;
            int new_distance = current_distance + edge_weight;
            if (new_distance < distances[neighbor_node])
            {
                distances[neighbor_node] = new_distance;
                pq.push({new_distance, neighbor_node});
            }
        }
    }

    // If the destination is not reachable
    return -1;
}


int find_city_code(string city)
{
    if (city == "Noida")
        return 1;
    else if (city == "Delhi")
        return 2;
    else if (city == "Mumbai")
        return 3;
    else if (city == "Bangalore")
        return 4;
    else if (city == "Kolkata")
        return 5;
    else return -1;
}

void handle_menu_options(int choice, int username, string address)
{
    switch (choice)
    {
        case 0:
        {
            exit(0);
        }
        case 1:
    {
        map<int, Product> hash_map_of_products;
        hash_map_of_products[1] = Product(1, "Laptop", 44999);
        hash_map_of_products[2] = Product(2, "Smartphone", 11499);
        hash_map_of_products[3] = Product(3, "Headphones", 1179);
        hash_map_of_products[4] = Product(4, "Smartwatch", 1000);
        hash_map_of_products[5] = Product(5, "Tablet", 20000);
        hash_map_of_products[6] = Product(6, "Camera", 32000);
        place_order(hash_map_of_products, username, address);
        // inorder_traversal(root);
        Sleep(10000);
        break;
    }
    case 2:
    {
        system("cls");
        search(username, root);
        Sleep(10000);

        break;
    }
    case 3:
    {
    system("cls"); 
    int store = 0;

    int orderNo;
    cout << "Enter the order ID: ";
    cin >> orderNo;

    string add = helper_search(orderNo, root);

    if (add.empty())
    {
        cout << "Invalid order ID or destination." << endl;
    }
    else
    {
        int destination = find_city_code(add);
        if (destination == -1)
        {
            cout << "Invalid destination city." << endl;
        }
        else
        {
            int estimated_time = dijkstra(store, destination, graph.size());

            if (estimated_time == -1)
            {
                cout << "Destination " << destination << " is not reachable from the store." << endl;
            }
            else
            {
                cout << "Estimated delivery time to destination " << add << ": "
                     << estimated_time << " hours." << endl;
            }
        }
    }
    Sleep(10000); 
        break;
    }
    default:
        break;
    }
}

void render_main_menu(int username, string address)
{
    system("cls");
    int ch;
    gotoxy(30, 5);
    cout << "==============================";
    gotoxy(35, 6);
    cout << "Welcome to eShop";
    gotoxy(30, 7);
    cout << "==============================";

    // Options
    gotoxy(32,9);
    cout<<"[0] To Exit";

    gotoxy(32, 10);
    cout << "[1] Place Order";

    gotoxy(32, 12);
    cout << "[2] View All your past orders";

    gotoxy(32, 14);
    cout << "[3] Find estimated time of Arrival";

    gotoxy(32, 16);
    cout << "Please select an option (1-3): ";
    gotoxy(32, 17);
    cin >> ch;
    handle_menu_options(ch, username, address);
}

void display_catalog(map<int, Product> &products)
{
    system("cls");
    draw_boundaries();
    string title = "Product Catalog";
    gotoxy((width - title.size()) / 2, 2);
    cout << title;

    int box_width = 25;
    int box_height = 5;
    int products_per_row = 3;
    int start_x = 5;
    int start_y = 5;
    int i = 0;
    for (auto it = products.begin(); it != products.end(); ++it, ++i)
    {
        int x = start_x + (i % products_per_row) * (box_width + 5);
        int y = start_y + (i / products_per_row) * (box_height + 3);
        for (int j = 0; j < box_width; j++)
        {
            gotoxy(x + j, y);
            cout << "-";
            gotoxy(x + j, y + box_height);
            cout << "-";
        }
        for (int j = 0; j <= box_height; j++)
        {
            gotoxy(x, y + j);
            cout << "|";
            gotoxy(x + box_width, y + j);
            cout << "|";
        }
        gotoxy(x + 2, y + 1);
        cout << "Product #" << it->second.productNo;
        gotoxy(x + 2, y + 2);
        cout << "Name: " << it->second.name;
        gotoxy(x + 2, y + 3);
        cout << "Price: $" << it->second.price;
    }
}

void place_order(map<int, Product> products, int username, string address)
{
    char ch = 'y';
    vector<Product> products_ordered;
    while (ch == 'Y' || ch == 'y')
    {
        display_catalog(products);
        int choice_of_product;a