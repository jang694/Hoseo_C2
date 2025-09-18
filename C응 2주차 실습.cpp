#include <stdio.h>

int main() {
    // --- �Է� �κ� ---

    // 1. ��ǰ�� �� ���� �Է�
    int num_products;
    printf("��ǰ�� �� ������ �Է��ϼ���: ");
    scanf("%d", &num_products);

    // ��꿡 �ʿ��� �迭 ����
    int incoming_stock[100] = { 0 };
    int sales_quantity[100] = { 0 };
    int inventory[100] = { 0 };

    // 2. �� ��ǰ�� �԰���� �Է�
    printf("\n--- �� ��ǰ�� �԰������ ������� �Է��ϼ��� ---\n");
    for (int i = 0; i < num_products; i++) {
        printf("��ǰ ID %d �԰����: ", i + 1);
        scanf("%d", &incoming_stock[i]);
    }

    // 3. �� ��ǰ�� �Ǹż��� �Է�
    printf("\n--- �� ��ǰ�� �Ǹż����� ������� �Է��ϼ��� ---\n");
    for (int i = 0; i < num_products; i++) {
        printf("��ǰ ID %d �Ǹż���: ", i + 1);
        scanf("%d", &sales_quantity[i]);
    }

    // --- ��� �� ��� �κ� ---
    printf("\n======== ��� ���� �ý��� ��� ========\n");

    // �䱸���� ����� ���� ���� �ʱ�ȭ
    int total_incoming = 0;
    int total_sales = 0;
    int max_sales = -1;
    int min_sales = 100000; // ����� ū ������ �ʱ�ȭ
    int max_sales_id = 0;
    int min_sales_id = 0;

    // 1. ��� ��ǰ�� ��� ���� ��� �� ���� ���
    printf("������ : ");
    for (int i = 0; i < num_products; i++) {
        // ��� ���
        inventory[i] = incoming_stock[i] - sales_quantity[i];
        printf("%d ", inventory[i]);

        // ��ü �԰� �� �Ǹŷ� ����
        total_incoming += incoming_stock[i];
        total_sales += sales_quantity[i];

        // �ִ� �Ǹŷ� ��ǰ ã��
        if (sales_quantity[i] > max_sales) {
            max_sales = sales_quantity[i];
            max_sales_id = i + 1;
        }

        // �ּ� �Ǹŷ� ��ǰ ã��
        if (sales_quantity[i] < min_sales) {
            min_sales = sales_quantity[i];
            min_sales_id = i + 1;
        }
    }
    printf("\n");

    // 2. ��ü �Ǹŷ� �� �Ǹ��� ��� �� ���
    float sales_rate = 0.0;
    if (total_incoming > 0) { // 0���� ������ ���� ����
        sales_rate = (float)total_sales / total_incoming * 100;
    }
    printf("�� �Ǹŷ� : %d (�Ǹ��� %.2f%%)\n", total_sales, sales_rate);

    // 3. �ִ� �� �ּ� �Ǹŷ� ��ǰ ���
    printf("���� ���� �Ǹŵ� ��ǰ : ID %d, �Ǹŷ� %d\n", max_sales_id, max_sales);
    printf("���� ���� �Ǹŵ� ��ǰ : ID %d, �Ǹŷ� %d\n", min_sales_id, min_sales);

    // 4. ��� ���� ��� ��� (��� 2�� ����)
    for (int i = 0; i < num_products; i++) {
        if (inventory[i] <= 2) {
            printf("��ǰ ID %d : ������(%d)\n", i + 1, inventory[i]);
        }
    }

    return 0;
}