#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define FILE_NAME_REAL "activity.csv"
#define FILE_NAME_TEST "activity_test.csv"

// ----- ฟังก์ชันทั่วไป -----
int getMaxDays(int month) {
    if (month == 2) return 29;
    else if (month==4||month==6||month==9||month==11) return 30;
    else return 31;
}

int getLastID(const char* fileName) {
    FILE *fp = fopen(fileName,"r");
    if(!fp) return 0;
    char line[256]; int lastID=0;
    while(fgets(line,sizeof(line),fp)){
        int id;
        if(sscanf(line,"%d,",&id)==1) if(id>lastID) lastID=id;
    }
    fclose(fp);
    return lastID;
}

void viewAllFile(const char* fileName) {
    FILE *fp = fopen(fileName,"r");
    char line[256];
    if(!fp){ printf("ไม่พบไฟล์ข้อมูล!\n"); return;}
    printf("\n---- ข้อมูลทั้งหมด ----\n");
    while(fgets(line,sizeof(line),fp)) printf("%s",line);
    fclose(fp);
}

// ----- ฟังก์ชันไฟล์จริง -----
void addRecord() {
    FILE *fp = fopen(FILE_NAME_REAL,"a");
    char name[100], activity[100];
    int day, month, year, hour, minute;
    if(!fp){ printf("ไม่สามารถเปิดไฟล์ได้\n"); return;}
    int newID = getLastID(FILE_NAME_REAL)+1;

    printf("ชื่อผู้เข้าร่วม: "); scanf(" %[^\n]", name);
    printf("ชื่อกิจกรรม: "); scanf(" %[^\n]", activity);

    do{ printf("เดือน (1-12): "); scanf("%d",&month); if(month<1||month>12) printf("เดือนต้องอยู่ระหว่าง 1 ถึง 12!\n"); }
    while(month<1||month>12);

    int maxDay=getMaxDays(month);
    do{ printf("วัน (1-%d): ",maxDay); scanf("%d",&day); if(day<1||day>maxDay) printf("วันต้องอยู่ระหว่าง 1 ถึง %d!\n",maxDay); }
    while(day<1||day>maxDay);

    printf("ปี: "); scanf("%d",&year);
    printf("เวลา (ชั่วโมง นาที): "); scanf("%d %d",&hour,&minute);

    fprintf(fp,"%d,%s,%s,%02d/%02d/%04d,%02d:%02d\n",newID,name,activity,day,month,year,hour,minute);
    fclose(fp);
    printf("เพิ่มข้อมูลเรียบร้อย! (ID=%d)\n",newID);
}

void searchRecord() {
    FILE *fp = fopen(FILE_NAME_REAL,"r");
    char line[256], keyword[100];
    if(!fp){ printf("ไม่พบไฟล์ข้อมูล!\n"); return;}
    printf("กรอกคำค้นหา (ชื่อหรือกิจกรรม): "); scanf(" %[^\n]", keyword);
    int found=0;
    printf("\n---- ผลการค้นหา ----\n");
    while(fgets(line,sizeof(line),fp)){
        if(strstr(line,keyword)){ printf("%s",line); found=1; }
    }
    if(!found) printf("ไม่พบข้อมูลที่ตรงกับ \"%s\"\n",keyword);
    fclose(fp);
}

