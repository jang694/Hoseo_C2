#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// --- ���� ���� ---

// ��ǰ ������ ������ ���ϸ�
#define INVENTORY_FILE "inventory.dat"

// �ִ� ���� ������ ��ǰ ����
#define MAX_PRODUCTS 5

// ��ǰ ����ü ����
typedef struct {
    char id[50];         // ��ǰ ID
    char name[100];      // ��ǰ��
    int stock;           // ��� ����
    int price;           // �Ǹ� ����
    int cost;            // �԰� ����
    long long sales_amount; // �� �Ǹűݾ� (ū ���ڸ� ���� long long)
} Product;

// ��ǰ ������ ���� ���� �迭�� ���� ��ǰ ��
Product products[MAX_PRODUCTS];
int product_count = 0;

// --- �Լ� ���� ---
void load_data();
void save_data();
Product* find_product(const char* product_id);
Product* find_product_by_name(const char* product_name); // ��ǰ�� �˻� �Լ� �߰�
void get_string_input(const char* prompt, char* buffer, int buffer_size);
int get_integer_input(const char* prompt);
void handle_receive();
void handle_sell();
void view_individual();
void view_total();
void print_menu();

// --- ������ �ε�/���� ---

/**
 * @brief ���α׷� ���� �� inventory.dat ���Ͽ��� �����͸� �ҷ��ɴϴ�.
 * (���̳ʸ� ���)
 */
void load_data() {
    FILE* f = fopen(INVENTORY_FILE, "rb");
    if (f == NULL) {
        printf("'%s' ������ �����ϴ�. �� ������� �����մϴ�.\n", INVENTORY_FILE);
        product_count = 0;
        return;
    }

    // 1. ����� ��ǰ ����(product_count)�� ���� �о�ɴϴ�.
    if (fread(&product_count, sizeof(int), 1, f) != 1) {
        printf("'%s' ���� �б� ���� (count). �� ������� �����մϴ�.\n", INVENTORY_FILE);
        product_count = 0;
        fclose(f);
        return;
    }

    // 2. ������ �����Ͱ� �ִ�ġ�� �ʰ��ϴ��� Ȯ�� (���� �ջ� ����)
    if (product_count > MAX_PRODUCTS) {
        printf("������ ���� ����: ��ǰ ����(%d)�� �ִ�ġ(%d)�� �ʰ��մϴ�. ����� �ʱ�ȭ�մϴ�.\n", product_count, MAX_PRODUCTS);
        product_count = 0;
        fclose(f);
        return;
    }

    // 3. ��ǰ ������ŭ ��ǰ ������(Product �迭)�� �о�ɴϴ�.
    if (product_count > 0) {
        if (fread(products, sizeof(Product), product_count, f) != product_count) {
            printf("'%s' ���� �б� ���� (data). �� ������� �����մϴ�.\n", INVENTORY_FILE);
            product_count = 0;
        }
        else {
            printf("'%s'���� �����͸� �ҷ��Խ��ϴ�. (��ǰ %d��)\n", INVENTORY_FILE, product_count);
        }
    }
    else {
        printf("'%s'���� �����͸� �ҷ��Խ��ϴ�. (��ǰ 0��)\n", INVENTORY_FILE);
    }

    fclose(f);
}

/**
 * @brief ���α׷� ���� �� products �迭�� inventory.dat ���Ͽ� �����մϴ�.
 * (���̳ʸ� ���)
 */
void save_data() {
    FILE* f = fopen(INVENTORY_FILE, "wb");
    if (f == NULL) {
        printf("������ ���� �� ���� �߻�: ������ �� �� �����ϴ�.\n");
        return;
    }

    // 1. ���� ��ǰ ����(product_count)�� ���� ���ϴ�.
    if (fwrite(&product_count, sizeof(int), 1, f) != 1) {
        printf("������ ���� �� ���� �߻� (count).\n");
        fclose(f);
        return;
    }

    // 2. ��ǰ ������ŭ ��ǰ ������(Product �迭)�� ���ϴ�.
    if (product_count > 0) {
        if (fwrite(products, sizeof(Product), product_count, f) != product_count) {
            printf("������ ���� �� ���� �߻� (data).\n");
        }
        else {
            printf("�����͸� '%s'�� �����߽��ϴ�.\n", INVENTORY_FILE);
        }
    }
    else {
        printf("�����͸� '%s'�� �����߽��ϴ�. (��ǰ 0��)\n", INVENTORY_FILE);
    }

    fclose(f);
}

