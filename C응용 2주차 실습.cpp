#include <stdio.h>

int main() {
    // --- 입력 부분 ---

    // 1. 상품의 총 개수 입력
    int num_products;
    printf("상품의 총 개수를 입력하세요: ");
    scanf("%d", &num_products);

    // 계산에 필요한 배열 선언
    int incoming_stock[100] = { 0 };
    int sales_quantity[100] = { 0 };
    int inventory[100] = { 0 };

    // 2. 각 상품별 입고수량 입력
    printf("\n--- 각 상품의 입고수량을 순서대로 입력하세요 ---\n");
    for (int i = 0; i < num_products; i++) {
        printf("상품 ID %d 입고수량: ", i + 1);
        scanf("%d", &incoming_stock[i]);
    }

    // 3. 각 상품별 판매수량 입력
    printf("\n--- 각 상품의 판매수량을 순서대로 입력하세요 ---\n");
    for (int i = 0; i < num_products; i++) {
        printf("상품 ID %d 판매수량: ", i + 1);
        scanf("%d", &sales_quantity[i]);
    }

    // --- 계산 및 출력 부분 ---
    printf("\n======== 재고 관리 시스템 결과 ========\n");

    // 요구사항 계산을 위한 변수 초기화
    int total_incoming = 0;
    int total_sales = 0;
    int max_sales = -1;
    int min_sales = 100000; // 충분히 큰 값으로 초기화
    int max_sales_id = 0;
    int min_sales_id = 0;

    // 1. 모든 상품의 재고 수량 계산 및 총합 계산
    printf("재고수량 : ");
    for (int i = 0; i < num_products; i++) {
        // 재고 계산
        inventory[i] = incoming_stock[i] - sales_quantity[i];
        printf("%d ", inventory[i]);

        // 전체 입고량 및 판매량 누적
        total_incoming += incoming_stock[i];
        total_sales += sales_quantity[i];

        // 최대 판매량 상품 찾기
        if (sales_quantity[i] > max_sales) {
            max_sales = sales_quantity[i];
            max_sales_id = i + 1;
        }

        // 최소 판매량 상품 찾기
        if (sales_quantity[i] < min_sales) {
            min_sales = sales_quantity[i];
            min_sales_id = i + 1;
        }
    }
    printf("\n");

    // 2. 전체 판매량 및 판매율 계산 및 출력
    float sales_rate = 0.0;
    if (total_incoming > 0) { // 0으로 나누는 오류 방지
        sales_rate = (float)total_sales / total_incoming * 100;
    }
    printf("총 판매량 : %d (판매율 %.2f%%)\n", total_sales, sales_rate);

    // 3. 최대 및 최소 판매량 상품 출력
    printf("가장 많이 판매된 상품 : ID %d, 판매량 %d\n", max_sales_id, max_sales);
    printf("가장 적게 판매된 상품 : ID %d, 판매량 %d\n", min_sales_id, min_sales);

    // 4. 재고 부족 경고 출력 (재고 2개 이하)
    for (int i = 0; i < num_products; i++) {
        if (inventory[i] <= 2) {
            printf("상품 ID %d : 재고부족(%d)\n", i + 1, inventory[i]);
        }
    }

    return 0;
}
