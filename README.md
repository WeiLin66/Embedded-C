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

### 中斷實驗
    1. 將key1, key2設置成外部IO中斷觸發
    2. 分別配置上升緣與下降緣
    3. 當按下按鍵時使用中斷Toogle指定LED

### SysTick實驗
    1. 設定每1ms進入中斷
    2. 每1秒觸發LED閃爍
    3. LED規律: 紅 → 暗 → 綠 → 暗 → 藍 → ...

### USART console command實驗
    1. 使用Access Port與開發版溝通
    2. 使用中斷獲取command消息
    3. 編寫多個控制指令
        1. led r/g/b 0/1 → 控制單顆LED
        2. flash → LED流水燈
        3. stop → 停在當前動作
        4. clear → 復位

### DMA實驗
    1. Memory to Memory
        1. flash to SRAM
        2. 使用LED顯示過程、結果、狀態
    2. Memory to Peripheral
        1. flash to USART
        2. 在PC端打印出結果
        3. 使用FIFO模式、突發模式
### I2C實驗

### SPI實驗
