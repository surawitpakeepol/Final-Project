#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define FILE_NAME "activity.csv"

// ฟังก์ชันตรวจสอบจำนวนวันของเดือน
int getMaxDays(int month) {
    if (month == 2) return 29;
    else if (month == 4 || month == 6 || month == 9 || month == 11) return 30;
    else return 31;
}

// อ่าน ID ล่าสุดจากไฟล์
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

// แสดงข้อมูลทั้งหมด
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

// เพิ่มข้อมูลใหม่
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

// ค้นหาข้อมูล
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

// แก้ไขข้อมูล
void updateRecord() {
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

    printf("\nกรอก ID ที่ต้องการแก้ไข: ");
    scanf("%s", id);

    rewind(fp);
    while (fgets(line, sizeof(line), fp)) {
        char fileId[10], name[100], activity[100], date[20], time[20];
        sscanf(line, "%[^,],%[^,],%[^,],%[^,],%s", fileId, name, activity, date, time);

        if (strcmp(fileId, id) == 0) {
            found = 1;
            int choice;
            printf("เลือกสิ่งที่ต้องการแก้ไข:\n");
            printf("1. ชื่อ\n2. กิจกรรม\n3. วันที่/เดือน/ปี\n4. เวลา\n");
            printf("เลือก: ");
            scanf("%d", &choice);

            if (choice == 1) {
                printf("ชื่อใหม่: ");
                scanf(" %[^\n]", name);
            } else if (choice == 2) {
                printf("กิจกรรมใหม่: ");
                scanf(" %[^\n]", activity);
            } else if (choice == 3) {
                int day, month, year;
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
                sprintf(date, "%02d/%02d/%04d", day, month, year);
            } else if (choice == 4) {
                int hour, minute;
                printf("เวลาใหม่ (ชั่วโมง นาที): ");
                scanf("%d %d", &hour, &minute);
                sprintf(time, "%02d:%02d", hour, minute);
            }

            fprintf(temp, "%s,%s,%s,%s,%s\n", fileId, name, activity, date, time);
        } else {
            fprintf(temp, "%s", line);
        }
    }

    fclose(fp);
    fclose(temp);
    remove(FILE_NAME);
    rename("temp.csv", FILE_NAME);

    if (found)
        printf("แก้ไขข้อมูลเรียบร้อย!\n");
    else
        printf("ไม่พบข้อมูลที่ต้องการแก้ไข!\n");
}

// ลบข้อมูล
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

int main() {
    int choice;
    int running = 1; 

    while (running) {
        printf("\n---------- ระบบจัดการข้อมูลผู้เข้าร่วมกิจกรรม ----------\n");
        printf("1. แสดงข้อมูลทั้งหมด\n");
        printf("2. เพิ่มข้อมูล\n");
        printf("3. ค้นหาข้อมูล\n");
        printf("4. แก้ไขข้อมูล\n");
        printf("5. ลบข้อมูล\n");
        printf("6. ออก\n");
        printf("เลือก: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1: viewAll(); break;
            case 2: addRecord(); break;
            case 3: searchRecord(); break;
            case 4: updateRecord(); break;
            case 5: deleteRecord(); break;
            case 6: 
                printf("ออกจากโปรแกรมแล้วครับ!\n");
                running = 0;
                break;
            default:
                printf("กรุณาเลือกใหม่!\n");
        }
    }
    return 0;
}
