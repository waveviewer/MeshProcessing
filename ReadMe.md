# Mesh Simplification
## 功能说明
**三角网格化简**指的是对于面片数量巨大的稠密三角网格模型，在尽可能少改变原有模型几何特征（模型的拓扑结构、模型体积、平面、边缘等）的条件下，使用更少的面片数量完成对模型的建模表达，让简化后的轻量模型具有和原始模型相似的视觉效果。

本工具的主要功能是读取一个三角网格模型文件，对此模型进行化简，将化简结果写入本地文件。

## 注意事项
1. 读入的mesh文件要求是满足2-流形条件的三角网格，否则无法处理。
2. 本工具只能化简模型的几何，不能处理uv贴图、顶点颜色等材质属性

## 用法
### 帮助信息
```
Mesh Simplification: if no params passed in, default execution is -c ../configs/config.json
Usage: D:\VisualStudioProjects\meshprocessing\build\Release\MeshProcessing.exe [OPTIONS]

Options:
  -h,--help                   Print this help message and exit
  -c,--config TEXT:FILE       i.e. ../configs/config.json, if config is set, other options will be ignored
  -r,--ratio FLOAT:FLOAT in [0 - 1]
                              stop ratio, 0.0 < ratio < 1.0, default is 0.1
  -i,--input TEXT:FILE        input mesh filepath, format supports obj,ply
  -o,--output TEXT            save result in obj file
  -l,--logger TEXT            logs folder path
  -m,--method INT             simplify method, int type from 0 to 2
```

### 命令行参数说明
- `-i` 需要简化的三角网格模型文件路径
- `-r` 简化比例，要求输入0.0-1.0之间的浮点数，默认为0.1
- `-o` 简化后的结果文件输出路径，支持 .obj, .ply文件格式
- `-l` 日志文件夹位置，日志信息将写入此文件夹下
- `-m` 指定化简方法，要求输入化简方法的整数代号，默认为0
  - 0 : GH_STANDARD, Garland-Heckbert(QEM)简化，速度最快
  - 1 : LT_STANDARD, Lindstrom-Turk简化
  - 2 : LT_KEEP_BOUNDARY, 保持边界不变的Lindstrom-Turk简化
- `-c` config.json 配置文件路径，如果指定了配置文件，上述命令行参数失效，直接读取配置文件中的参数

### config.json 配置文件示例
```json
{
  "mesh path": "D:\\Mesh\\Armadillo.ply",
  "save path": "D:\\VisualStudioProjects\\meshprocessing\\result\\GH_01_Armadillo.ply",
  "logger path":"D:\\VisualStudioProjects\\meshprocessing\\logs",
  "ratio": 0.1,
  "method": 0
}
```
json文件中的全部键值对要求完整填写，不可省略。每个键值对的具体含义和上述的命令行参数保持一致。

### 示例用法1：指定完整的命令行参数
```
.\MeshProcessing.exe -i D:\Mesh\Armadillo.ply -o D:\VisualStudioProjects\meshprocessing\result\LT_02_Armadillo.ply -l D:\VisualStudioProjects\meshprocessing\logs -r 0.2 -m 1
```

对应生成的日志文件
```
[2022-06-15 12:20:06.283] [info] read mesh takes 425 ms
[2022-06-15 12:20:06.284] [info] simplify mesh : D:\Mesh\Armadillo.ply, ratio is 0.2, method is 1
[2022-06-15 12:20:06.284] [info] LT_standard() start
[2022-06-15 12:20:13.981] [info] LT_standard simplification, 415134 edges collapsed in 7697 ms
[2022-06-15 12:20:14.012] [info] save result in D:\VisualStudioProjects\meshprocessing\result\LT_02_Armadillo.ply
```


### 示例用法2：缺省部分命令行参数
```
.\MeshProcessing.exe -i D:\Mesh\Armadillo.ply -o D:\VisualStudioProjects\meshprocessing\result\GH_01_Armadillo.obj -l
D:\VisualStudioProjects\meshprocessing\logs
```

对应生成的日志文件
```
[2022-06-15 12:25:33.290] [info] read mesh takes 425 ms
[2022-06-15 12:25:33.290] [info] simplify mesh : D:\Mesh\Armadillo.ply, ratio is 0.1, method is 0
[2022-06-15 12:25:33.290] [info] GH_QEM_standard() start
[2022-06-15 12:25:40.936] [info] GH_QEM_standard, 467025 edges collapsed in 7642 ms
[2022-06-15 12:25:41.045] [info] save result in D:\VisualStudioProjects\meshprocessing\result\GH_01_Armadillo.obj
```

### 示例用法3：指定配置文件
```
.\MeshProcessing.exe -c D:\VisualStudioProjects\meshprocessing\configs\config.json
```

配置文件内容
```
{
  "mesh path": "D:\\Mesh\\Armadillo.ply",
  "save path": "D:\\VisualStudioProjects\\meshprocessing\\result\\GH_03_Armadillo.ply",
  "logger path":"D:\\VisualStudioProjects\\meshprocessing\\logs",
  "ratio": 0.3,
  "method": 0
}
```

对应生成的日志文件
```
[2022-06-15 12:27:51.163] [info] read mesh takes 426 ms
[2022-06-15 12:27:51.164] [info] simplify mesh : D:\Mesh\Armadillo.ply, ratio is 0.1, method is 0
[2022-06-15 12:27:51.164] [info] GH_QEM_standard() start
[2022-06-15 12:27:55.766] [info] GH_QEM_standard, 467025 edges collapsed in 4598 ms
[2022-06-15 12:27:55.785] [info] save result in D:\VisualStudioProjects\meshprocessing\result\GH_01_Armadillo.ply
```