// --- ��ƿ��Ƽ �Լ� ---

/**
 * @brief ��ǰ ID�� products �迭���� ��ǰ�� ã�� �����͸� ��ȯ�մϴ�.
 * @param product_id �˻��� ��ǰ ID
 * @return ã�� ��� Product ������, �� ã�� ��� NULL
 */
Product* find_product(const char* product_id) {
    for (int i = 0; i < product_count; i++) {
        // C������ ���ڿ� �� �� strcmp ���
        if (strcmp(products[i].id, product_id) == 0) {
            return &products[i]; // �ش� ��ǰ�� �ּ� ��ȯ
        }
    }
    return NULL; // �� ã��
}

/**
 * @brief ��ǰ������ products �迭���� ��ǰ�� ã�� �����͸� ��ȯ�մϴ�. (�ߺ� �˻��)
 * @param product_name �˻��� ��ǰ��
 * @return ã�� ��� Product ������, �� ã�� ��� NULL
 */
Product* find_product_by_name(const char* product_name) {
    for (int i = 0; i < product_count; i++) {
        if (strcmp(products[i].name, product_name) == 0) {
            return &products[i]; // �ش� ��ǰ�� �ּ� ��ȯ
        }
    }
    return NULL; // �� ã��
}


/**
 * @brief ����ڷκ��� �� ���� ���ڿ� �Է��� �����ϰ� �޽��ϴ�. (fgets ���)
 * @param prompt ����ڿ��� ������ �ȳ� �޽���
 * @param buffer �Է¹��� ���ڿ��� ������ ����
 * @param buffer_size ������ ũ��
 */
void get_string_input(const char* prompt, char* buffer, int buffer_size) {
    printf("%s", prompt);
    fflush(stdout); // ������Ʈ�� �ٷ� ���̵��� ���� ����
    fgets(buffer, buffer_size, stdin);

    // fgets�� ���� ���ڿ��� ���� �ִ� ���� ����(\n) ����
    buffer[strcspn(buffer, "\n")] = 0;
}

/**
 * @brief ����ڷκ��� 0 �̻��� ���� �Է��� �����ϰ� �޽��ϴ�.
 * @param prompt ����ڿ��� ������ �ȳ� �޽���
 * @return �Է¹��� 0 �̻��� ����
 */
int get_integer_input(const char* prompt) {
    char buffer[100];
    int value;
    char remaining; // �Է� ���ۿ� ���� ���ڰ� �ִ��� Ȯ���ϱ� ����

    while (1) {
        get_string_input(prompt, buffer, sizeof(buffer));

        // sscanf�� ����� ���ڿ����� ������ �о��
        // %c�� ���� ���ڰ� �ִ��� Ȯ���Ͽ� "123a" ���� �Է��� �ɷ���
        if (sscanf(buffer, "%d%c", &value, &remaining) == 1) {
            if (value < 0) {
                printf("����: 0 �̻��� ���ڸ� �Է��ؾ� �մϴ�.\n");
            }
            else {
                return value; // ����
            }
        }
        else {
            printf("����: ��ȿ�� ���ڸ� �Է��ϼ���.\n");
        }
    }
}

// --- ���� �޴� ��� �Լ� ---

/**
 * @brief 1. �԰� �޴� (�ű�/�߰�) ó���� ����մϴ�.
 */
