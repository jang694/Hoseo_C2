#include <stdio.h>
#include <string.h>

// ==========================================================
// ✨ 바로 이 부분에서 최대 상품 개수를 5개로 고정합니다. ✨
#define MAX_PRODUCTS 5
// ==========================================================
#define MAX_NAME_LENGTH 50

// 상품 정보를 담는 구조체 정의
typedef struct {
    int id;
    char name[MAX_NAME_LENGTH];
    int price;
    int stock_in_total;
    int sales_total;
} Product;

// 전역 변수
Product inventory[MAX_PRODUCTS];
int product_count = 0;

// 함수 프로토타입 선언
void display_menu();
int find_product_index(int id);
void stock_in_product();
void sell_product();
void display_product_details();
void display_all_products();
int is_name_duplicated(const char* name);
void clear_input_buffer();

int main() {
    int choice = 0;
    do {
        display_menu();
        printf(">> ");

        if (scanf("%d", &choice) != 1) {
            printf(">> 잘못된 입력입니다. 숫자를 입력해주세요.\n");
            clear_input_buffer();
            continue;
        }

        clear_input_buffer();

        switch (choice) {
        case 1: stock_in_product(); break;
        case 2: sell_product(); break;
        case 3: display_product_details(); break;
        case 4: display_all_products(); break;
        case 5: printf("프로그램을 종료합니다.\n"); break;
        default: printf("잘못된 선택입니다. 다시 입력해주세요.\n"); break;
        }
        printf("\n");
    } while (choice != 5);

    return 0;
}

void display_menu() {
    printf("===========================================\n");
    printf("원하는 메뉴를 선택하세요.\n");
    printf("1. 입고  2. 판매  3. 개별상품정보  4. 전체상품정보  5. 종료\n");
    printf("===========================================\n");
}

int find_product_index(int id) {
    for (int i = 0; i < product_count; i++) {
        if (inventory[i].id == id) return i;
    }
    return -1;
}

int is_name_duplicated(const char* name) {
    for (int i = 0; i < product_count; i++) {
        if (strcmp(inventory[i].name, name) == 0) return 1;
    }
    return 0;
}

void clear_input_buffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

void stock_in_product() {
    int id, stock_quantity, price;
    char name[MAX_NAME_LENGTH];

    printf("입고처리 실행\n");
    printf("상품ID : ");

    if (scanf("%d", &id) != 1) {
        printf(">> 상품 ID는 숫자로 입력해야 합니다.\n");
        clear_input_buffer();
        return;
    }
    clear_input_buffer();

    int index = find_product_index(id);

    if (index != -1) { // 기존 상품
        printf("입고량 : ");
        if (scanf("%d", &stock_quantity) != 1) {
            printf(">> 입고량은 숫자로 입력해야 합니다.\n");
            clear_input_buffer();
            return;
        }
        clear_input_buffer();
        inventory[index].stock_in_total += stock_quantity;
        printf(">> 기존 상품에 재고가 추가되었습니다.\n");

    }
    else { // 신규 상품
        // ==========================================================
        // ✨ 여기서 현재 상품 수가 5개인지 확인하여 추가를 막습니다. ✨
        if (product_count >= MAX_PRODUCTS) {
            printf(">> 더 이상 상품을 추가할 수 없습니다. (최대 5개)\n");
            return;
        }
        // ==========================================================

        printf("<< 신규 상품입니다. 정보를 입력하세요. >>\n");

        while (1) {
            printf("상품명 : ");
            if (fgets(name, sizeof(name), stdin) == NULL) {
                printf(">> 입력 오류가 발생했습니다.\n");
                return;
            }
            name[strcspn(name, "\n")] = 0;

            if (is_name_duplicated(name)) {
                printf(">> 이미 존재하는 상품명입니다. 다시 입력해주세요.\n");
            }
            else {
                break;
            }
        }

        printf("판매가격 : ");
        if (scanf("%d", &price) != 1) {
            printf(">> 가격은 숫자로 입력해야 합니다.\n");
            clear_input_buffer();
            return;
        }
        clear_input_buffer();

        printf("입고량 : ");
        if (scanf("%d", &stock_quantity) != 1) {
            printf(">> 입고량은 숫자로 입력해야 합니다.\n");
            clear_input_buffer();
            return;
        }
        clear_input_buffer();

        inventory[product_count].id = id;
        strcpy(inventory[product_count].name, name);
        inventory[product_count].price = price;
        inventory[product_count].stock_in_total = stock_quantity;
        inventory[product_count].sales_total = 0;
        product_count++;
        printf(">> 신규 상품이 등록되었습니다.\n");
    }
}

