# 导入相关模块
import sensor
import image
import time
from fpioa_manager import fm
from machine import UART

# 设置摄像头分辨率和颜色模式
sensor.reset()  # 初始化摄像头
sensor.set_pixformat(sensor.RGB565)  # 设置颜色模式为RGB565
sensor.set_framesize(sensor.QVGA)  # 设置图像分辨率为QVGA (320x240)
sensor.skip_frames(time=2000)  # 跳过2000ms的图像，使新设置生效
sensor.set_auto_gain(False)  # 关闭自动增益
sensor.set_auto_whitebal(False)  # 关闭白平衡

# 设置时钟
clock = time.clock()  # 创建一个时钟对象，用于计算帧率

# 设置串口
fm.register(11, fm.fpioa.UART1_TX, force=True)  # 设置IO11为UART1的TX引脚
fm.register(10, fm.fpioa.UART1_RX, force=True)  # 设置IO10为UART1的RX引脚
uart_A = UART(UART.UART1, 115200, 8, 1, 0, timeout=1000,
              read_buf_len=4096)  # 创建一个串口对象

# 设置颜色阈值
ithresholds = [(40, 56, -1, 5, -13, 1)] # 铁矿(0, 40, -6, 127, -5, 127)
gthresholds = [(30, 100, -128, 16, 16, 127)] # 金矿
dthresholds = [(0, 37, -128, 127, -53, -14)] # 钻石矿

# 存储色素块位置
goldmap = []
diamondmap = []

while (True):
    clock.tick()  # 更新时钟
    img = sensor.snapshot()  # 拍摄一张图片，返回图片对象
    #Img = cv2.imread(img, cv2.IMREAD_GRAYSCALE)
    ## 使用Canny边缘检测算法找到边缘
    #edges = cv2.Canny(Img, threshold1=30, threshold2=100)
    ## 显示边缘
    #cv2.imshow('Edges', edges)
    #cv2.waitKey(0)
    #cv2.destroyAllWindows()
    total_gold = 0
    total_diamond = 0
    for blob in img.find_blobs(gthresholds, pixels_threshold=100, area_threshold=100, merge=True):  # 找到颜色区域
        img.draw_rectangle(blob.rect())  # 标记颜色区域
        img.draw_cross(blob.cx(), blob.cy())  # 标记颜色区域的中心点
        cx, cy = blob.x(), blob.y()
        img_width, img_height = img.width(), img.height()
        x = min(max(int((cx / img_width) * 8) + 1, 1), 8)
        y = min(max(int((cy / img_height) * 8) + 1, 1), 8)
        goldmap.append((x,y))
        total_gold += 1
        #print('gold')
        #print(x,y)
        #print("gold x = %d, y = %d" % (blob.cx(), blob.cy()))  # 打印颜色区域的中心点坐标
    for blob in img.find_blobs(dthresholds, pixels_threshold=100, area_threshold=100, merge=True):  # 找到颜色区域
        img.draw_rectangle(blob.rect())  # 标记颜色区域
        img.draw_cross(blob.cx(), blob.cy())  # 标记颜色区域的中心点
        cx, cy = blob.x(), blob.y()
        img_width, img_height = img.width(), img.height()
        x = min(max(int((cx / img_width) * 8) + 1, 1), 8)
        y = min(max(int((cy / img_height) * 8) + 1, 1), 8)
        diamondmap.append((x,y))
        total_diamond += 1
        #print('diamond')
        #print(x,y)
        #print("diamond x = %d, y = %d" % (blob.cx(), blob.cy()))  # 打印颜色区域的中心点坐标
    #totalx = 0
    #totaly = 0
    #number = 0
    #for i in goldmap:
        #totalx += i[0]
        #totaly += i[1]
        #number+=1
    #for i in diamondmap:
        #totalx += i[0]
        #totaly += i[1]
        #number+=1
    #totalx /=number
    #totaly /=number
    '''
    采用如下信息编码格式，第一位表示矿物类型，第二位表示接下来发送数据长度
    接下来的每两个构成一个完整坐标，最后一个字节的数据是校验位，大小应该等
    于前面所有数据的和
    '''
    # 发送金矿位置 0x55
    if len(goldmap)!=0:
        check = 85
        uart_A.write(b'\x55')
        total_gold = total_gold*2+1
        check += total_gold
        gold_length = bytes([total_gold])
        uart_A.write(gold_length)
        for i in goldmap:
            uart_A.write(bytes([i[0]]))
            uart_A.write(bytes([i[1]]))
            check += i[0]+i[1]
        check %=256
        uart_A.write(bytes([check]))
    # 发送钻石矿位置 0x56
    if len(diamondmap)!=0:
        check = 86
        uart_A.write(b'\x56')
        total_diamond = total_diamond*2+1
        check += total_diamond
        diamond_length = bytes([total_diamond])
        uart_A.write(diamond_length)
        for i in diamondmap:
            uart_A.write(bytes([i[0]]))
            uart_A.write(bytes([i[1]]))
            check += i[0]+i[1]
        check %=256
        uart_A.write(bytes([check]))
    goldmap.clear()
    diamondmap.clear()
    time.sleep_ms(95)