void handle_receive() {
    char product_id[50];
    printf("\n<< 1. �԰� �޴� ���� >>\n");
    get_string_input("��ǰ ID: ", product_id, sizeof(product_id));

    if (strlen(product_id) == 0) {
        printf("����: ��ǰ ID�� �Է��ؾ� �մϴ�.\n");
        return;
    }

    Product* product = find_product(product_id);

    if (product != NULL) {
        // --- ���� ��ǰ �԰� (������Ʈ) ---
        printf("���� ��ǰ [%s]�� ��� �߰��մϴ�.\n", product->name);

        int qty = get_integer_input("�߰� �԰� ����: ");
        product->stock += qty;

        // �԰� �� ���� ������ ������Ʈ
        printf("(���� �Ǹ� ����: %d)\n", product->price);
        product->price = get_integer_input("�� �Ǹ� ����: ");

        printf("(���� �԰� ����: %d)\n", product->cost);
        product->cost = get_integer_input("�� �԰� ����: ");

        printf("��ǰ [%s]�� ��� %d���� ������Ʈ�Ǿ����ϴ�.\n", product->name, product->stock);

    }
    else {
        // --- �ű� ��ǰ �԰� (����) ---
        if (product_count >= MAX_PRODUCTS) {
            printf("����: �ִ� ��ǰ ����(%d��)�� �����߽��ϴ�. �� ��ǰ�� �߰��� �� �����ϴ�.\n", MAX_PRODUCTS);
            return;
        }

        printf("�ű� ��ǰ�� ����մϴ�.\n");

        Product* new_product = &products[product_count]; // �� ��ǰ�� ����� ��ġ
        char new_name[100]; // �ӽ÷� ��ǰ�� ���� ����

        // ���ڿ� ����
        strcpy(new_product->id, product_id);

        get_string_input("��ǰ��: ", new_name, sizeof(new_name));
        if (strlen(new_name) == 0) {
            printf("����: ��ǰ���� �Է��ؾ� �մϴ�.\n");
            return; // product_count�� �������� �ʵ��� ���⼭ �ߴ�
        }

        // *** ��ǰ�� �ߺ� �˻� �߰� ***
        if (find_product_by_name(new_name) != NULL) {
            printf("����: ������ ��ǰ��('%s')�� �̹� �����մϴ�.\n", new_name);
            return; // �ߺ��̹Ƿ� ��� �ߴ�
        }

        // �ߺ��� �ƴϹǷ� ��ǰ�� ����
        strcpy(new_product->name, new_name);

        new_product->stock = get_integer_input("�ʱ� �԰� ����: ");
        new_product->price = get_integer_input("�Ǹ� ����: ");
        new_product->cost = get_integer_input("�԰� ����: ");
        new_product->sales_amount = 0; // �ű� ��ǰ�̹Ƿ� �� �Ǹűݾ��� 0

        // ��ǰ ����� ���������� �Ϸ�Ǿ����Ƿ�, ��ü ��ǰ ������ 1 ����
        product_count++;

        printf("�ű� ��ǰ [%s]��(��) ��ϵǾ����ϴ�.\n", new_product->name);
    }
}

/**
 * @brief 2. �Ǹ� �޴� ó���� ����մϴ�.
 */
void handle_sell() {
    char product_id[50];
    printf("\n<< 2. �Ǹ� �޴� ���� >>\n");
    get_string_input("��ǰ ID: ", product_id, sizeof(product_id));

    Product* product = find_product(product_id);

    if (product == NULL) {
        printf("����: �ش� ID�� ��ǰ�� �������� �ʽ��ϴ�.\n");
        return;
    }

    int qty = get_integer_input("�Ǹ� ����: ");

    if (qty == 0) {
        printf("�Ǹ� ������ 0�Դϴ�. �۾��� ����մϴ�.\n");
        return;
    }

    if (qty > product->stock) {
        printf("����: ��� �����մϴ�. (���� ���: %d��)\n", product->stock);
    }
    else {
        product->stock -= qty;
        long long sale_value = (long long)product->price * qty;
        product->sales_amount += sale_value;
        printf("��ǰ [%s] %d�� �Ǹ� �Ϸ�.\n", product->name, qty);
        printf("���� ���: %d��\n", product->stock);
        // %lld�� long long int�� ����ϱ� ���� ���� ������
        printf("�ش� ��ǰ �� �Ǹűݾ�: %lld��\n", product->sales_amount);
    }
}

