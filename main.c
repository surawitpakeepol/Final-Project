#include <stdio.h>
#include <string.h>

int main()
{   
    int Choice;
    char Name[100], Activity[100];
    int Day, Month, Year; 
    int Hour, Minute;
    char data[256], line[256];
    char *NameColumn, *ActivityColumn, *DateColumn, *TimeColumn;
    char searchName[100], deleteName[100];
    int searchChoice;
    char searchText[100];
    int found = 0;
    int updated = 0;
    int deleted = 0;
    char confirm;

    FILE *activity;

    //เช็คCSV
    activity = fopen("activity.csv", "r");
    if (activity == NULL) {

        activity = fopen("activity.csv", "w");
        if (activity == NULL) {
            printf("ไม่สามารถสร้างไฟล์ได้\n");
            return 1;
        }
        fprintf(activity, "ชื่อผู้เข้าร่วม,ชื่อกิจกรรม,วันที่เข้าร่วม,เวลา\n");
        fclose(activity);
    } 
    else {
        fclose(activity);
    }
    //MENU
    while (1) {   
        printf("\n----------ระบบจัดการข้อมูลผู้เข้าร่วมกิจกรรม----------\n");
        printf("[1] เพิ่มข้อมูลการเข้าร่วมกิจกรรม\n");
        printf("[2] ค้นหาข้อมูลการเข้าร่วมกิจกรรม\n");
        printf("[3] แก้ไขข้อมูลการเข้าร่วมกิจกรรม\n");
        printf("[4] ลบข้อมูลการเข้าร่วมกิจกรรม\n");    
        printf("[5] ออกจากระบบจัดการข้อมูลผู้เข้าร่วมกิจกรรม\n");
        do {
            printf("เลือกเมนู (1-5): ");
            if (scanf("%d", &Choice) != 1) {

                printf("บอกให้เลือก1-5!\n");

                while (getchar() != '\n');
                Choice = 0; 
            } else if (Choice < 1 || Choice > 5) {
                printf("ไม่ใช่เลือกอีกรอบบ!! (1-5)\n");
            }
        } while (Choice < 1 || Choice > 5);

        if (Choice == 5) {  
            printf("ออกจากโปรแกรมเรียบร้อย!\n");
            break;  
        }

        switch (Choice) {
    // ADD
        case 1:
            printf("\nชื่อผู้เข้าร่วมกิจกรรม: ");
            scanf(" %[^\n]", Name);   
            printf("ชื่อกิจกรรม: ");
            scanf(" %[^\n]", Activity);   
            printf("วัน/เดือน/ปี (เช่น 10 12 2021): ");
            scanf("%d %d %d", &Day, &Month, &Year);
            printf("เวลา (ชั่วโมง นาที เช่น 10 30): ");
            scanf("%d %d", &Hour, &Minute);

            activity = fopen("activity.csv", "a");
            if (activity == NULL) {
                printf("ไม่สามารถเปิดไฟล์ได้\n");
                return 1;
            }
            fprintf(activity, "%s,%s,%02d/%02d/%04d,%02d:%02d\n",
                    Name, Activity, Day, Month, Year, Hour, Minute);
            fclose(activity);

            printf("เพิ่มข้อมูลเรียบร้อย!\n");
        //เเสดงข้อมูลในCSV
        activity = fopen("activity.csv", "r");
            if (activity == NULL) {
               printf("ไม่สามารถอ่านไฟล์ได้\n");
                return 1;
            }
            printf("\n----------รายละเอียดกิจกรรม----------\n");
            while (fgets(data, sizeof(data), activity)) {
                printf("%s", data);
            }
            fclose(activity);
            
            break;

    // SEARCH
        case 2: 
            do {
                printf("\n---- ค้นหาข้อมูลการเข้าร่วมกิจกรรม ----\n");
                printf("[1] ค้นหาด้วยชื่อ\n");
                printf("[2] ค้นหาด้วยกิจกรรม\n");
                printf("เลือกเมนูค้นหา (1-2): ");
                if (scanf("%d", &searchChoice) != 1) {
                    printf("บอกให้เลือก 1-2!\n");
                    while (getchar() != '\n');  
                    searchChoice = 0;           
                } else if (searchChoice < 1 || searchChoice > 2) {
                    printf("ไม่ใช่! เลือกใหม่ (1-2)\n");
                }
            } while (searchChoice < 1 || searchChoice > 2);

            printf("กรอกคำค้นหา: ");
            scanf(" %[^\n]", searchText);   

            activity = fopen("activity.csv", "r");
            if (activity == NULL) {
                printf("ไม่สามารถเปิดไฟล์ได้\n");
                break;
            }

            fgets(line, sizeof(line), activity); 
            printf("\n---------- ผลลัพธ์การค้นหา ----------\n");

            found = 0;
            while (fgets(line, sizeof(line), activity)) {
                line[strcspn(line, "\n")] = 0; 

                NameColumn     = strtok(line, ",");
                ActivityColumn = strtok(NULL, ",");
                DateColumn     = strtok(NULL, ",");
                TimeColumn     = strtok(NULL, ",");

                int match = 0;
                switch (searchChoice) {
                    case 1: 
                        if (NameColumn && strcmp(NameColumn, searchText) == 0) match = 1;
                        break;
                    case 2: 
                        if (ActivityColumn && strcmp(ActivityColumn, searchText) == 0) match = 1;
                        break;
                }

                if (match) {
                    printf("%s,%s,%s,%s\n", NameColumn, ActivityColumn, DateColumn, TimeColumn);
                    found = 1;
                }
            }

            if (!found) {
                printf("ไม่พบข้อมูลที่ค้นหา \"%s\"\n", searchText);
            }

            fclose(activity);
            break;

    // UPDATE        
        case 3:
            printf("\n---- แก้ไขข้อมูลการเข้าร่วมกิจกรรม ----\n");
            printf("กรอกชื่อผู้เข้าร่วมที่ต้องการแก้ไข: ");
            scanf(" %[^\n]", searchName);   

            FILE *temp = fopen("temp.csv", "w");
            activity = fopen("activity.csv", "r");

            if (activity == NULL || temp == NULL) {
                printf("ไม่สามารถเปิดไฟล์ได้\n");
                break;
            }

            if (fgets(line, sizeof(line), activity)) {
                fprintf(temp, "%s", line);
            }

            updated = 0;
            while (fgets(line, sizeof(line), activity)) {
                line[strcspn(line, "\n")] = 0; 

                NameColumn     = strtok(line, ",");
                ActivityColumn = strtok(NULL, ",");
                DateColumn     = strtok(NULL, ",");
                TimeColumn     = strtok(NULL, ",");

                if (NameColumn && strcmp(NameColumn, searchName) == 0) {
                    printf("\nข้อมูลเดิม: %s,%s,%s,%s\n",
                        NameColumn, ActivityColumn, DateColumn, TimeColumn);
                    printf("ชื่อใหม่: ");
                    scanf(" %[^\n]", Name);   
                    printf("กิจกรรมใหม่: ");
                    scanf(" %[^\n]", Activity);   
                    printf("วัน/เดือน/ปี ใหม่ (เช่น 10 12 2021): ");
                    scanf("%d %d %d", &Day, &Month, &Year);
                    printf("เวลาใหม่ (ชั่วโมง นาที เช่น 10 30): ");
                    scanf("%d %d", &Hour, &Minute);

                    fprintf(temp, "%s,%s,%02d/%02d/%04d,%02d:%02d\n",
                            Name, Activity, Day, Month, Year, Hour, Minute);
                    updated = 1;
                } else {
                    fprintf(temp, "%s,%s,%s,%s\n",
                            NameColumn, ActivityColumn, DateColumn, TimeColumn);
                }
            }

            fclose(activity);
            fclose(temp);

            remove("activity.csv");
            rename("temp.csv", "activity.csv");

            if (updated) {
                printf("แก้ไขข้อมูลเรียบร้อย!\n");
            } else {
                printf("ไม่พบชื่อที่ต้องการแก้ไข\n");
            }
            break;

    // DELETE
        case 4:
            printf("\n---- ลบข้อมูลการเข้าร่วมกิจกรรม ----\n");
            printf("กรอกชื่อผู้เข้าร่วมที่ต้องการลบ: ");
            scanf(" %[^\n]", deleteName);   

            printf("คุณต้องการลบข้อมูลของ \"%s\" ใช่หรือไม่? (y/n): ", deleteName);
            scanf(" %c", &confirm);

            if (confirm != 'y' && confirm != 'Y') {
                printf("ยกเลิกการลบข้อมูล\n");
                break;
            }

            temp = fopen("temp.csv", "w");
            activity = fopen("activity.csv", "r");

            if (activity == NULL || temp == NULL) {
                printf("ไม่สามารถเปิดไฟล์ได้\n");
                break;
            }

            if (fgets(line, sizeof(line), activity)) {
                fprintf(temp, "%s", line);
            }

            deleted = 0;
            while (fgets(line, sizeof(line), activity)) {
                line[strcspn(line, "\n")] = 0;

                NameColumn     = strtok(line, ",");
                ActivityColumn = strtok(NULL, ",");
                DateColumn     = strtok(NULL, ",");
                TimeColumn     = strtok(NULL, ",");

                if (NameColumn && strcmp(NameColumn, deleteName) != 0) {
                    fprintf(temp, "%s,%s,%s,%s\n",
                            NameColumn, ActivityColumn, DateColumn, TimeColumn);
                } else {
                    deleted = 1;
                }
            }

            fclose(activity);
            fclose(temp);

            remove("activity.csv");
            rename("temp.csv", "activity.csv");

            if (deleted) {
                printf("ลบข้อมูลของ \"%s\" เรียบร้อย!\n", deleteName);
            } else {
                printf("ไม่พบชื่อที่ต้องการลบ\n");
            }
            break;
        }
        
    } 

    return 0;
}
