#include <stdio.h>
#include <string.h> // strcspn, sprintf �Լ� ����� ���� �߰�

#define NUM_PRODUCTS 5 // ��ǰ�� �� ����
#define LOW_STOCK_THRESHOLD 3 // ��� ���� �˸� ����

// ��ǰ ������ ����ü (�̸� �ʵ� �߰�)
typedef struct {
    int stock;
    int sales;
    char name[51]; // ��ǰ�� ���� ���� (�� ���� ���� 51��)
} Product;

// �Լ� ����
void inputProductNames(Product products[]);
void restockProducts(Product products[]);
void sellProducts(Product products[]);
void printStatus(const Product products[]);

int main() {
    Product products[NUM_PRODUCTS];
    int menu_choice;
    char line_buffer[100]; // �Է��� ���� ���ڿ� ����

    printf("--- ���θ� ��� ���� ���α׷� (Ver. 4) ---\n");

    // ���α׷� ���� �� ��ǰ ������ �ʱ�ȭ
    for (int i = 0; i < NUM_PRODUCTS; i++) {
        sprintf(products[i].name, "��ǰ %d", i + 1); // "��ǰ 1", "��ǰ 2" ... �⺻ �̸� ����
        products[i].sales = 0;
    }

    // �ʱ� ��� ����
    printf("�� %d�� ��ǰ�� �ʱ� ��� ������� �Է��ϼ��� (��: 10 15 20 25 30): ", NUM_PRODUCTS);
    while (1) {
        if (fgets(line_buffer, sizeof(line_buffer), stdin)) {
            if (sscanf(line_buffer, "%d %d %d %d %d",
                &products[0].stock, &products[1].stock, &products[2].stock,
                &products[3].stock, &products[4].stock) == 5) {
                break;
            }
        }
        printf("����: 5���� ���ڸ� �������� �����Ͽ� ��Ȯ�� �Է����ּ���: ");
    }

    // ���� ����
    while (1) {
        printf("\n���ϴ� �޴��� �����ϼ���.(1.�԰�, 2.�Ǹ�, 3.��ǰ�� �Է�, 4.��ǰ��Ȳ, 5.����)\n> ");

        if (fgets(line_buffer, sizeof(line_buffer), stdin)) {
            if (sscanf(line_buffer, "%d", &menu_choice) != 1) {
                printf("����: �޴� ��ȣ�� ���ڿ��� �մϴ�.\n");
                continue;
            }
        }
        else { continue; }

        switch (menu_choice) {
        case 1: restockProducts(products); break;
        case 2: sellProducts(products); break;
        case 3: inputProductNames(products); break; // �ű� ���
        case 4: printStatus(products); break;       // ���� 3�� -> 4��
        case 5:                                     // ���� 4�� -> 5��
            printf("���α׷��� �����մϴ�.\n");
            return 0;
        default:
            printf("�߸��� �޴� ��ȣ�Դϴ�. �ٽ� �Է����ּ���.\n");
            break;
        }
    }
    return 0;
}

/**
 * @brief [�ű�] ��ǰ���� �Է¹޴� �Լ� (�޴� 3)
 */
void inputProductNames(Product products[]) {
    char name_buffer[100];
    printf("<< ��ǰ�� �Է� �޴� ���� >>\n");
    for (int i = 0; i < NUM_PRODUCTS; i++) {
        printf("> ID %d ��ǰ�� : ", i + 1);
        if (fgets(name_buffer, sizeof(name_buffer), stdin)) {
            // fgets�� ���� ���ڿ��� ���� �ִ� ���๮��(\n) ����
            name_buffer[strcspn(name_buffer, "\n")] = 0;
            // ����ü�� �̸� ����
            strcpy(products[i].name, name_buffer);
        }
    }
    printf("��ǰ�� �Է��� �Ϸ�Ǿ����ϴ�.\n");
}

/**
 * @brief ��ǰ �԰� ó�� �Լ� (�޴� 1)
 */
