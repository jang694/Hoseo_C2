#include <stdio.h>

int main() {
    // 변수 선언 및 배열 0으로 초기화
    int num_products;
    int stock_in[100] = { 0 };
    int sales[100] = { 0 };
    int query_id;
    int total_inventory = 0;

    // 프로그램이 입력을 기다리고 있음을 알려주는 안내 메시지 (선택 사항)
    // printf("상품 종류 개수를 입력하세요: "); 
    scanf("%d", &num_products);

    // printf("상품별 입고 수량을 입력하세요: ");
    for (int i = 0; i < num_products; i++) {
        scanf("%d", &stock_in[i]);
    }

    // printf("상품별 판매 수량을 입력하세요: ");
    for (int i = 0; i < num_products; i++) {
        scanf("%d", &sales[i]);
    }

    // printf("조회할 상품 ID를 입력하세요: ");
    scanf("%d", &query_id);

    // ID에 해당하는 상품 재고 계산 및 출력
    int specific_inventory = stock_in[query_id - 1] - sales[query_id - 1];
    printf("%d\n", specific_inventory);

    // 전체 재고 계산 및 출력
    for (int i = 0; i < num_products; i++) {
        total_inventory += (stock_in[i] - sales[i]);
    }
    printf("%d\n", total_inventory);

    return 0;
}