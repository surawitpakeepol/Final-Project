ได้เลยครับ! 🎯
ด้านล่างนี้คือไฟล์ `HOWTO_COMPILE.md` **ฉบับพร้อมใช้งาน** — แค่คัดลอกไปวางใน GitHub ก็ใช้งานได้ทันที
(ออกแบบมาสำหรับโค้ดภาษา **C ที่มีข้อความภาษาไทย** ต้องตั้งค่าภาษาเป็น UTF-8 ก่อน compile)

---

````markdown
# 🧩 HOWTO_COMPILE.md

## 💡 เกี่ยวกับโปรเจกต์
โครงการนี้พัฒนาโดยใช้ภาษา **C**  
เพื่อให้ข้อความภาษาไทยแสดงผลได้ถูกต้อง จำเป็นต้อง **ตั้งค่าภาษา (locale) เป็นภาษาไทย (UTF-8)** ก่อนทำการคอมไพล์และรันโปรแกรม

---

## ⚙️ วิธีคอมไพล์และรันโปรแกรม

### 🔹 สำหรับ Windows

1. **ตรวจสอบว่าได้ติดตั้ง GCC แล้ว**  
   เปิด Command Prompt แล้วพิมพ์คำสั่ง:
   ```bash
   gcc --version
````

หากยังไม่มี ให้ติดตั้งผ่าน [MinGW](https://sourceforge.net/projects/mingw/) หรือ [TDM-GCC](https://jmeubank.github.io/tdm-gcc/)

2. **ตั้งค่าภาษาใน Command Prompt ให้รองรับ UTF-8**

   ```bash
   chcp 65001
   ```

3. **คอมไพล์ไฟล์ C**
   สมมติว่าไฟล์ชื่อ `main.c`

   ```bash
   gcc main.c -o main.exe
   ```

4. **รันโปรแกรม**

   ```bash
   main.exe
   ```

> 📝 **หมายเหตุ:**
>
> * ให้บันทึกไฟล์ `.c` เป็น **UTF-8 (without BOM)**
> * หากข้อความภาษาไทยยังแสดงผิด ให้ลองเปลี่ยนฟอนต์ของ Command Prompt เป็น **"Consolas" หรือ "Lucida Console"**

---

### 🔹 สำหรับ Linux / macOS

1. **ตรวจสอบว่าเครื่องมี GCC แล้ว**

   ```bash
   gcc --version
   ```

2. **ตั้งค่าภาษาให้เป็นไทย (UTF-8)**

   ```bash
   export LANG=th_TH.UTF-8
   export LC_ALL=th_TH.UTF-8
   ```

3. **คอมไพล์ไฟล์ C**

   ```bash
   gcc main.c -o main
   ```

4. **รันโปรแกรม**

   ```bash
   ./main
   ```

---

## 🧠 ตัวอย่างโค้ดภาษา C

```c
#include <stdio.h>
#include <locale.h>

int main() {
    setlocale(LC_ALL, "th_TH.UTF-8");
    printf("สวัสดีครับ! นี่คือโปรแกรมภาษา C ที่รองรับภาษาไทย\n");
    return 0;
}
```

---

## ✅ สรุปคำสั่งหลัก

| ระบบปฏิบัติการ  | ตั้งค่าภาษา                                              | คำสั่งคอมไพล์            | คำสั่งรัน  |
| --------------- | -------------------------------------------------------- | ------------------------ | ---------- |
| **Windows**     | `chcp 65001`                                             | `gcc main.c -o main.exe` | `main.exe` |
| **Linux/macOS** | `export LANG=th_TH.UTF-8`<br>`export LC_ALL=th_TH.UTF-8` | `gcc main.c -o main`     | `./main`   |

---

📄 **License:** MIT
👨‍💻 **ผู้พัฒนา:** Surawit Phakeepon
📅 **อัปเดตล่าสุด:** 2025-10-05

```

---

อยากให้ผมช่วยเพิ่ม **Makefile** หรือ **วิธีรันผ่าน VS Code (Terminal + UTF-8)** ให้ใน README ด้วยไหมครับ? จะได้ครบชุดสำหรับ GitHub โปรเจกต์ภาษา C ของคุณเลย ✅
```