void restockProducts(Product products[]) {
    // ���� ������ ����
    int choice;
    char line_buffer[256];

    printf("> (�Է�)�԰���� �Է� : ��ü ��ǰ �԰� 1, ���� ��ǰ �԰� 2�� ����\n> ");
    if (fgets(line_buffer, sizeof(line_buffer), stdin)) {
        if (sscanf(line_buffer, "%d", &choice) != 1) {
            printf("����: 1 �Ǵ� 2�� ���ڷ� �Է��ؾ� �մϴ�.\n");
            return;
        }
    }
    else { return; }

    if (choice == 1) {
        printf("<<< 1�� �������� ��\n");
        printf("> ��ü ��ǰ�� �԰������ �Է��ϼ��� (%d��) : ", NUM_PRODUCTS);

        if (fgets(line_buffer, sizeof(line_buffer), stdin)) {
            int r[NUM_PRODUCTS];
            if (sscanf(line_buffer, "%d %d %d %d %d", &r[0], &r[1], &r[2], &r[3], &r[4]) == NUM_PRODUCTS) {
                for (int i = 0; i < NUM_PRODUCTS; i++) { products[i].stock += r[i]; }
                printf("��ü ��ǰ �԰� �Ϸ�Ǿ����ϴ�.\n");
            }
            else {
                printf("\n����: %d���� ���ڸ� ��Ȯ�� �Է��ؾ� �մϴ�.\n", NUM_PRODUCTS);
            }
        }
    }
    else if (choice == 2) {
        int product_id, restock_qty;
        printf("<<< 2�� �������� ��\n");
        printf("> ��ǰID (1-%d) : ", NUM_PRODUCTS);
        if (fgets(line_buffer, sizeof(line_buffer), stdin)) {
            if (sscanf(line_buffer, "%d", &product_id) != 1) { printf("����: ��ǰ ID�� ���ڿ��� �մϴ�.\n"); return; }
        }
        else { return; }

        if (product_id < 1 || product_id > NUM_PRODUCTS) { printf("�߸��� ��ǰ ID�Դϴ�.\n"); return; }

        printf("> �԰���� : ");
        if (fgets(line_buffer, sizeof(line_buffer), stdin)) {
            if (sscanf(line_buffer, "%d", &restock_qty) != 1) { printf("����: �԰� ������ ���ڿ��� �մϴ�.\n"); return; }
        }
        else { return; }

        products[product_id - 1].stock += restock_qty;
        printf("ID %d ��ǰ �԰� �Ϸ�Ǿ����ϴ�.\n", product_id);
    }
    else {
        printf("�߸��� �����Դϴ�.\n");
    }
}

/**
 * @brief ��ǰ �Ǹ� ó�� �Լ� (�޴� 2)
 */
void sellProducts(Product products[]) {
    // ���� ������ ����
    int product_id, sell_qty;
    char line_buffer[100];

    printf("<<< 2. �ǸŸ� �������� ��\n");
    printf("> ��ǰID (1-%d) : ", NUM_PRODUCTS);

    if (fgets(line_buffer, sizeof(line_buffer), stdin)) {
        if (sscanf(line_buffer, "%d", &product_id) != 1) { printf("����: ��ǰ ID�� ���ڿ��� �մϴ�.\n"); return; }
    }
    else { return; }

    if (product_id < 1 || product_id > NUM_PRODUCTS) { printf("�߸��� ��ǰ ID�Դϴ�.\n"); return; }

    printf("> �Ǹż��� : ");
    if (fgets(line_buffer, sizeof(line_buffer), stdin)) {
        if (sscanf(line_buffer, "%d", &sell_qty) != 1) { printf("����: �Ǹ� ������ ���ڿ��� �մϴ�.\n"); return; }
    }
    else { return; }

    if (sell_qty > 0 && products[product_id - 1].stock >= sell_qty) {
        products[product_id - 1].stock -= sell_qty;
        products[product_id - 1].sales += sell_qty;
        printf("ID %d ��ǰ %d�� �ǸŰ� �Ϸ�Ǿ����ϴ�.\n", product_id, sell_qty);
    }
    else if (sell_qty <= 0) {
        printf("�Ǹ� ������ 0���� Ŀ�� �մϴ�.\n");
    }
    else {
        printf("��� �����մϴ�. (���� ���: %d��)\n", products[product_id - 1].stock);
    }
}

/**
 * @brief [����] ��Ȳ ��Ȳ ��� �Լ� (�޴� 4)
 */
void printStatus(const Product products[]) {
    printf("<< ��ǰ ��Ȳ �޴� ���� >>\n");

    int total_stock = 0;
    int total_sales = 0;
    int total_available = 0;
    int max_sales_id = 1, min_sales_id = 1;

    for (int i = 0; i < NUM_PRODUCTS; i++) {
        total_stock += products[i].stock;
        total_sales += products[i].sales;
    }
    total_available = total_stock + total_sales;

    int max_sales = products[0].sales;
    int min_sales = products[0].sales;
    for (int i = 1; i < NUM_PRODUCTS; i++) {
        if (products[i].sales > max_sales) {
            max_sales = products[i].sales;
            max_sales_id = i + 1;
        }
        if (products[i].sales < min_sales) {
            min_sales = products[i].sales;
            min_sales_id = i + 1;
        }
    }

    printf("�� ������ : %d\n", total_stock);
    printf("�� �Ǹŷ� : %d (�Ǹ���: %.2f%%)\n", total_sales,
        (total_available > 0) ? ((double)total_sales / total_available) * 100.0 : 0.0);
    printf("���� ���� �Ǹŵ� ��ǰ : ID %d, ��ǰ�� : %s, �Ǹŷ� %d\n", max_sales_id, products[max_sales_id - 1].name, max_sales);
    printf("���� ���� �Ǹŵ� ��ǰ : ID %d, ��ǰ�� : %s, �Ǹŷ� %d\n", min_sales_id, products[min_sales_id - 1].name, min_sales);

    printf("--- ��ǰ�� ��� ��Ȳ ---\n");
    for (int i = 0; i < NUM_PRODUCTS; i++) {
        printf("��ǰ ID %d : %s - ", i + 1, products[i].name);
        if (products[i].stock == 0) {
            printf("ǰ��(0)\n");
        }
        else if (products[i].stock <= LOW_STOCK_THRESHOLD) {
            printf("������(%d)\n", products[i].stock);
        }
        else {
            printf("���(%d)\n", products[i].stock);
        }
    }
}