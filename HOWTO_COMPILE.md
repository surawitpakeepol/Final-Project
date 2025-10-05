# 🧩 HOWTO_COMPILE.md
---
## 💡 เกี่ยวกับโปรเจกต์
โครงการนี้พัฒนาโดยใช้ภาษา **C**  
เพื่อให้ข้อความภาษาไทยแสดงผลได้ถูกต้อง จำเป็นต้อง **ตั้งค่าภาษา (locale) เป็นภาษาไทย (UTF-8)** ก่อนทำการคอมไพล์และรันโปรแกรม

---

## ⚙️ วิธีคอมไพล์และรันโปรแกรม
---
### 🔹 สำหรับ Windows
````markdown

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




📄 **License:** MIT
👨‍💻 **ผู้พัฒนา:** Surawit Phakeepon
📅 **อัปเดตล่าสุด:** 2025-10-05



