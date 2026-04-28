# Tetris - Qt6 C++

一个用 Qt6 和 C++ 实现的俄罗斯方块游戏。

## 功能特性

- ✅ 完整的俄罗斯方块游戏逻辑
- ✅ 7 种标准四棱柱方块 (Tetromino)
- ✅ 流畅的游戏操作（移动、旋转、下降）
- ✅ 消行判定和得分系统
- ✅ 动态难度等级（难度越高，方块下降越快）
- ✅ 下一个方块预览
- ✅ 暂停/继续功能
- ✅ 游戏结束检测
- ✅ 现代化的用户界面

## 需求

- Qt6 (Core, Gui, Widgets)
- CMake 3.21+
- C++17 或更高
- 编译器：GCC, Clang, MSVC

## 编译和运行

### Linux/Mac

```bash
mkdir build
cd build
cmake ..
make
./tetris-qt6
```

### Windows (MSVC)

```bash
mkdir build
cd build
cmake -G "Visual Studio 17 2022" ..
cmake --build . --config Release
.\Release\tetris-qt6.exe
```

## 游戏操作

| 按键 | 功能 |
|------|------|
| ← / → | 左右移动方块 |
| ↑ | 旋转方块 |
| ↓ | 加速下降 |
| 空格 | 快速下落 |
| P | 暂停/继续 |
| "New Game" 按钮 | 开始新游戏 |

## 游戏规则

1. 方块从顶部下落
2. 使用方向键移动和旋转方块
3. 当一行被完全填满时，该行被消除
4. 消除一行获得分数
5. 随着消除行数增加，游戏难度和分数倍数都会提升
6. 当方块堆叠到顶部时，游戏结束

## 得分系统

- 消除 1 行：100 × 等级
- 消除 2 行：300 × 等级
- 消除 3 行：500 × 等级
- 消除 4 行（四连消）：800 × 等级
- 每消除 10 行升一个等级

## 项目结构

```
tetris-qt6/
├── CMakeLists.txt          # CMake 构建配置
├── README.md               # 项目文档
└── src/
    ├── main.cpp            # 主程序入口
    ├── gamewindow.h/cpp    # 游戏窗口 UI
    ├── gameboard.h/cpp     # 游戏板逻辑
    └── tetromino.h/cpp     # 方块定义
```

## 代码说明

### Tetromino 类
处理俄罗斯方块的形状、颜色和旋转逻辑。支持 7 种标准方块类型：
- I （直线，青色）
- O （正方形，黄色）
- T （T形，紫色）
- S （S形，绿色）
- Z （Z形，红色）
- J （J形，蓝色）
- L （L形，橙色）

### GameBoard 类
管理游戏板的状态、方块移动、碰撞检测和消行逻辑。
- 10×20 的游戏区域
- 实时碰撞检测
- 自动下降速度控制
- 行消除判定

### GameWindow 类
Qt6 用户界面，负责渲染和事件处理。
- 实时绘制游戏板
- 键盘输入处理
- 游戏循环控制
- 信息面板显示

## 许可证

MIT License

## 作者

创建于 2026