void sell_product() {
    int id, sales_quantity;
    printf("판매처리 실행\n");
    printf("상품ID : ");
    if (scanf("%d", &id) != 1) {
        printf(">> 상품 ID는 숫자로 입력해야 합니다.\n");
        clear_input_buffer();
        return;
    }
    clear_input_buffer();

    int index = find_product_index(id);
    if (index == -1) {
        printf(">> ID에 해당하는 상품이 없습니다.\n");
    }
    else {
        int current_stock = inventory[index].stock_in_total - inventory[index].sales_total;
        printf("판매량 : ");
        if (scanf("%d", &sales_quantity) != 1) {
            printf(">> 판매량은 숫자로 입력해야 합니다.\n");
            clear_input_buffer();
            return;
        }
        clear_input_buffer();

        if (sales_quantity <= 0) {
            printf(">> 판매량은 1 이상이어야 합니다.\n");
        }
        else if (sales_quantity > current_stock) {
            printf(">> 재고가 부족합니다. (현재 재고: %d개)\n", current_stock);
        }
        else {
            inventory[index].sales_total += sales_quantity;
            printf(">> 판매가 완료되었습니다.\n");
        }
    }
}

void display_product_details() {
    int id;
    printf("개별상품정보 실행\n");
    printf("상품ID : ");
    if (scanf("%d", &id) != 1) {
        printf(">> 상품 ID는 숫자로 입력해야 합니다.\n");
        clear_input_buffer();
        return;
    }
    clear_input_buffer();

    int index = find_product_index(id);
    if (index == -1) {
        printf(">> ID에 해당하는 상품이 없습니다.\n");
    }
    else {
        printf("\n--- [%s] 상품 정보 ---\n", inventory[index].name);
        printf("상품 ID    : %d\n", inventory[index].id);
        printf("상품 가격  : %d원\n", inventory[index].price);
        printf("현재 재고  : %d개\n", inventory[index].stock_in_total - inventory[index].sales_total);
        printf("총 판매량  : %d개\n", inventory[index].sales_total);
        printf("총 판매금액: %lld원\n", (long long)inventory[index].price * inventory[index].sales_total);
        printf("--------------------------\n");
    }
}

void display_all_products() {
    printf("전체상품정보 실행\n");
    if (product_count == 0) {
        printf(">> 등록된 상품이 없습니다.\n");
        return;
    }
    printf("=============================================================================\n");
    printf("%-8s %-20s %-10s %-10s %-10s %-15s\n", "상품ID", "상품명", "가격", "현재입고량", "현재판매량", "총판매금액");
    printf("-----------------------------------------------------------------------------\n");

    for (int i = 0; i < product_count; i++) {
        int current_stock = inventory[i].stock_in_total - inventory[i].sales_total;
        long long total_sales_amount = (long long)inventory[i].price * inventory[i].sales_total;
        printf("%-8d %-20s %-10d %-10d %-10d %-15lld\n",
            inventory[i].id, inventory[i].name, inventory[i].price,
            current_stock, inventory[i].sales_total, total_sales_amount);
    }
    printf("=============================================================================\n");
}
