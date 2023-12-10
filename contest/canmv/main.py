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
thresholds = [
    # (0, 20, -128, 127, -23, 2),  # 灰色阈值
    (48, 100, -128, -8, -128, 127),  # 黄色阈值
    (0, 100, 32, 127, -128, 0)  # 蓝色阈值
]

while (True):
    clock.tick()  # 更新时钟
    img = sensor.snapshot()  # 拍摄一张图片，返回图片对象
    for blob in img.find_blobs(thresholds, pixels_threshold=100, area_threshold=100, merge=True):  # 找到颜色区域
        img.draw_rectangle(blob.rect())  # 标记颜色区域
        img.draw_cross(blob.cx(), blob.cy())  # 标记颜色区域的中心点
        print("x = %d, y = %d" % (blob.cx(), blob.cy()))  # 打印颜色区域的中心点坐标
