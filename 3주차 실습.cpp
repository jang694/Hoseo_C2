#include <stdio.h>
#include <string.h> // ���ڿ� ó���� ���� �߰�

#define NUM_PRODUCTS 5 // ��ǰ�� �� ����

// ��ǰ ������ ����ü
typedef struct {
    int stock;
    int sales;
} Product;

// �Լ� ����
// ���� clearInputBuffer�� �ʿ� �����Ƿ� �����մϴ�.
void restockProducts(Product products[]);
void sellProducts(Product products[]);
void printStatus(const Product products[]);

int main() {
    Product products[NUM_PRODUCTS];
    int menu_choice;
    char line_buffer[100]; // �Է��� ���� ���ڿ� ����

    printf("--- ���θ� ��� ���� ���α׷� (���� ����ȭ Ver.) ---\n");

    // �ʱ� ��� ����
    printf("�� %d�� ��ǰ�� �ʱ� ��� ������� �Է��ϼ��� (��: 10 15 20 25 30): ", NUM_PRODUCTS);
    while (1) {
        if (fgets(line_buffer, sizeof(line_buffer), stdin)) {
            // sscanf�� ����� ���ڿ����� 5���� ���ڸ� �о�ɴϴ�.
            if (sscanf(line_buffer, "%d %d %d %d %d",
                &products[0].stock, &products[1].stock, &products[2].stock,
                &products[3].stock, &products[4].stock) == 5) {
                break; // 5�� ���ڸ� ���������� ������ ���� Ż��
            }
        }
        printf("����: 5���� ���ڸ� �������� �����Ͽ� ��Ȯ�� �Է����ּ���: ");
    }

    for (int i = 0; i < NUM_PRODUCTS; i++) {
        products[i].sales = 0;
    }

    // ���� ����
    while (1) {
        printf("\n���ϴ� �޴��� �����ϼ���.(1. �԰�, 2. �Ǹ�, 3. ��Ȳ��Ȳ, 4. ����)\n> ");

        if (fgets(line_buffer, sizeof(line_buffer), stdin)) {
            if (sscanf(line_buffer, "%d", &menu_choice) != 1) {
                printf("����: �޴� ��ȣ�� ���ڿ��� �մϴ�.\n");
                continue;
            }
        }
        else { continue; } // �Է��� ������ ���

        switch (menu_choice) {
        case 1: restockProducts(products); break;
        case 2: sellProducts(products); break;
        case 3: printStatus(products); break;
        case 4:
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
 * @brief ��ǰ �԰� ó�� �Լ� (fgets/sscanf ������� ���� ����)
 */
void restockProducts(Product products[]) {
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

    if (choice == 1) { // ��ü ��ǰ �԰�
        printf("<<< 1�� �������� ��\n");
        printf("> ��ü ��ǰ�� �԰������ �Է��ϼ��� (%d��) : ", NUM_PRODUCTS);

        if (fgets(line_buffer, sizeof(line_buffer), stdin)) {
            int r[NUM_PRODUCTS];
            if (sscanf(line_buffer, "%d %d %d %d %d", &r[0], &r[1], &r[2], &r[3], &r[4]) == NUM_PRODUCTS) {
                for (int i = 0; i < NUM_PRODUCTS; i++) {
                    products[i].stock += r[i];
                }
                printf("��ü ��ǰ �԰� �Ϸ�Ǿ����ϴ�.\n");
            }
            else {
                printf("\n����: %d���� ���ڸ� ��Ȯ�� �Է��ؾ� �մϴ�. (��: 10 20 30 40 50)\n", NUM_PRODUCTS);
            }
        }

    }
    else if (choice == 2) { // ���� ��ǰ �԰�
        int product_id, restock_qty;
        printf("<<< 2�� �������� ��\n");

        printf("> ��ǰID (1-%d) : ", NUM_PRODUCTS);
        if (fgets(line_buffer, sizeof(line_buffer), stdin)) {
            if (sscanf(line_buffer, "%d", &product_id) != 1) {
                printf("����: ��ǰ ID�� ���ڿ��� �մϴ�.\n");
                return;
            }
        }
        else { return; }

        if (product_id < 1 || product_id > NUM_PRODUCTS) {
            printf("�߸��� ��ǰ ID�Դϴ�.\n");
            return;
        }

        printf("> �԰���� : ");
        if (fgets(line_buffer, sizeof(line_buffer), stdin)) {
            if (sscanf(line_buffer, "%d", &restock_qty) != 1) {
                printf("����: �԰� ������ ���ڿ��� �մϴ�.\n");
                return;
            }
        }
        else { return; }

        products[product_id - 1].stock += restock_qty;
        printf("ID %d ��ǰ �԰� �Ϸ�Ǿ����ϴ�.\n", product_id);
    }
    else {
        printf("�߸��� �����Դϴ�.\n");
    }
}

// �Ǹ� �Լ��� �������� ���� fgets/sscanf ������� ����
void sellProducts(Product products[]) {
    int product_id, sell_qty;
    char line_buffer[100];

    printf("<<< 2. �ǸŸ� �������� ��\n");
    printf("> ��ǰID (1-%d) : ", NUM_PRODUCTS);

    if (fgets(line_buffer, sizeof(line_buffer), stdin)) {
        if (sscanf(line_buffer, "%d", &product_id) != 1) {
            printf("����: ��ǰ ID�� ���ڿ��� �մϴ�.\n"); return;
        }
    }
    else { return; }

    if (product_id < 1 || product_id > NUM_PRODUCTS) {
        printf("�߸��� ��ǰ ID�Դϴ�.\n"); return;
    }

    printf("> �Ǹż��� : ");
    if (fgets(line_buffer, sizeof(line_buffer), stdin)) {
        if (sscanf(line_buffer, "%d", &sell_qty) != 1) {
            printf("����: �Ǹ� ������ ���ڿ��� �մϴ�.\n"); return;
        }
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

// ��Ȳ ��� �Լ��� �Է� �κ��� �����Ƿ� �״���Դϴ�.
void printStatus(const Product products[]) {
    printf("<<< 3. ��Ȳ��Ȳ�� �������� ��\n");
    int total_sales = 0;
    int total_available = 0;
    int max_sales_id = 1, min_sales_id = 1;
    for (int i = 0; i < NUM_PRODUCTS; i++) {
        total_sales += products[i].sales;
        total_available += products[i].stock + products[i].sales;
    }
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
    printf("�� �Ǹŷ� : %d\n", total_sales);
    if (total_available > 0) {
        printf("��ü �Ǹ��� : %.2f%%\n", ((double)total_sales / total_available) * 100.0);
    }
    else {
        printf("��ü �Ǹ��� : 0.00%%\n");
    }
    printf("���� ���� �Ǹŵ� ��ǰ : ID %d, �Ǹŷ� %d\n", max_sales_id, max_sales);
    printf("���� ���� �Ǹŵ� ��ǰ : ID %d, �Ǹŷ� %d\n", min_sales_id, min_sales);
    printf("--- ��ǰ�� ���� ��� ---\n");
    for (int i = 0; i < NUM_PRODUCTS; i++) {
        printf("��ǰ ID %d ��� : %d\n", i + 1, products[i].stock);
    }
}