void updateRecordReal() {
    FILE *fp=fopen(FILE_NAME_REAL,"r");
    FILE *temp=fopen("temp.csv","w");
    if(!fp || !temp){ printf("ไม่สามารถเปิดไฟล์ได้\n"); return; }

    char line[256], id[10], keyword[100];
    printf("กรอกคำค้นหา (ชื่อหรือกิจกรรม): "); scanf(" %[^\n]", keyword);
    printf("\n---- ผลการค้นหา ----\n");
    while(fgets(line,sizeof(line),fp)) if(strstr(line,keyword)) printf("%s",line);

    printf("\nกรอก ID ที่ต้องการแก้ไข: "); scanf("%s",id);
    rewind(fp);

    int found=0;
    while(fgets(line,sizeof(line),fp)){
        char fileId[10], name[100], activity[100], date[20], time[20];
        sscanf(line,"%[^,],%[^,],%[^,],%[^,],%s",fileId,name,activity,date,time);
        if(strcmp(fileId,id)==0){
            found=1;
            int choice;
            printf("เลือกสิ่งที่ต้องการแก้ไข:\n1. ชื่อ\n2. กิจกรรม\n3. วันที่/เดือน/ปี\n4. เวลา\nเลือก: "); scanf("%d",&choice);
            if(choice==1){ printf("ชื่อใหม่: "); scanf(" %[^\n]",name); }
            else if(choice==2){ printf("กิจกรรมใหม่: "); scanf(" %[^\n]",activity); }
            else if(choice==3){
                int day,month,year;
                do{ printf("เดือน (1-12): "); scanf("%d",&month); }while(month<1||month>12);
                int maxDay=getMaxDays(month);
                do{ printf("วัน (1-%d): ",maxDay); scanf("%d",&day); }while(day<1||day>maxDay);
                printf("ปี: "); scanf("%d",&year);
                sprintf(date,"%02d/%02d/%04d",day,month,year);
            }
            else if(choice==4){
                int hour,minute;
                printf("เวลาใหม่ (ชั่วโมง นาที): "); scanf("%d %d",&hour,&minute);
                sprintf(time,"%02d:%02d",hour,minute);
            }
        }
        fprintf(temp,"%s,%s,%s,%s,%s\n",fileId,name,activity,date,time);
    }
    fclose(fp); fclose(temp);
    remove(FILE_NAME_REAL); rename("temp.csv",FILE_NAME_REAL);
    if(found) printf("แก้ไขข้อมูลเรียบร้อย!\n");
    else printf("ไม่พบข้อมูลที่ต้องการแก้ไข!\n");
}

void deleteRecordReal() {
    FILE *fp=fopen(FILE_NAME_REAL,"r");
    FILE *temp=fopen("temp.csv","w");
    if(!fp || !temp){ printf("ไม่สามารถเปิดไฟล์ได้\n"); return; }

    char line[256], id[10], keyword[100];
    printf("กรอกคำค้นหา (ชื่อหรือกิจกรรม): "); scanf(" %[^\n]", keyword);
    printf("\n---- ผลการค้นหา ----\n");
    while(fgets(line,sizeof(line),fp)) if(strstr(line,keyword)) printf("%s",line);

    printf("\nกรอก ID ที่ต้องการลบ: "); scanf("%s",id);
    rewind(fp);

    int found=0;
    while(fgets(line,sizeof(line),fp)){
        char fileId[10]; sscanf(line,"%[^,]",fileId);
        if(strcmp(fileId,id)==0){ found=1; continue; }
        fprintf(temp,"%s",line);
    }
    fclose(fp); fclose(temp);
    remove(FILE_NAME_REAL); rename("temp.csv",FILE_NAME_REAL);
    if(found) printf("ลบข้อมูลเรียบร้อย!\n");
    else printf("ไม่พบข้อมูลที่ต้องการลบ!\n");
}

// ----- ฟังก์ชันช่วยสำหรับ Test -----
void copyCSVToTestFile() {
    FILE *src=fopen(FILE_NAME_REAL,"r");
    FILE *dst=fopen(FILE_NAME_TEST,"w");
    if(!dst){printf("ไม่สามารถสร้างไฟล์ test ได้\n"); if(src) fclose(src); return;}
    if(!src){fclose(dst); return;}
    char line[256];
    while(fgets(line,sizeof(line),src)) fputs(line,dst);
    fclose(src); fclose(dst);
}

void addRecordAutoTest(int id,const char* name,const char* activity,int day,int month,int year,int hour,int minute){
    FILE *fp=fopen(FILE_NAME_TEST,"a");
    if(!fp){printf("ไม่สามารถเปิดไฟล์ test ได้\n"); return;}
    fprintf(fp,"%d,%s,%s,%02d/%02d/%04d,%02d:%02d\n",id,name,activity,day,month,year,hour,minute);
    fclose(fp);
}

