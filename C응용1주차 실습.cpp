#include <stdio.h>

int main() {
    // ���� ���� �� �迭 0���� �ʱ�ȭ
    int num_products;
    int stock_in[100] = { 0 };
    int sales[100] = { 0 };
    int query_id;
    int total_inventory = 0;

    // ���α׷��� �Է��� ��ٸ��� ������ �˷��ִ� �ȳ� �޽��� (���� ����)
    // printf("��ǰ ���� ������ �Է��ϼ���: "); 
    scanf("%d", &num_products);

    // printf("��ǰ�� �԰� ������ �Է��ϼ���: ");
    for (int i = 0; i < num_products; i++) {
        scanf("%d", &stock_in[i]);
    }

    // printf("��ǰ�� �Ǹ� ������ �Է��ϼ���: ");
    for (int i = 0; i < num_products; i++) {
        scanf("%d", &sales[i]);
    }

    // printf("��ȸ�� ��ǰ ID�� �Է��ϼ���: ");
    scanf("%d", &query_id);

    // ID�� �ش��ϴ� ��ǰ ��� ��� �� ���
    int specific_inventory = stock_in[query_id - 1] - sales[query_id - 1];
    printf("%d\n", specific_inventory);

    // ��ü ��� ��� �� ���
    for (int i = 0; i < num_products; i++) {
        total_inventory += (stock_in[i] - sales[i]);
    }
    printf("%d\n", total_inventory);

    return 0;
}