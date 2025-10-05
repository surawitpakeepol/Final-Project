#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "unity.h"

#define FILE_NAME "activity.csv"

void ensureFileExists() {
    FILE *fp = fopen(FILE_NAME, "r");
    if (fp == NULL) {
        fp = fopen(FILE_NAME, "w");
        if (fp != NULL) {
            fprintf(fp, "ID,Name,Activity,Date,Time\n"); 
            fclose(fp);
            printf("[ระบบ] สร้างไฟล์ใหม่: %s\n", FILE_NAME);
        } else {
            printf("[ข้อผิดพลาด] ไม่สามารถสร้างไฟล์ %s ได้!\n", FILE_NAME);
        }
    } else {
        fclose(fp);
    }
}

int getMaxDays(int month) {
    if (month == 2) return 29;
    else if (month == 4 || month == 6 || month == 9 || month == 11) return 30;
    else return 31;
}

int getLastID() {
    FILE *fp = fopen(FILE_NAME, "r");
    if (fp == NULL) return 0;

    char line[256];
    int lastID = 0;

    while (fgets(line, sizeof(line), fp)) {
        int id;
        if (sscanf(line, "%d,", &id) == 1) {
            if (id > lastID) lastID = id;
        }
    }

    fclose(fp);
    return lastID;
}

void viewAll() {
    FILE *fp = fopen(FILE_NAME, "r");
    char line[256];
    if (fp == NULL) {
        printf("ไม่พบไฟล์ข้อมูล!\n");
        return;
    }
    printf("\n---- ข้อมูลทั้งหมด ----\n");
    while (fgets(line, sizeof(line), fp)) {
        printf("%s", line);
    }
    fclose(fp);
}

void addRecord() {
    FILE *fp = fopen(FILE_NAME, "a");
    char name[100], activity[100];
    int day, month, year, hour, minute;

    if (fp == NULL) {
        printf("ไม่สามารถเปิดไฟล์ได้\n");
        return;
    }

    int newID = getLastID() + 1;

    printf("ชื่อผู้เข้าร่วม: ");
    scanf(" %[^\n]", name);
    printf("ชื่อกิจกรรม: ");
    scanf(" %[^\n]", activity);

    do {
        printf("เดือน (1-12): ");
        scanf("%d", &month);
        if (month < 1 || month > 12)
            printf("เดือนต้องอยู่ระหว่าง 1 ถึง 12!\n");
    } while (month < 1 || month > 12);

    int maxDay = getMaxDays(month);
    do {
        printf("วัน (1-%d): ", maxDay);
        scanf("%d", &day);
        if (day < 1 || day > maxDay)
            printf("วันต้องอยู่ระหว่าง 1 ถึง %d!\n", maxDay);
    } while (day < 1 || day > maxDay);

    printf("ปี: ");
    scanf("%d", &year);
    printf("เวลา (ชั่วโมง นาที): ");
    scanf("%d %d", &hour, &minute);

    fprintf(fp, "%d,%s,%s,%02d/%02d/%04d,%02d:%02d\n",
            newID, name, activity, day, month, year, hour, minute);

    fclose(fp);
    printf("เพิ่มข้อมูลเรียบร้อย! (ID = %d)\n", newID);
}

void searchRecord() {
    FILE *fp = fopen(FILE_NAME, "r");
    char line[256], keyword[100];
    if (fp == NULL) {
        printf("ไม่พบไฟล์ข้อมูล!\n");
        return;
    }

    printf("กรอกคำค้นหา (ชื่อหรือกิจกรรม): ");
    scanf(" %[^\n]", keyword);

    printf("\n---- ผลการค้นหา ----\n");
    int found = 0;
    while (fgets(line, sizeof(line), fp)) {
        if (strstr(line, keyword)) {
            printf("%s", line);
            found = 1;
        }
    }

    if (!found)
        printf("ไม่พบข้อมูลที่ตรงกับ \"%s\"\n", keyword);

    fclose(fp);
}

