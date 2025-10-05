# 🧩 HOWTO_COMPILE.md
---
## 💡 เกี่ยวกับโปรเจกต์
โครงการนี้พัฒนาโดยใช้ภาษา **C**  
มีจุดประสงค์เพื่อสร้างระบบจัดการข้อมูลผู้เข้าร่วมกิจกรรม (Activity Management System)  

รองรับการทดสอบหน่วย (Unit Test) ด้วย **Unity Framework**  
เพื่อให้ข้อความภาษาไทยแสดงผลได้ถูกต้อง จำเป็นต้อง **ตั้งค่าภาษา (locale) เป็นภาษาไทยแบบ UTF-8)** ก่อนทำการคอมไพล์และรันโปรแกรม

---
### 🔹 สำหรับ Windows

---

## ⚙️ วิธีคอมไพล์และรันโปรแกรม


1. **ตรวจสอบว่าได้ติดตั้ง GCC แล้ว**  
   เปิด Command Prompt แล้วพิมพ์คำสั่ง:
````markdown   
   ```bash
   gcc --version
   ```
````

หากยังไม่มี ให้ติดตั้งผ่าน [MinGW](https://sourceforge.net/projects/mingw/) หรือ [TDM-GCC](https://jmeubank.github.io/tdm-gcc/)

2. **ตั้งค่าภาษาใน Command Prompt ให้รองรับ UTF-8**

   ```bash
   chcp 65001
   ```

3. **คอมไพล์ไฟล์โปรแกรมหลัก (รวม Unity)**
   สมมติว่าไฟล์ชื่อ `main.c` และมีไลบรารี Unity (`unity.c`, `unity.h`) อยู่ในโฟลเดอร์เดียวกัน

   ```bash
   gcc main.c unity.c -o main.exe
   ```

4. **รันโปรแกรม**

   ```bash
   main.exe
   ```

5. **(ทางเลือก)** หากต้องการรันเฉพาะ Unit Test
   ภายในเมนูโปรแกรมให้เลือกตัวเลือก **6. ทดสอบระบบ (Unit Test)**

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

3. **คอมไพล์ไฟล์โปรแกรมหลัก (รวม Unity)**

   ```bash
   gcc main.c unity.c -o main
   ```

4. **รันโปรแกรม**

   ```bash
   ./main
   ```

5. **รัน Unit Test (ภายในโปรแกรม)**
   เลือกเมนูหมายเลข **6** เมื่อโปรแกรมเริ่มทำงาน

---

## 🧱 โครงสร้างไฟล์โปรเจกต์ (แนะนำ)

```
📦 Project/
 ├── main.c             # โค้ดหลักของโปรแกรม
 ├── unity.c            # ไลบรารี Unity สำหรับ Unit Test
 ├── unity.h
 ├── activity.csv       # ไฟล์เก็บข้อมูลกิจกรรม
 ├── HOWTO_COMPILE.md   # คู่มือการคอมไพล์ (ไฟล์นี้)
 └── README.md          # (อาจเพิ่มภายหลัง)
```

---

📄 **License:** MIT
👨‍💻 **ผู้พัฒนา:** Surawit Phakeepon
🧪 **ใช้ Unity Framework สำหรับการทดสอบหน่วย**
📅 **อัปเดตล่าสุด:** 2025-10-05


