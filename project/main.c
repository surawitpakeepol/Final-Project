#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#define FILE_NAME "activity.csv"
int isValidName(const char *name) {
    for (int i = 0; i < strlen(name); i++) {
        unsigned char c = name[i];
        
        if (!(isalpha(c) || (c >= 0xC0 && c <= 0xFF) || c == ' ')) {
            return 0;
        }
    }
    return 1;
}

char* strcasestr_custom(const char *haystack, const char *needle) {
    if (!*needle) return (char*)haystack;
    for (; *haystack; haystack++) {
        const char *h = haystack;
        const char *n = needle;
        while (*h && *n && tolower(*h) == tolower(*n)) { h++; n++; }
        if (!*n) return (char*)haystack;
    }
    return NULL;
}


void checkCSV() {
    FILE *fp = fopen(FILE_NAME, "r");
    if (fp == NULL) { 
        fp = fopen(FILE_NAME, "w");
        if (fp == NULL) {
            printf("❌ ไม่สามารถสร้างไฟล์ %s ได้\n", FILE_NAME);
            return;
        }


        fprintf(fp, "ID,ชื่อผู้เข้าร่วม,ชื่อกิจกรรม,วันที่เข้าร่วม,เวลา,สถานะ\n");
        fclose(fp);
        printf("✅ สร้างไฟล์ %s เรียบร้อย \n", FILE_NAME);
    } else {
        fclose(fp); 
    }
}


// ตรวจสอบจำนวนวันของเดือน
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
    if (fp == NULL) {
        printf("ไม่พบไฟล์ข้อมูล!\n");
        return;
    }

    char line[256];
    int headerPrinted = 0;
    int found = 0;

    printf("\n--------------------------- รายชื่อผู้เข้าร่วมกิจกรรม ---------------------------\n");

   
    fgets(line, sizeof(line), fp);

    while (fgets(line, sizeof(line), fp)) {
        if (strlen(line) < 3) continue;
        line[strcspn(line, "\n")] = '\0'; 

        char *token;
        char *fields[6]; 
        int i = 0;

        token = strtok(line, ",");
        while (token != NULL && i < 6) {
            fields[i++] = token;
            token = strtok(NULL, ",");
        }

  
        if (i == 6 && strcmp(fields[5], "deleted") != 0) {
            if (!headerPrinted) {
                printf("-------------------------------------------------------------------------------\n");
                printf("| %-3s | %-15s | %-15s | %-15s | %-5s |\n", 
                       "ID", "ชื่อผู้เข้าร่วม", "ชื่อกิจกรรม", "วันที่เข้าร่วม", "เวลา");
                printf("-------------------------------------------------------------------------------\n");
                headerPrinted = 1;
            }
            printf("| %-3s | %-15s | %-15s | %-15s | %-5s |\n",
                   fields[0], fields[1], fields[2], fields[3], fields[4]);
            found = 1;
        }
    }

    if (!found)
        printf("ไม่พบข้อมูลในไฟล์ครับ\n");
    else
        printf("-------------------------------------------------------------------------------\n");

    fclose(fp);
}