int searchRecordAutoTest(const char* keyword){
    FILE *fp=fopen(FILE_NAME_TEST,"r");
    if(!fp){printf("ไม่พบไฟล์ test!\n"); return 0;}
    char line[256]; int found=0;
    while(fgets(line,sizeof(line),fp)){
        if(strstr(line,keyword)){ printf("พบข้อมูล: %s",line); found=1; }
    }
    fclose(fp);
    return found;
}

// ----- Unit Test -----
void runUnitTest(){
    printf("\n====== Unit Test ======\n");
    copyCSVToTestFile();
    int newID=getLastID(FILE_NAME_TEST)+1;
    addRecordAutoTest(newID,"UnitTester","EdgeCaseActivity",31,12,2025,23,59);
    if(searchRecordAutoTest("EdgeCaseActivity")) printf("Unit Test Passed!\n");
    else printf("Unit Test Failed!\n");
}

// ----- E2E Test -----
void runE2ETest(){
    printf("\n====== E2E Test ======\n");
    copyCSVToTestFile();
    int id1=getLastID(FILE_NAME_TEST)+1;
    addRecordAutoTest(id1,"Tester1","Activity1",1,1,2025,10,0);
    int id2=getLastID(FILE_NAME_TEST)+1;
    addRecordAutoTest(id2,"Tester2","Activity2",15,2,2025,15,30);

    printf("\n-- แสดงข้อมูลทั้งหมด (Test File) --\n"); viewAllFile(FILE_NAME_TEST);
    printf("\n-- ค้นหาข้อมูล Activity2 --\n"); searchRecordAutoTest("Activity2");

    printf("\n-- แก้ไขข้อมูล ID=1 (Test File) --\n");
    FILE *fp=fopen(FILE_NAME_TEST,"r");
    FILE *temp=fopen("temp.csv","w");
    char line[256];
    while(fgets(line,sizeof(line),fp)){
        char fileId[10], name[100], activity[100], date[20], time[20];
        sscanf(line,"%[^,],%[^,],%[^,],%[^,],%s",fileId,name,activity,date,time);
        if(strcmp(fileId,"1")==0) strcpy(activity,"UpdatedActivity");
        fprintf(temp,"%s,%s,%s,%s,%s\n",fileId,name,activity,date,time);
    }
    fclose(fp); fclose(temp);
    remove(FILE_NAME_TEST); rename("temp.csv",FILE_NAME_TEST);
    viewAllFile(FILE_NAME_TEST);

    printf("\n-- ลบข้อมูล ID=2 (Test File) --\n");
    fp=fopen(FILE_NAME_TEST,"r");
    temp=fopen("temp.csv","w");
    while(fgets(line,sizeof(line),fp)){
        char fileId[10]; sscanf(line,"%[^,]",fileId);
        if(strcmp(fileId,"2")==0) continue;
        fprintf(temp,"%s",line);
    }
    fclose(fp); fclose(temp);
    remove(FILE_NAME_TEST); rename("temp.csv",FILE_NAME_TEST);
    viewAllFile(FILE_NAME_TEST);

    printf("\nE2E Test Completed!\n");
}

// ----- Main -----
int main(){
    int choice,running=1;
    while(running){
        printf("\n---------- ระบบจัดการข้อมูลผู้เข้าร่วมกิจกรรม ----------\n");
        printf("1. แสดงข้อมูลทั้งหมด\n");
        printf("2. เพิ่มข้อมูล\n");
        printf("3. ค้นหาข้อมูล\n");
        printf("4. แก้ไขข้อมูล\n");
        printf("5. ลบข้อมูล\n");
        printf("6. รัน Unit Test\n");
        printf("7. รัน E2E Test\n");
        printf("8. ออก\nเลือก: "); scanf("%d",&choice);

        switch(choice){
            case 1: viewAllFile(FILE_NAME_REAL); break;
            case 2: addRecord(); break;
            case 3: searchRecord(); break;
            case 4: updateRecordReal(); break;
            case 5: deleteRecordReal(); break;
            case 6: runUnitTest(); break;
            case 7: runE2ETest(); break;
            case 8: running=0; printf("ออกจากโปรแกรมแล้วครับ!\n"); break;
            default: printf("กรุณาเลือกใหม่!\n");
        }
    }
    return 0;
}
