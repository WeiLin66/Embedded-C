## embedded c STM32F429 projects
### LED 閃爍
    1. 使用庫函式
    2. 流水燈閃爍: 紅 → 綠 → 藍
    3. 間格時間: 500ms

### 按鍵觸發LED
    1. 設置兩個按鍵GPIO
        1. GPIOA PIN0
        2. GPIOC PIN13
    2. 按下按鍵後toogle LED燈
    3. PA0 → 紅
    4. PC13 → 綠

### 按鍵觸發LED(帶位操作版本)
    1. 將初始化操作改成bit-banding
    2. 將toggle函式改為bit-banding 
    3. 新增位帶區段地址相關macro

### RCC
    1. RCC的基本系統時間配置程式碼範例說明
    2. 自定義HSE函數配置系統時間