// เพิ่มข้อมูลใหม่ 
void addRecord() {
    FILE *fp = fopen(FILE_NAME, "a");
    char name[100], activity[100];
    int day, month, year, hour, minute;
    char buffer[100];

    if (fp == NULL) {
        printf("ไม่สามารถเปิดไฟล์ได้\n");
        return;
    }

    int newID = getLastID() + 1;

    // ล้าง stdin ก่อน
    int c;
    while ((c = getchar()) != '\n' && c != EOF);

    // ชื่อผู้เข้าร่วม
    printf("ชื่อผู้เข้าร่วม: ");
    fgets(name, sizeof(name), stdin);
    name[strcspn(name, "\n")] = 0; 

    // ชื่อกิจกรรม
    printf("ชื่อกิจกรรม: ");
    fgets(activity, sizeof(activity), stdin);
    activity[strcspn(activity, "\n")] = 0;

    // เดือน
    while (1) {
        printf("เดือน (1-12): ");
        fgets(buffer, sizeof(buffer), stdin);
        if (sscanf(buffer, "%d", &month) != 1 || month < 1 || month > 12) {
            printf("⛔ กรุณาใส่ตัวเลข 1-12 เท่านั้น!\n");
        } else break;
    }

    
    int maxDay = getMaxDays(month);
    while (1) {
        printf("วัน (1-%d): ", maxDay);
        fgets(buffer, sizeof(buffer), stdin);
        if (sscanf(buffer, "%d", &day) != 1 || day < 1 || day > maxDay) {
            printf("⛔ กรุณาใส่ตัวเลข 1-%d เท่านั้น!\n", maxDay);
        } else break;
    }

    
    while (1) {
        printf("ปี: ");
        fgets(buffer, sizeof(buffer), stdin);
        if (sscanf(buffer, "%d", &year) != 1 || year < 1900 || year > 2025) {
            printf("⛔ กรุณาใส่ตัวเลขปีที่ถูกต้อง!\n");
        } else break;
    }

    
    while (1) {
        printf("ชั่วโมง (0-23): ");
        fgets(buffer, sizeof(buffer), stdin);
        if (sscanf(buffer, "%d", &hour) != 1 || hour < 0 || hour > 23) {
            printf("⛔ กรุณาใส่ตัวเลข 0-23 เท่านั้น!\n");
        } else break;
    }

    
    while (1) {
        printf("นาที (0-59): ");
        fgets(buffer, sizeof(buffer), stdin);
        if (sscanf(buffer, "%d", &minute) != 1 || minute < 0 || minute > 59) {
            printf("⛔ กรุณาใส่ตัวเลข 0-59 เท่านั้น!\n");
        } else break;
    }

    
    fprintf(fp, "%d,%s,%s,%02d/%02d/%04d,%02d:%02d,active\n",
            newID, name, activity, day, month, year, hour, minute);

    fclose(fp);
    printf("✅ เพิ่มข้อมูลเรียบร้อย! (ID = %d)\n", newID);
}