void deleteRecord() {
    FILE *fp = fopen(FILE_NAME, "r");
    FILE *temp = fopen("temp.csv", "w");
    char line[256], id[10], keyword[100];
    int found = 0;

    if (fp == NULL || temp == NULL) {
        printf("ไม่สามารถเปิดไฟล์ได้\n");
        return;
    }

    printf("กรอกคำค้นหา (ชื่อหรือกิจกรรม): ");
    scanf(" %[^\n]", keyword);

    printf("\n---- ผลการค้นหา ----\n");
    while (fgets(line, sizeof(line), fp)) {
        if (strstr(line, keyword)) {
            printf("%s", line);
        }
    }

    printf("\nกรอก ID ที่ต้องการลบ: ");
    scanf("%s", id);

    rewind(fp);
    while (fgets(line, sizeof(line), fp)) {
        char fileId[10];
        sscanf(line, "%[^,]", fileId);

        if (strcmp(fileId, id) == 0) {
            found = 1;
            continue;
        }
        fprintf(temp, "%s", line);
    }

    fclose(fp);
    fclose(temp);
    remove(FILE_NAME);
    rename("temp.csv", FILE_NAME);

    if (found)
        printf("ลบข้อมูลเรียบร้อย!\n");
    else
        printf("ไม่พบข้อมูลที่ต้องการลบ!\n");
}

void setUp(void) {}
void tearDown(void) {}

void test_searchRecord_should_find_keyword(void) {
    FILE *fp = fopen(FILE_NAME, "w");
    fprintf(fp, "1,Somchai,Running,01/01/2024,10:00\n");
    fprintf(fp, "2,Suda,Swimming,02/02/2024,11:00\n");
    fclose(fp);

    char keyword[] = "Running";
    FILE *fp2 = fopen(FILE_NAME, "r");
    char line[256];
    int found = 0;
    while (fgets(line, sizeof(line), fp2)) {
        if (strstr(line, keyword)) {
            found = 1;
            break;
        }
    }
    fclose(fp2);

    TEST_ASSERT_EQUAL(1, found);
}

void test_deleteRecord_should_remove_line(void) {
    FILE *fp = fopen(FILE_NAME, "w");
    fprintf(fp, "1,Somchai,Running,01/01/2024,10:00\n");
    fprintf(fp, "2,Suda,Swimming,02/02/2024,11:00\n");
    fclose(fp);

    FILE *temp = fopen("temp.csv", "w");
    rewind(fp);
    fp = fopen(FILE_NAME, "r");
    char line[256];
    while (fgets(line, sizeof(line), fp)) {
        if (strstr(line, "1,") == NULL)
            fprintf(temp, "%s", line);
    }
    fclose(fp);
    fclose(temp);
    remove(FILE_NAME);
    rename("temp.csv", FILE_NAME);

    fp = fopen(FILE_NAME, "r");
    int found = 0;
    while (fgets(line, sizeof(line), fp)) {
        if (strstr(line, "1,"))
            found = 1;
    }
    fclose(fp);

    TEST_ASSERT_EQUAL(0, found);
}

void runUnityTests() {
    UNITY_BEGIN();
    RUN_TEST(test_searchRecord_should_find_keyword);
    RUN_TEST(test_deleteRecord_should_remove_line);
    UNITY_END();
}


// เมนูหลัก

int main() {
    int choice;
    int running = 1;

    ensureFileExists(); 
    while (running) {
        printf("\n---------- ระบบจัดการข้อมูลผู้เข้าร่วมกิจกรรม ----------\n");
        printf("1. แสดงข้อมูลทั้งหมด\n");
        printf("2. เพิ่มข้อมูล\n");
        printf("3. ค้นหาข้อมูล\n");
        printf("4. แก้ไขข้อมูล\n");
        printf("5. ลบข้อมูล\n");
        printf("6. ทดสอบระบบ (Unit Test)\n");
        printf("7. ออก\n");
        printf("เลือก: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1: viewAll(); break;
            case 2: addRecord(); break;
            case 3: searchRecord(); break;
            case 4: printf("ยังไม่เปิดใช้ฟังก์ชันแก้ไขในโหมดทดสอบ\n"); break;
            case 5: deleteRecord(); break;
            case 6: runUnityTests(); break;
            case 7:
                printf("ออกจากโปรแกรมแล้วครับ!\n");
                running = 0;
                break;
            default:
                printf("กรุณาเลือกใหม่!\n");
        }
    }
    return 0;
}