/**
 * @brief 3. ���� ��Ȳ �޴� ó���� ����մϴ�.
 */
void view_individual() {
    char product_id[50];
    printf("\n<< 3. ���� ��Ȳ ���� >>\n");
    get_string_input("��ǰ ID: ", product_id, sizeof(product_id));

    Product* product = find_product(product_id);

    if (product == NULL) {
        printf("����: �ش� ID�� ��ǰ�� �������� �ʽ��ϴ�.\n");
    }
    else {
        printf("--- ��ǰ ���� ��Ȳ ---\n");
        printf("  ��ǰ ID:    %s\n", product->id);
        printf("  ��ǰ��:     %s\n", product->name);
        printf("  ��� ����:  %d ��\n", product->stock);
        printf("  �Ǹ� ����:  %d ��\n", product->price);
        printf("  �԰� ����:  %d ��\n", product->cost);
        printf("  �� �Ǹűݾ�: %lld ��\n", product->sales_amount); // long long
        printf("------------------------\n");
    }
}

/**
 * @brief 4. ��ü ��Ȳ �޴� ó���� ����մϴ�.
 */
void view_total() {
    printf("\n<< 4. ��ü ��Ȳ ���� >>\n");
    if (product_count == 0) {
        printf("��ϵ� ��ǰ�� �����ϴ�.\n");
        return;
    }

    printf("--- ��ǰ ��ü ��Ȳ ---\n");
    long long total_stock_value = 0;
    long long total_sales_amount = 0;

    for (int i = 0; i < product_count; i++) {
        Product* p = &products[i];
        printf("  ID: %s, �̸�: %s, ���: %d, �ǸŰ�: %d, ���Ǹž�: %lld\n",
            p->id, p->name, p->stock, p->price, p->sales_amount);

        total_stock_value += (long long)p->cost * p->stock;
        total_sales_amount += p->sales_amount;
    }

    printf("------------------------\n");
    printf("�� ��ǰ ������: %d ��\n", product_count);
    printf("�� ��� �ڻ� (�԰� ����): %lld ��\n", total_stock_value);
    printf("��� ��ǰ �� �Ǹűݾ� �հ�: %lld ��\n", total_sales_amount);
}

/**
 * @brief ���� �޴��� ����մϴ�.
 */
void print_menu() {
    printf("\n===== ���θ� ��� ���� �ý��� (C ver.) =====\n");
    printf("1. �԰� (�ű�/�߰�)\n");
    printf("2. �Ǹ�\n");
    printf("3. ���� ��Ȳ (ID�˻�)\n");
    printf("4. ��ü ��Ȳ\n");
    printf("5. ���� (�ڵ� ����)\n");
    printf("===========================================\n");
}

/**
 * @brief ���� ���α׷� ������ �����մϴ�.
 */
int main() {
    load_data(); // ���α׷� ���� �� ������ �ε�
    char choice_buffer[10];

    while (1) {
        print_menu();
        get_string_input("���ϴ� �޴��� �����ϼ��� (1-5): ", choice_buffer, sizeof(choice_buffer));

        // �Է��� "1", "2" ó�� �� �������� Ȯ��
        if (strlen(choice_buffer) != 1) {
            printf("\n����: �߸��� �Է��Դϴ�. 1-5 ������ ���� �ϳ��� �Է��ϼ���.\n");
            continue;
        }

        char choice = choice_buffer[0]; // �Էµ� ù��° ����

        switch (choice) {
        case '1':
            handle_receive();
            break;
        case '2':
            handle_sell();
            break;
        case '3':
            view_individual();
            break;
        case '4':
            view_total();
            break;
        case '5':
            save_data(); // ���α׷� ���� �� ������ ����
            printf("\n���α׷��� �����մϴ�. �̿����ּż� �����մϴ�.\n");
            return 0; // ���α׷� ����
        default:
            printf("\n����: �߸��� �Է��Դϴ�. 1-5 ������ ���ڸ� �Է��ϼ���.\n");
            break;
        }
    }

    // ���� �� ���α��� ���� ����
    return 0;
}