// ค้นหาข้อมูล 
void searchRecord() {
    FILE *fp = fopen(FILE_NAME, "r");
    char line[256], keyword[100], status[20];
    if (fp == NULL) {
        printf("ไม่พบไฟล์ข้อมูล!\n");
        return;
    }

    printf("กรอกคำค้นหา (ชื่อหรือกิจกรรม): ");
    scanf(" %[^\n]", keyword);

    printf("\n---- ผลการค้นหา ----\n");
    int found = 0;
    while (fgets(line, sizeof(line), fp)) {
        char temp[256];
        strcpy(temp, line);
        char *token = strrchr(temp, ',');
        if (token != NULL) {
            token++;
            sscanf(token, "%s", status);
            if (strcmp(status, "deleted") == 0)
                continue; 
        }

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


    fprintf(temp, "ID,ชื่อผู้เข้าร่วม,ชื่อกิจกรรม,วันที่เข้าร่วม,เวลา,สถานะ\n");

    
    int c;
    while ((c = getchar()) != '\n' && c != EOF);

    printf("กรอกคำค้นหา (ชื่อหรือกิจกรรม): ");
    fgets(keyword, sizeof(keyword), stdin);
    keyword[strcspn(keyword, "\n")] = 0; 

    printf("\n---- ผลการค้นหา ----\n");


    if (!fgets(line, sizeof(line), fp)) {
        printf("ไฟล์ว่าง!\n");
        fclose(fp);
        fclose(temp);
        remove("temp.csv");
        return;
    }


    char allLines[1000][256];
    int lineCount = 0;
    strcpy(allLines[lineCount++], line); 

    int hasResult = 0;

    while (fgets(line, sizeof(line), fp)) {
        char fileId[10], name[100], activity[100], date[20], time[20], status[20];
        if (sscanf(line, "%[^,],%[^,],%[^,],%[^,],%[^,],%s",
                   fileId, name, activity, date, time, status) != 6)
            continue;

        strcpy(allLines[lineCount++], line); 

        if ((strstr(name, keyword) || strstr(activity, keyword)) && strcmp(status, "deleted") != 0) {
            printf("%s", line);
            hasResult = 1;
        }
    }

    if (!hasResult) {
        printf("ไม่พบข้อมูลที่ตรงกับ \"%s\"\n", keyword);
        fclose(fp);
        fclose(temp);
        remove("temp.csv");
        return;
    }

    
    printf("\nกรอก ID ที่ต้องการแก้ไข: ");
    scanf("%s", id);

    
    for (int i = 1; i < lineCount; i++) {
        char fileId[10], name[100], activity[100], date[20], time[20], status[20];
        if (sscanf(allLines[i], "%[^,],%[^,],%[^,],%[^,],%[^,],%s",
                   fileId, name, activity, date, time, status) != 6)
            continue;

        if (strcmp(fileId, id) == 0 && strcmp(status, "deleted") != 0) {
            found = 1;
            int done = 0;
            while (!done) {
                printf("\nข้อมูลปัจจุบัน:\n");
                printf("ID: %s\nชื่อ: %s\nกิจกรรม: %s\nวันที่: %s\nเวลา: %s\n",
                       fileId, name, activity, date, time);

                int choice;
                printf("\nเลือกสิ่งที่ต้องการแก้ไข:\n");
                printf("1. ชื่อ\n2. กิจกรรม\n3. วันที่/เดือน/ปี\n4. เวลา\n5. เสร็จสิ้น\n");
                printf("เลือก: ");
                scanf("%d", &choice);
                while ((c = getchar()) != '\n' && c != EOF);

                switch (choice) {
                    case 1:
                        printf("ชื่อใหม่: ");
                        fgets(name, sizeof(name), stdin);
                        name[strcspn(name, "\n")] = 0;
                        break;
                    case 2:
                        printf("กิจกรรมใหม่: ");
                        fgets(activity, sizeof(activity), stdin);
                        activity[strcspn(activity, "\n")] = 0;
                        break;
                    case 3: {
                        int day, month, year;
                        char buf[20];
                        while (1) {
                            printf("เดือน (1-12): ");
                            fgets(buf, sizeof(buf), stdin);
                            if (sscanf(buf, "%d", &month) == 1 && month >= 1 && month <= 12) break;
                            printf("⛔ กรุณาใส่ตัวเลข 1-12 เท่านั้น!\n");
                        }
                        int maxDay = getMaxDays(month);
                        while (1) {
                            printf("วัน (1-%d): ", maxDay);
                            fgets(buf, sizeof(buf), stdin);
                            if (sscanf(buf, "%d", &day) == 1 && day >= 1 && day <= maxDay) break;
                            printf("⛔ กรุณาใส่ตัวเลข 1-%d เท่านั้น!\n", maxDay);
                        }
                        while (1) {
                            printf("ปี: ");
                            fgets(buf, sizeof(buf), stdin);
                            if (sscanf(buf, "%d", &year) == 1 && year >= 1900 && year <= 2025) break;
                            printf("⛔ กรุณาใส่ตัวเลขปีที่ถูกต้อง!\n");
                        }
                        sprintf(date, "%02d/%02d/%04d", day, month, year);
                        break;
                    }
                    case 4: {
                        int hour, minute;
                        char buf[20];
                        while (1) {
                            printf("ชั่วโมง (0-23): ");
                            fgets(buf, sizeof(buf), stdin);
                            if (sscanf(buf, "%d", &hour) == 1 && hour >= 0 && hour <= 23) break;
                            printf("⛔ กรุณาใส่ตัวเลข 0-23 เท่านั้น!\n");
                        }
                        while (1) {
                            printf("นาที (0-59): ");
                            fgets(buf, sizeof(buf), stdin);
                            if (sscanf(buf, "%d", &minute) == 1 && minute >= 0 && minute <= 59) break;
                            printf("⛔ กรุณาใส่ตัวเลข 0-59 เท่านั้น!\n");
                        }
                        sprintf(time, "%02d:%02d", hour, minute);
                        break;
                    }
                    case 5:
                        done = 1;
                        break;
                    default:
                        printf("กรุณาเลือกตัวเลือกที่ถูกต้อง!\n");
                }
            }
            sprintf(allLines[i], "%s,%s,%s,%s,%s,%s", fileId, name, activity, date, time, status);
        }
    }


    for (int i = 0; i < lineCount; i++) {
        fprintf(temp, "%s\n", allLines[i]);
    }

    fclose(fp);
    fclose(temp);
    remove(FILE_NAME);
    rename("temp.csv", FILE_NAME);

    if (found)
        printf("\nบันทึกการแก้ไขเรียบร้อยแล้ว ✅\n");
    else
        printf("\nไม่พบข้อมูลที่ต้องการแก้ไข\n");
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


    int anyFound = 0;
    while (fgets(line, sizeof(line), fp)) {
        char tempLine[256], status[20];
        strcpy(tempLine, line);

        char *token = strrchr(tempLine, ',');
        if (token != NULL) {
            token++;
            sscanf(token, "%s", status);
        }

        if (strcmp(status, "deleted") != 0 && strcasestr_custom(line, keyword)) {
            printf("%s", line);
            anyFound = 1;
        }
    }

    if (!anyFound) {
        printf("ไม่พบข้อมูลที่ตรงกับ \"%s\"\n", keyword);
        fclose(fp);
        fclose(temp);
        remove("temp.csv");
        return;
    }

    printf("\nกรอก ID ที่ต้องการลบ: ");
    scanf("%s", id);

    rewind(fp);
    fprintf(temp, "ID,ชื่อผู้เข้าร่วม,ชื่อกิจกรรม,วันที่เข้าร่วม,เวลา,สถานะ\n"); 

    while (fgets(line, sizeof(line), fp)) {
        char fileId[10], name[100], activity[100], date[20], time[20], status[20];
        if (sscanf(line, "%[^,],%[^,],%[^,],%[^,],%[^,],%s",
                   fileId, name, activity, date, time, status) != 6) continue;

        if (strcmp(fileId, id) == 0 && strcmp(status, "deleted") != 0) {
            found = 1;
            printf("\nข้อมูลที่พบ:\n%s", line);
            char confirm;
            printf("ต้องการลบข้อมูลนี้จริงหรือไม่? (y/n): ");
            scanf(" %c", &confirm);
            if (confirm == 'y' || confirm == 'Y') {
                strcpy(status, "deleted");
                printf("✅ ข้อมูลถูกลบเรียบร้อย!\n");
            }
        }

        fprintf(temp, "%s,%s,%s,%s,%s,%s\n", fileId, name, activity, date, time, status);
    }

    fclose(fp);
    fclose(temp);
    remove(FILE_NAME);
    rename("temp.csv", FILE_NAME);

    if (!found)
        printf("ไม่พบข้อมูลที่ต้องการลบ!\n");
}
//คืนค่า
void restoreRecord() {
    FILE *fp = fopen(FILE_NAME, "r+"); 
    if (fp == NULL) {
        printf("ไม่สามารถเปิดไฟล์ได้\n");
        return;
    }

    char lines[1000][256]; 
    int lineCount = 0;
    char line[256];


    while (fgets(line, sizeof(line), fp)) {
        line[strcspn(line, "\n")] = '\0'; 
        strcpy(lines[lineCount++], line);
    }


    int anyDeleted = 0;
    printf("---- รายการที่ถูกลบ ----\n");
    for (int i = 1; i < lineCount; i++) { 
        char fileId[10], name[100], activity[100], date[20], time[20], status[20];
        sscanf(lines[i], "%[^,],%[^,],%[^,],%[^,],%[^,],%s",
               fileId, name, activity, date, time, status);
        if (strcmp(status, "deleted") == 0) {
            printf("%s\n", lines[i]);
            anyDeleted = 1;
        }
    }

    if (!anyDeleted) {
        printf("ไม่พบข้อมูลที่ถูกลบ!\n");
        fclose(fp);
        return;
    }

    char id[10];
    printf("กรอก ID ที่ต้องการคืนค่า: ");
    scanf("%s", id);

    int found = 0;
    for (int i = 1; i < lineCount; i++) { 
        char fileId[10], name[100], activity[100], date[20], time[20], status[20];
        sscanf(lines[i], "%[^,],%[^,],%[^,],%[^,],%[^,],%s",
               fileId, name, activity, date, time, status);
        if (strcmp(fileId, id) == 0 && strcmp(status, "deleted") == 0) {
            strcpy(status, "active"); 
            sprintf(lines[i], "%s,%s,%s,%s,%s,%s", fileId, name, activity, date, time, status);
            printf("✅ คืนค่าข้อมูลเรียบร้อย!\n");
            found = 1;
            break;
        }
    }

    if (!found) {
        printf("ไม่พบข้อมูลที่ต้องการคืนค่า!\n");
        fclose(fp);
        return;
    }

  
    rewind(fp);
    for (int i = 0; i < lineCount; i++) {
        fprintf(fp, "%s\n", lines[i]);
    }
    fclose(fp);
}


void addRecordTest(const char* name, const char* activity, int day, int month, int year, int hour, int minute) {
    FILE *fp = fopen(FILE_NAME, "a");
    if (!fp) {
        printf("ไม่สามารถเปิดไฟล์ได้\n");
        return;
    }

    int newID = getLastID() + 1;
    fprintf(fp, "%d,%s,%s,%02d/%02d/%04d,%02d:%02d,active\n",
            newID, name, activity, day, month, year, hour, minute);
    fclose(fp);
    printf("✅ UnitTest: เพิ่มข้อมูลเรียบร้อย! (ID=%d)\n", newID);
}


void searchRecordTest(const char* keyword) {
    FILE *fp = fopen(FILE_NAME, "r");
    char line[256], status[20];
    int found = 0;
    if (!fp) {
        printf("ไม่พบไฟล์ข้อมูล!\n");
        return;
    }

    printf("\n---- UnitTest: ผลการค้นหา \"%s\" ----\n", keyword);
    fgets(line, sizeof(line), fp); 
    while (fgets(line, sizeof(line), fp)) {
        char temp[256];
        strcpy(temp, line);
        char *token = strrchr(temp, ',');
        if (token != NULL) {
            token++;
            sscanf(token, "%s", status);
            if (strcmp(status, "deleted") == 0) continue;
        }
        if (strcasestr_custom(line, keyword)) {
            printf("%s", line);
            found = 1;
        }
    }
    if (!found) printf("ไม่พบข้อมูลที่ตรงกับ \"%s\"\n", keyword);
    fclose(fp);
}

void runUnitTest() {
    printf("\n========== RUN UNIT TEST ==========\n");
    addRecordTest("UnitTestUser", "UnitTestActivity", 1, 10, 2025, 12, 30);
    searchRecordTest("UnitTestUser");
    printf("========== END UNIT TEST ==========\n");
}
typedef struct {
    int id;
    char name[100];
    char activity[100];
    int day, month, year;
    int hour, minute;
} Record;

void showRecords() {
    FILE *fp = fopen(FILE_NAME, "r");
    if (fp == NULL) {
        printf("❌ ไม่มีไฟล์ข้อมูล\n");
        return;
    }

    char line[256];
    printf("\n📋 ข้อมูลในระบบ:\n");
    while (fgets(line, sizeof(line), fp)) {
        printf("%s", line);
    }
    fclose(fp);
}
void addRecordFromStruct(Record r) {
    FILE *fp = fopen(FILE_NAME, "a");
    if (!fp) {
        printf("❌ ไม่สามารถเปิดไฟล์ได้\n");
        return;
    }

    int newID = getLastID() + 1;
    fprintf(fp, "%d,%s,%s,%02d/%02d/%04d,%02d:%02d,active\n",
            newID, r.name, r.activity, r.day, r.month, r.year, r.hour, r.minute);
    fclose(fp);
    printf("✅ เพิ่มข้อมูลผู้ใช้: %s สำเร็จ (ID=%d)\n", r.name, newID);
}
int isValidDate(int day, int month) {
    int maxDay;
    if (month == 2) maxDay = 29;
    else if (month == 4 || month == 6 || month == 9 || month == 11) maxDay = 30;
    else maxDay = 31;

    if (day < 1 || day > maxDay) return 0;
    return 1;
}

void runE2ETest() {
    printf("\n===== 🔧 เริ่มการทดสอบ End-to-End (E2E Test) =====\n");

    // เตรียมไฟล์ CSV ใหม่
    FILE *fp = fopen(FILE_NAME, "w");
    fprintf(fp, "ID,ชื่อผู้เข้าร่วม,ชื่อกิจกรรม,วันที่เข้าร่วม,เวลา,สถานะ\n");
    fclose(fp);
    printf("✅ เตรียมไฟล์ CSV ใหม่แล้ว\n");

    // ข้อมูลทดสอบ
    Record inputs[] = {
        {1, "Surawit", "Coding Camp", 8, 10, 2025, 14, 30},  // ถูกต้อง
        {2, "1234", "AI Workshop", 8, 10, 2025, 9, 0},       // ชื่อผิด
        {3, "Nattapon", "Robotics", 31, 4, 2025, 10, 15},    // วันที่ผิด
        {4, "Anucha", "IoT", 29, 2, 2025, 12, 0},            // ถูกต้อง
        {5, "Siriluck", "Drone", 30, 2, 2025, 13, 0}         // วันที่ผิด
    };
    int n = sizeof(inputs)/sizeof(inputs[0]);

    for (int i = 0; i < n; i++) {
        if (!isValidName(inputs[i].name)) {
            printf("❌ ปฏิเสธข้อมูลชื่อผู้ใช้ไม่ถูกต้อง: %s\n", inputs[i].name);
            continue;
        }
        if (!isValidDate(inputs[i].day, inputs[i].month)) {
            printf("❌ ปฏิเสธข้อมูลวัน/เดือนไม่ถูกต้อง: %d/%d\n", inputs[i].day, inputs[i].month);
            continue;
        }
        addRecordFromStruct(inputs[i]);
    }

    // แสดงข้อมูลสุดท้าย
    showRecords();

    printf("\n✅ การทดสอบ E2E ครบทุกขั้นตอนเสร็จสิ้น!\n");
}



int main() {

    checkCSV();
    
    int choice;
    int running = 1;

    while (running) {
    printf("\n---------- ระบบจัดการข้อมูลผู้เข้าร่วมกิจกรรม ----------\n");
    printf("1. แสดงข้อมูลทั้งหมด\n");
    printf("2. เพิ่มข้อมูล\n");
    printf("3. ค้นหาข้อมูล\n");
    printf("4. แก้ไขข้อมูล\n");
    printf("5. ลบข้อมูล\n");
    printf("6. คืนค่าข้อมูลที่ถูกลบ\n");
    printf("7. UnitTest\n");
    printf("8. E2ETest\n");
    printf("เลือก: ");
    scanf("%d", &choice);

    switch (choice) {
        case 1: viewAll(); break;
        case 2: addRecord(); break;
        case 3: searchRecord(); break;
        case 4: updateRecord(); break;
        case 5: deleteRecord(); break;
        case 6: restoreRecord(); break;  
        case 7: runUnitTest();break;
        case 8: runE2ETest();break;
        case 9:
            printf("ออกจากโปรแกรมแล้วครับ!\n");
            running = 0;
            break;
        default:
            printf("กรุณาเลือกใหม่!\n");
    }
}

    return 0;
}