#include <stdio.h>
#include <string.h>

int main()
{   
    int Choice;
    char Name[100], Activity[100];
    int Day, Month, Year; 
    int Hour, Minute;
    char data[256];
    char *NameColumn,*ActivityColumn,*DateColumn,*TimeColumn;
    char searchName[100];
    char line[256];
    int found = 0;

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
} else {

    fclose(activity);
}
//MENU
    while (1) {   
        printf("\n----------ระบบจัดการข้อมูลผู้เข้าร่วมกิจกรรม----------\n");
        printf("[1] เพิ่มข้อมูลการเข้าร่วมกิจกรรม\n");
        printf("[2] ค้นหาข้อมูลการเข้าร่วมกิจกรรม\n");
        printf("[3] แก้ไขข้อมูลการเข้าร่วมกิจกรรม\n");
        printf("[4] ลบข้อมูลการเข้าร่วมกิจกรรม\n");    
        printf("[5] อื่น ๆ\n");
        printf("[6] ออกจากระบบจัดการข้อมูลผู้เข้าร่วมกิจกรรม\n");
        do {
        printf("เลือกเมนู (1-6): ");
        if (scanf("%d", &Choice) != 1) {

            printf("บอกให้เลือก1-6!\n");

            while (getchar() != '\n');
            Choice = 0; 
        } else if (Choice < 1 || Choice > 6) {
            printf("ไม่ใช่เลือกอีกรอบบ!! (1-6)\n");
        }
        } while (Choice < 1 || Choice > 6);

        if (Choice == 6) {  
            printf("ออกจากโปรแกรมเรียบร้อย!\n");
            break;  
        }

        switch (Choice) {
//ADD
        case 1:
            printf("\nชื่อผู้เข้าร่วมกิจกรรม: ");
            scanf(" %[^\n]", Name);
            printf("ชื่อกิจกรรม: ");
            scanf("%[^\n]", Activity);
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
//search
        case 2: 
            char searchName[100];
            char line[256];
            int found = 0;

            printf("\nกรอกชื่อผู้เข้าร่วมกิจกรรมที่ต้องการค้นหา: ");
            scanf("%s", searchName);

            activity = fopen("activity.csv", "r");
            if (activity == NULL) {
                printf("ไม่สามารถเปิดไฟล์ได้\n");
                break;
            }

            printf("\n----------ผลลัพธ์การค้นหา----------\n");

            fgets(line, sizeof(line), activity);
            printf("%s", line);  

            while (fgets(line, sizeof(line), activity)) {

                NameColumn = strtok(line, ",");       
                ActivityColumn = strtok(NULL, ",");   
                DateColumn = strtok(NULL, ",");       
                TimeColumn = strtok(NULL, ",");       

                if (NameColumn != NULL && strcmp(NameColumn, searchName) == 0) {
                    printf("%s,%s,%s,%s\n", NameColumn, ActivityColumn, DateColumn, TimeColumn);
                    found = 1;
                }
            }

            if (!found) {
                printf("ไม่พบข้อมูลของชื่อ %s\n", searchName);
            }

            fclose(activity);
            break;
        

        case 3:
        case 4:
        case 5:
            printf("ฟีเจอร์ยังไม่พร้อมใช้งาน\n");
            break;
            
        }
    } 

    return 0;
}